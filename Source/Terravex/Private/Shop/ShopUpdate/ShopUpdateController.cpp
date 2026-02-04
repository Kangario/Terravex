// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopUpdate/ShopUpdateController.h"

#include "CreateUser/CreateUserSubsystem.h"
#include "UI/InterfaceState.h"

#define SAVE_SLOT TEXT("UserData")

void UShopUpdateController::RequestUpdateShop()
{
	GI = Cast<UTerravexInstance>(GetGameInstance());
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().CreateRequest();

	Request->SetURL(TEXT("https://terravexshopserver.onrender.com/shop/update"));
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
		UE_LOG(LogTemp, Log, TEXT("[ShopSubSystem] Loaded UserId: %s"), *UserId);
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
		&UShopUpdateController::ResponseUpdateShop
	);

	Request->ProcessRequest();
}


void UShopUpdateController::ResponseUpdateShop(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[Shop] HTTP request failed"));
		return;
	}

	const FString ResponseStr = Response->GetContentAsString();
	UE_LOG(LogTemp, Log, TEXT("[Shop] Raw response: %s"), *ResponseStr);

	TSharedPtr<FJsonObject> RootJson;
	
	TSharedRef<TJsonReader<>> Reader =
		TJsonReaderFactory<>::Create(ResponseStr);

	if (!FJsonSerializer::Deserialize(Reader, RootJson) || !RootJson.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[Shop] JSON parse error"));
		return;
	}
	// Seed
	FString ShopSeed = RootJson->GetStringField(TEXT("shopSeed"));
	UE_LOG(LogTemp, Log, TEXT("[Shop] Seed: %s"), *ShopSeed);

	// Heroes array
	const TArray<TSharedPtr<FJsonValue>>* HeroesArray;
	if (!RootJson->TryGetArrayField(TEXT("heroes"), HeroesArray))
	{
		UE_LOG(LogTemp, Error, TEXT("[Shop] Heroes array not found"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Shop] Heroes count: %d"), HeroesArray->Num());
	for (int32 i = 0; i < HeroesArray->Num(); i++)
	{
		const TSharedPtr<FJsonObject> HeroObj =
			(*HeroesArray)[i]->AsObject();

		if (!HeroObj.IsValid())
			continue;
		DisplayDebugInfoShopUpdate(HeroObj);
	}
	OnShopUpdated.Broadcast(RootJson);
	
}

void UShopUpdateController::DisplayDebugInfoShopUpdate(TSharedPtr<FJsonObject> JsonObject)
{
	int32 id        = JsonObject->GetIntegerField(TEXT("Id"));
	FString Name        = JsonObject->GetStringField(TEXT("Name"));
	int32 TypeClass     = JsonObject->GetIntegerField(TEXT("TypeClass"));
	int32 Hp            = JsonObject->GetIntegerField(TEXT("Hp"));
	int32 DamageP       = JsonObject->GetIntegerField(TEXT("DamageP"));
	int32 DamageM       = JsonObject->GetIntegerField(TEXT("DamageM"));
	int32 DefenceP      = JsonObject->GetIntegerField(TEXT("DefenceP"));
	int32 DefenceM      = JsonObject->GetIntegerField(TEXT("DefenceM"));
	int32 Speed         = JsonObject->GetIntegerField(TEXT("Speed"));
	int32 AttackSpeed   = JsonObject->GetIntegerField(TEXT("AttackSpeed"));
	int32 Lvl           = JsonObject->GetIntegerField(TEXT("Lvl"));
	int32 Xp            = JsonObject->GetIntegerField(TEXT("Xp"));

	UE_LOG(LogTemp, Log,
		TEXT("[Id] %d |[Hero] %s | Class=%d | HP=%d | DP=%d | DM=%d | DefP=%d | DefM=%d | Speed=%d | AS=%d | Lvl=%d | XP=%d"),
		id, *Name, TypeClass, Hp, DamageP, DamageM,
		DefenceP, DefenceM, Speed, AttackSpeed, Lvl, Xp
	);
}