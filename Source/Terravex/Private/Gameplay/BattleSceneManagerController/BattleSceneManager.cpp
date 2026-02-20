// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/BattleSceneManagerController/BattleSceneManager.h"

#include "Enums/BattleEventType/BattleEventType.h"
#include "Structurs/DeployData/DeployData.h"
#include "Structurs/UserUnitPositionState/UserUnitPositionState.h"

void UBattleSceneManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	State = EBattleConnectionState::Idle;
	JoinRetryCount = 0;
	SocketClient = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Initialize, State reset to Idle"));
}

void UBattleSceneManager::Deinitialize()
{
	Super::Deinitialize();

	State = EBattleConnectionState::Idle;
	SocketClient = nullptr;
	JoinRetryCount = 0;

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Deinitialize"));
}

void UBattleSceneManager::RegisterSceneController(
	ABattleSceneController* Controller)
{
	SceneController = Controller;

	SceneController->OnBattleSceneBuilt.AddUObject(
		this,
		&UBattleSceneManager::SendDeployRequest
	);
	SceneController->OnDeployReady.AddUObject(
		this,
		&UBattleSceneManager::SendDeployReadyRequest);
	
	SceneController->OnMoveRequested.AddUObject(
	this,
	&UBattleSceneManager::SendMoveRequest);
	
	SceneController->OnAttackRequested.AddUObject(
	this,
	&UBattleSceneManager::SendAttackRequest);
	
	SceneController->OnTurnEnd.AddUObject(
	this,
	&UBattleSceneManager::HandleTurnEnd);
}

void UBattleSceneManager::StartBattle()
{
	if (State != EBattleConnectionState::Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleSceneManager] StartBattle skipped: already started"));
		return;
	}

	if (!InitializeSessionData())
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleSceneManager] Session not ready, StartBattle aborted"));
		return;
	}

	CreateSocket();
	State = EBattleConnectionState::Connecting;

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Connecting to battle server"));
	SocketClient->Connect();
}

void UBattleSceneManager::EndBattle()
{
	if (State == EBattleConnectionState::Idle)
		return;

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] EndBattle"));

	State = EBattleConnectionState::Idle;
	JoinRetryCount = 0;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(JoinRetryTimer);
	}

	if (SocketClient)
	{
		SocketClient->Close();
		SocketClient = nullptr;
	}

	CurrentBattleState = FBattleState();
}


bool UBattleSceneManager::InitializeSessionData()
{
	UMatchSessionSubsystem* Session =
	GetWorld()->GetGameInstance()->GetSubsystem<UMatchSessionSubsystem>();

	if (!Session)
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] MatchSessionSubsystem not found"));
		return false;
	}

	LocalUserID = Session->GetUserId();
	MatchID = Session->GetMatchId();

	return !LocalUserID.IsEmpty() && !MatchID.IsEmpty();
}

void UBattleSceneManager::CreateSocket()
{
	SocketClient = NewObject<UBattleSocketClient>(this);

	SocketClient->OnConnected.BindUObject(this, &UBattleSceneManager::OnSocketConnected);
	SocketClient->OnMessage.BindUObject(this, &UBattleSceneManager::OnSocketMessage);
	SocketClient->OnClosed.BindUObject(this, &UBattleSceneManager::OnSocketClosed);
}


void UBattleSceneManager::OnSocketConnected()
{
	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Connected to battle server"));

	JoinRetryCount = 0;
	SendJoinRequest();
}


void UBattleSceneManager::OnSocketMessage(const FString& Message)
{
	TSharedPtr<FJsonObject> Root;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] Invalid JSON received"));
		return;
	}

	const FString Type = Root->GetStringField(TEXT("type"));

	if (Type == TEXT("battle_init"))
	{
		HandleBattleInit(Root);
		return;
	}
	
	if (Type == TEXT("events"))
	{
		HandlerBattleEvents(Root);
		return;
	}

	if (Type == TEXT("error"))
	{
		HandleServerError(Root);
		return;
	}
}


void UBattleSceneManager::OnSocketClosed()
{
}

