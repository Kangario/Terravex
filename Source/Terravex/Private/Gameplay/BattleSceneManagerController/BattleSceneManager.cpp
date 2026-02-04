// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/BattleSceneManagerController/BattleSceneManager.h"

#include "Structurs/DeployData/DeployData.h"

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
		GetGameInstance()->GetSubsystem<UMatchSessionSubsystem>();

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
	
	if (Type == TEXT("deployment_start"))
	{
		HandlerBattleDeployment(Root);
		return;
	}

	if (Type == TEXT("turn_result"))
	{
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


void UBattleSceneManager::SendJoinRequest()
{
	if (!SocketClient)
		return;

	TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
	Json->SetStringField(TEXT("type"), TEXT("join"));
	Json->SetStringField(TEXT("matchId"), MatchID);
	Json->SetStringField(TEXT("userId"), LocalUserID);

	FString Payload;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Payload);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Sending join request"));

	SocketClient->Send(Payload);
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

void UBattleSceneManager::HandlerBattleDeployment(TSharedPtr<FJsonObject> Root)
{
	UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] Battle Deploy received"));
	int32 durationTemp = 15000;
	TArray<FDeployData> allowedRowsMap;
	if (!Root->TryGetNumberField(TEXT("duration"), durationTemp))
	{
		UE_LOG(LogTemp, Log, TEXT("[BattleSceneManager] durationTemp = %d") , durationTemp);
	}
	
	TSharedPtr<FJsonObject> AllowedRowsObj = Root->GetObjectField(TEXT("allowedRows"));
	
		for (const auto& Pair : AllowedRowsObj->Values)
		{
			const FString& TeamKey = Pair.Key;
			const TSharedPtr<FJsonValue>& Value = Pair.Value;

			if (Value->Type != EJson::Array)
				continue;

			TArray<int32> Rows;
			for (const TSharedPtr<FJsonValue>& Elem : Value->AsArray())
			{
				Rows.Add(Elem->AsNumber());
			}

			FDeployData data;
			data.AllowedRows = Rows;
			data.Team = TeamKey;
			allowedRowsMap.Add(data);
		}
	

	// 3️⃣ Лог для проверки
	UE_LOG(LogTemp, Log, TEXT("Deployment duration: %d ms"), durationTemp);

	for (const auto& Pair : allowedRowsMap)
	{
		FString RowsStr;
		for (int32 Row : Pair.AllowedRows)
		{
			RowsStr += FString::FromInt(Row) + TEXT(" ");
		}

		UE_LOG(LogTemp, Log, TEXT("Allowed rows %s: %s"),
			*Pair.Team,
			*RowsStr
		);
	}
	OnDeployReady.Broadcast(durationTemp,allowedRowsMap);
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
	OnBattleReady.Broadcast(CurrentBattleState); 
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
