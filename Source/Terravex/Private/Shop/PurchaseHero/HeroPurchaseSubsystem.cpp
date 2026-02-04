// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/PurchaseHero/HeroPurchaseSubsystem.h"

#include "CreateUser/CreateUserSubsystem.h"

#define SAVE_SLOT TEXT("UserData")

void UHeroPurchaseSubsystem::RequestPurchase(UHero* Hero)
{
	GI = Cast<UTerravexInstance>(GetGameInstance());
	if (!Hero)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Purchase] Hero is null"));
		return;
	}

	PendingHero = Hero; 

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
		UE_LOG(LogTemp, Log, TEXT("[AuthSubsystem] Loaded UserId: %s"), *UserId);
	}

	TSharedPtr<FJsonObject> Json = MakeShared<FJsonObject>();
	Json->SetStringField(TEXT("userId"), UserId);
	Json->SetStringField(TEXT("heroId"), Hero->GetId());

	FString Body;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(TEXT("https://terravexshopserver.onrender.com/shop/buy"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(Body);
	
	Request->OnProcessRequestComplete().BindUObject(
		this,
		&UHeroPurchaseSubsystem::HandlePurchaseResponse
	);

	Request->ProcessRequest();
}

void UHeroPurchaseSubsystem::HandlePurchaseResponse(
	FHttpRequestPtr Request,
	FHttpResponsePtr Response,
	bool bWasSuccessful
)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[Purchase] Request failed"));
		PendingHero = nullptr;
		return;
	}

	const int32 Code = Response->GetResponseCode();
	const FString Body = Response->GetContentAsString();

	UE_LOG(LogTemp, Log, TEXT("[Purchase] Response %d: %s"), Code, *Body);

	if (Code != 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Purchase] Failed: %s"), *Body);
		PendingHero = nullptr;
		return;
	}

	// ✅ УСПЕХ
	UE_LOG(LogTemp, Log, TEXT("[Purchase] Hero purchased: %s"),
		PendingHero ? *PendingHero->GetNameHero() : TEXT("Unknown"));

	// 🔥 Тут можешь:
	// - убрать карточку
	// - обновить золото
	// - отправить событие в UI

	PendingHero = nullptr;
}