void UBattleSceneManager::SendMessage(TSharedPtr<FJsonObject> Json)
{
	if (!SocketClient)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleSceneManager] SocketClient invalid"));
		return;
	}

	FString Payload;

	TSharedRef<TJsonWriter<>> Writer =
		TJsonWriterFactory<>::Create(&Payload);

	if (!FJsonSerializer::Serialize(Json.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] Failed serialize"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] >> %s"), *Payload);

	SocketClient->Send(Payload);
}

void UBattleSceneManager::SendJoinRequest()
{
	TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();

	Json->SetStringField(TEXT("type"), TEXT("join"));
	Json->SetStringField(TEXT("matchId"), MatchID);
	Json->SetStringField(TEXT("userId"), LocalUserID);

	SendMessage(Json);
}

void UBattleSceneManager::SendDeployRequest()
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	Root->SetStringField(TEXT("type"), TEXT("action"));

	TSharedPtr<FJsonObject> Action = MakeShared<FJsonObject>();

	Action->SetStringField(TEXT("type"), TEXT("deploy_start"));

	Root->SetObjectField(TEXT("action"), Action);

	SendMessage(Root);
}

void UBattleSceneManager::SendDeployReadyRequest(TArray<FString> unitId, TArray<TArray<int32>> unitPosition)
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
	Root->SetStringField(TEXT("type"), TEXT("action"));

	TSharedPtr<FJsonObject> Action = MakeShared<FJsonObject>();
	Action->SetStringField(TEXT("type"), TEXT("deploy_ready"));
	Action->SetStringField(TEXT("userId"), LocalUserID);

	TSharedPtr<FJsonObject> Units = MakeShared<FJsonObject>();
	
	for (int32 i = 0; i < unitId.Num(); ++i)
	{
		TSharedPtr<FJsonObject> Unit = MakeShared<FJsonObject>();

		TArray<TSharedPtr<FJsonValue>> PositionArray;

		for (int32 value : unitPosition[i])
		{
			PositionArray.Add(MakeShared<FJsonValueNumber>(value));
		}

		Unit->SetArrayField(TEXT("position"), PositionArray);

		Units->SetObjectField(unitId[i], Unit);
	}

	Action->SetObjectField(TEXT("units"), Units);
	Root->SetObjectField(TEXT("action"), Action);

	SendMessage(Root);
}

void UBattleSceneManager::HandleJoinRetry()
{
	++JoinRetryCount;

	if (JoinRetryCount > MaxJoinRetries)
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] Join failed after %d retries"), JoinRetryCount);
		EndBattle();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[BattleSceneManager] Retry join (%d/%d)"),
		JoinRetryCount, MaxJoinRetries);

	SendJoinRequest();
}

