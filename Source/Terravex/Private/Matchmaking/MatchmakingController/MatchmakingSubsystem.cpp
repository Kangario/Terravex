// Fill out your copyright notice in the Description page of Project Settings.


#include "Matchmaking/MatchmakingController/MatchmakingSubsystem.h"

#include "CreateUser/CreateUserSubsystem.h"
#include "Gameplay/MatchSessionController/MatchSessionSubsystem.h"
#include "SceneController/SceneManagerSubsystem.h"

#define SAVE_SLOT TEXT("UserData")

void UMatchmakingSubsystem::StartSearch()
{
	bSearchFinished = false;
	GI = Cast<UTerravexInstance>(GetGameInstance());
	if (GetWorld()->GetTimerManager().IsTimerActive(MatchmakingTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchmakingSubsystem] Search already running"));
		return;
	}
	
	RequestSearchMatch();
	GetWorld()->GetTimerManager().SetTimer(
		MatchmakingTimerHandle,
		this,
		&UMatchmakingSubsystem::RequestCheckMatch,
		2.0f,
		true
	);
}

void UMatchmakingSubsystem::RequestCheckMatch()
{
	// 1️⃣ СНАЧАЛА ПРОВЕРЯЕМ МАТЧ (has_match / waiting_other / match_created)
	RequestMatchStatus();

	// 2️⃣ ПОТОМ ИЩЕМ СОПЕРНИКА
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(TEXT("https://terravexmatchmaking.onrender.com/matchmaking/check"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString UserId = "";
	FString SavedUserId = GI->UserId;

	if (!UGameplayStatics::DoesSaveGameExist(SAVE_SLOT + SavedUserId, 0))
		return;

	USaveUserData* Save = Cast<USaveUserData>(
		UGameplayStatics::LoadGameFromSlot(SAVE_SLOT + SavedUserId, 0)
	);

	if (Save)
	{
		UserId = Save->UserId;
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Loaded UserId: %s"), *UserId);
	}

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("userId"), UserId);

	FString BodyString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
	FJsonSerializer::Serialize(Body.ToSharedRef(), Writer);

	Request->SetContentAsString(BodyString);

	Request->OnProcessRequestComplete().BindUObject(
		this,
		&UMatchmakingSubsystem::ResponseSearchMath
	);

	Request->ProcessRequest();
}


void UMatchmakingSubsystem::RequestSearchMatch()
{
	if (bSearchFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ Ignoring matchmaking response, search already finished"));
		return;
	}
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(TEXT("https://terravexmatchmaking.onrender.com/matchmaking/enqueue"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString UserId = "";
	FString SavedUserId = GI->UserId;
	if (!UGameplayStatics::DoesSaveGameExist(SAVE_SLOT + SavedUserId, 0))
		return;

	USaveUserData* Save = Cast<USaveUserData>(
		UGameplayStatics::LoadGameFromSlot(SAVE_SLOT + SavedUserId, 0)
	);

	if (Save)
	{
		UserId = Save->UserId;
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Loaded UserId: %s"), *UserId);
	}
	

	// JSON body
	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("userId"), UserId);

	FString BodyString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
	FJsonSerializer::Serialize(Body.ToSharedRef(), Writer);

	Request->SetContentAsString(BodyString);

	Request->OnProcessRequestComplete().BindUObject(
		this,
		&UMatchmakingSubsystem::ResponseSearchMath
	);

	Request->ProcessRequest();
}

void UMatchmakingSubsystem::RequestMatchStatus()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(TEXT("https://terravexmatchmaking.onrender.com/matchmaking/match"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString UserId = "";
	FString SavedUserId = GI->UserId;

	if (!UGameplayStatics::DoesSaveGameExist(SAVE_SLOT + SavedUserId, 0))
		return;

	USaveUserData* Save = Cast<USaveUserData>(
		UGameplayStatics::LoadGameFromSlot(SAVE_SLOT + SavedUserId, 0)
	);

	if (Save)
	{
		UserId = Save->UserId;
	}

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("userId"), UserId);

	FString BodyString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
	FJsonSerializer::Serialize(Body.ToSharedRef(), Writer);

	Request->SetContentAsString(BodyString);

	Request->OnProcessRequestComplete().BindUObject(
		this,
		&UMatchmakingSubsystem::ResponseSearchMath
	);

	Request->ProcessRequest();
}


void UMatchmakingSubsystem::ResponseSearchMath(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[MatchmakingSubsystem] HTTP request failed"));
		return;
	}

	const FString ResponseStr = Response->GetContentAsString();
	UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Raw response: %s"), *ResponseStr);

	TSharedPtr<FJsonObject> RootJson;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

	if (!FJsonSerializer::Deserialize(Reader, RootJson) || !RootJson.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[MatchmakingSubsystem] JSON parse error"));
		return;
	}

	FString Status = "";
	if (!RootJson->TryGetStringField(TEXT("status"), Status))
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchmakingSubsystem] No status field"));
		return;
	}

	if (Status == "has_match")
	{
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Match already exists"));
		TSharedPtr<FJsonObject> MatchObj = RootJson->GetObjectField(TEXT("match"));
		FString MatchId = MatchObj->GetStringField(TEXT("id"));
		FString UserID = GI->UserId;
		
		if (UMatchSessionSubsystem* Session =
	  GetGameInstance()->GetSubsystem<UMatchSessionSubsystem>())
		{
			Session->SetSession(UserID, MatchId);

			UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Session saved: User=%s Match=%s"),
				*UserID, *MatchId);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MatchSessionSubsystem NOT FOUND"));
			return; 
		}
		StopTimer();

		if (USceneManagerSubsystem* SceneManager =
			GetGameInstance()->GetSubsystem<USceneManagerSubsystem>())
		{
			SceneManager->TravelToLevel("GameScene");
		}

		return;
	}
	
	if (Status == "match_created")
	{
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Match created!"));
		TSharedPtr<FJsonObject> MatchObj = RootJson->GetObjectField(TEXT("match"));
		FString MatchId = MatchObj->GetStringField(TEXT("id"));
		FString UserID = GI->UserId;
		
		if (UMatchSessionSubsystem* Session =
		GetGameInstance()->GetSubsystem<UMatchSessionSubsystem>())
		{
			Session->SetSession(UserID, MatchId);
		}
		
		StopTimer();

		if (USceneManagerSubsystem* SceneManager =
			GetGameInstance()->GetSubsystem<USceneManagerSubsystem>())
		{
			SceneManager->TravelToLevel("GameScene");
		}

		return;
	}
	
	if (Status == "waiting_other")
	{
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Waiting for other player..."));
		return;
	}
	
	if (Status == "searching")
	{
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Searching..."));
		return;
	}
	
	if (Status == "found")
	{
		UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Opponent found"));
		return;
	}

	if (Status == "not_in_queue")
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchmakingSubsystem] Not in queue anymore"));
		return;
	}

	if (Status == "no_pending")
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchmakingSubsystem] No pending pair"));
		return;
	}

	if (Status == "expired")
	{
		UE_LOG(LogTemp, Warning, TEXT("[MatchmakingSubsystem] Pending expired, restarting search"));
		RequestSearchMatch();
		return;
	}
}


void UMatchmakingSubsystem::StopTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MatchmakingTimerHandle);
	}
	bSearchFinished = true;
	UE_LOG(LogTemp, Log, TEXT("[MatchmakingSubsystem] Matchmaking timer stopped"));
}