void UBattleSceneManager::HandlerBattleEvents(TSharedPtr<FJsonObject> Root)
{
	if (!Root.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[HandlerBattleEvents] Root invalid"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* EventsArray;

	if (!Root->TryGetArrayField(TEXT("events"), EventsArray))
	{
		UE_LOG(LogTemp, Error, TEXT("[HandlerBattleEvents] Missing 'events' array"));
		return;
	}

	for (const TSharedPtr<FJsonValue>& EventValue : *EventsArray)
	{
		if (!EventValue.IsValid() || EventValue->Type != EJson::Object)
		{
			UE_LOG(LogTemp, Warning, TEXT("[HandlerBattleEvents] Invalid event entry"));
			continue;
		}

		TSharedPtr<FJsonObject> EventObj = EventValue->AsObject();

		FString TypeEvent;

		if (!EventObj->TryGetStringField(TEXT("type"), TypeEvent))
		{
			UE_LOG(LogTemp, Warning, TEXT("[HandlerBattleEvents] Event missing type"));
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("[HandlerBattleEvents] Processing event: %s"), *TypeEvent);
		
		if (TypeEvent == TEXT("DEPLOYMENT")){
			DeployEvent(EventObj);
		}else if (TypeEvent == TEXT("deployment_player_ready"))
		{
			UE_LOG(LogTemp, Warning, TEXT("[HandlerBattleEvents] Deployment ready"));
			
		}else if (TypeEvent == TEXT("deployment_end"))
		{
			HandleMove(EventObj);
			OnEndDeploy.Broadcast();
		}else if (TypeEvent == TEXT("turn_start"))
		{
			HandleTurnStart(EventObj);
		}else if (TypeEvent == TEXT("turn_end"))
		{
			OnTurnEnd.Broadcast();
		}else if (TypeEvent == TEXT("unit_move"))
		{
			HandleMove(EventObj);
		}else if (TypeEvent == TEXT("damage"))
		{
			HandleApplyDamage(EventObj);
		}
		
	}
	
}

void UBattleSceneManager::HandleTurnStart(TSharedPtr<FJsonObject> Root)
{
	int32 duration;

	if (Root->TryGetNumberField(TEXT("duration"), duration))
	{
		if (!Root->TryGetStringField(TEXT("activeUnitId"), CurrentBattleState.activeUnitId))
		{
			UE_LOG(LogTemp, Log, TEXT("[UBattleSceneManager] Not have activeUnit"));
			return;
		}

		const TArray<TSharedPtr<FJsonValue>>* InitiativeArray;

		if (Root->TryGetArrayField(TEXT("initiative"), InitiativeArray))
		{
			CurrentBattleState.InitiativeOrder.Empty();

			for (const TSharedPtr<FJsonValue>& Value : *InitiativeArray)
			{
				FString IdString = FString::FromInt((int32)Value->AsNumber());
				CurrentBattleState.InitiativeOrder.Add(IdString);
			}
		}
		OnTurnStartDuration.Broadcast(duration, CurrentBattleState, LocalUserID);
		HandleMove(Root);
	}else
	{
		
	}
}

void UBattleSceneManager::HandleTurnEnd()
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	Root->SetStringField(TEXT("type"), TEXT("action"));

	TSharedPtr<FJsonObject> Action = MakeShared<FJsonObject>();

	Action->SetStringField(TEXT("type"), TEXT("turn_end"));
	Action->SetStringField(TEXT("userId"), LocalUserID);

	Root->SetObjectField(TEXT("action"), Action);

	SendMessage(Root);
}

void UBattleSceneManager::HandleMove(TSharedPtr<FJsonObject> Root)
{
	FUserUnitPositionState userUnitPosition;
	if (!Root->TryGetStringField(TEXT("userId"), userUnitPosition.userId))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBattleSceneManager][HandleMove] Event missing userId"));
		userUnitPosition.userId = TEXT("");
	}
	const TSharedPtr<FJsonObject>* UnitsObject;
	if (Root->TryGetObjectField(TEXT("units"), UnitsObject))
	{
		const TMap<FString, TSharedPtr<FJsonValue>>& UnitsMap = (*UnitsObject)->Values;

		for (const auto& Pair : UnitsMap)
		{
			FUnitPositionState unitPosition;
			FGridCell cell;
			cell.X = 0;
			cell.Y = 0;
			cell.Z = 0;
			unitPosition.unitId = Pair.Key;

			TSharedPtr<FJsonObject> UnitData = Pair.Value->AsObject();

			if (!UnitData.IsValid())
				continue;
			const TArray<TSharedPtr<FJsonValue>>* PositionArray;

			if (UnitData->TryGetArrayField(TEXT("position"), PositionArray))
			{
				if (PositionArray->Num() >= 2)
				{
					cell.X = (int32)(*PositionArray)[0]->AsNumber();
					cell.Y = (int32)(*PositionArray)[1]->AsNumber();

					UE_LOG(LogTemp, Warning, TEXT("Unit %s -> X:%d Y:%d"),
						*unitPosition.unitId, cell.X, cell.Y);
				}
			}
			unitPosition.cell = cell;
			userUnitPosition.units.Add(unitPosition);
		}
	}
		OnUnitMove.Broadcast(userUnitPosition);
}

void UBattleSceneManager::SendMoveRequest(FString unitId, FIntPoint position)
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	Root->SetStringField(TEXT("type"), TEXT("action"));

	TSharedPtr<FJsonObject> Action = MakeShared<FJsonObject>();

	Action->SetStringField(TEXT("type"), TEXT("unit_move"));
	Action->SetStringField(TEXT("unitId"), unitId);
	TSharedPtr<FJsonObject> Position = MakeShared<FJsonObject>();
	Position->SetNumberField(TEXT("x"),position.X);
	Position->SetNumberField(TEXT("y"),position.Y);
	
	Action->SetObjectField(TEXT("position"), Position);
	Root->SetObjectField(TEXT("action"), Action);

	SendMessage(Root);
}

void UBattleSceneManager::SendAttackRequest(FString unitId, FString targetUnitId)
{
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	Root->SetStringField(TEXT("type"), TEXT("action"));

	TSharedPtr<FJsonObject> Action = MakeShared<FJsonObject>();

	Action->SetStringField(TEXT("type"), TEXT("unit_attack"));
	Action->SetStringField(TEXT("unitId"), unitId);
	Action->SetStringField(TEXT("targetUnitId"), targetUnitId);
	
	Root->SetObjectField(TEXT("action"), Action);

	SendMessage(Root);
}

void UBattleSceneManager::HandleApplyDamage(TSharedPtr<FJsonObject> Root)
{
	if (!Root.IsValid())
		return;

	FString EventType;

	if (!Root->TryGetStringField(TEXT("type"), EventType))
		return;

	if (EventType != TEXT("damage"))
		return;

	FString attackerId;
	FString targetId;
	int32 hpAfter = 0;
	bool isDead = false;

	Root->TryGetStringField(TEXT("attackerId"), attackerId);
	Root->TryGetStringField(TEXT("targetId"), targetId);

	double HpValue = 0;
	if (Root->TryGetNumberField(TEXT("hpAfter"), HpValue))
	{
		hpAfter = (int32)HpValue;
	}

	Root->TryGetBoolField(TEXT("isDead"), isDead);
	
	OnApplyDamage.Broadcast(attackerId, targetId, hpAfter, isDead);
}

void UBattleSceneManager::DeployEvent(TSharedPtr<FJsonObject> EventObj)
{
	int32 DurationTemp = 0;

	if (!EventObj->TryGetNumberField(TEXT("duration"), DurationTemp))
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEPLOYMENT] Missing duration"));
	}

	const TArray<TSharedPtr<FJsonValue>>* AllowedRowsArray;

	if (!EventObj->TryGetArrayField(TEXT("allowedRows"), AllowedRowsArray))
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEPLOYMENT] Missing allowedRows"));
	}

	TArray<FDeployData> DeployDataList;

	for (int32 i = 0; i < AllowedRowsArray->Num(); ++i)
	{
		const TArray<TSharedPtr<FJsonValue>>& RowArray =
			(*AllowedRowsArray)[i]->AsArray();

		FDeployData Data;
		Data.Team = FString::FromInt(i);

		for (const TSharedPtr<FJsonValue>& RowValue : RowArray)
		{
			if (!RowValue.IsValid())
				continue;

			Data.AllowedRows.Add((int32)RowValue->AsNumber());
		}

		DeployDataList.Add(Data);
	}

	UE_LOG(LogTemp, Log, TEXT("[DEPLOYMENT] Parsed OK. Duration=%d Rows=%d"),
		DurationTemp,
		DeployDataList.Num());

	OnDeployReady.Broadcast(DurationTemp, DeployDataList, CurrentBattleState);
}

void UBattleSceneManager::HandleBattleInit(TSharedPtr<FJsonObject> Root)
{
	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Battle init received"));

	TSharedPtr<FJsonObject> StateObj = Root->GetObjectField(TEXT("state"));

	FBattleState NewState;
	if (!FJsonObjectConverter::JsonObjectToUStruct(
			StateObj.ToSharedRef(),
			&NewState,
			0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] Failed to parse FBattleState"));
		return;
	}

	CurrentBattleState = NewState;
	State = EBattleConnectionState::InBattle;

UE_LOG(LogTemp, Warning, TEXT("Parsed Width=%d Height=%d Heights=%d"),
    NewState.terrain.width,
    NewState.terrain.height,
    NewState.terrain.heights.Num());

	CurrentBattleState.teamId = Root->GetIntegerField(TEXT("teamId"));
	OnBattleReady.Broadcast(LocalUserID ,CurrentBattleState); 
}

void UBattleSceneManager::HandleServerError(TSharedPtr<FJsonObject> Root)
{
	const FString ErrorMsg = Root->GetStringField(TEXT("message"));

	UE_LOG(LogTemp, Error, TEXT("[BattleSceneManager] Server error: %s"), *ErrorMsg);

	if (ErrorMsg == TEXT("match not found"))
	{
		HandleJoinRetry();
	}
}
