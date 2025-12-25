#include "TerravexInstance.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "UI/Controllers/ShopMenuController/ShopMenuController.h"

void UTerravexInstance::Init()
{
	Super::Init();
	
	TArray<FString> Permissions;
	Permissions.Add("android.permission.MANAGE_EXTERNAL_STORAGE");
	UAndroidPermissionFunctionLibrary::AcquirePermissions(Permissions);
	
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AuthSlot"), 0))
	{
		bHasSave = true;

		PlayerAuthData = Cast<USaveUserData>(
			UGameplayStatics::LoadGameFromSlot(TEXT("AuthSlot"), 0)
		);

		PlayerAuthData->bIsFirstLaunch = false; 
		CheckUpdateShop();
		
	}
	else
	{
		PlayerAuthData = Cast<USaveUserData>(
			UGameplayStatics::CreateSaveGameObject(USaveUserData::StaticClass())
		);
		PlayerAuthData->bIsFirstLaunch = true; 
		CheckUpdateShop();
		
	}
}

void UTerravexInstance::InitInterfaceStateController(AInterfaceState* InterfaceState)
{
	UE_LOG(LogTemp, Log, TEXT("This is a log message"));
	GoogleAuthManager = GetSubsystem<UGoogleAuthManager>();
	GoogleAuthManager->SetInterfaceController(InterfaceState);
	if (PlayerAuthData->bIsFirstLaunch == false)
	{
		InterfaceState->SwithInterface(EMenuState::MainMenu);
	}
}

void UTerravexInstance::CheckUpdateShop()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL("http://localhost:3000/shop/check");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetNumberField("lastShopUpdate", PlayerAuthData->LastShopUpdate);

	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(Content);
	
	Request->OnProcessRequestComplete().BindUObject(this, &UTerravexInstance::UpdateLastShopUpdate);

	Request->ProcessRequest();
}

void UTerravexInstance::UpdateLastShopUpdate(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to contact server"));
		return;
	}

	FString ResponseString = Response->GetContentAsString();
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		bool bCanUpdate = JsonObject->GetBoolField(TEXT("canUpdate"));
		if (bCanUpdate)
		{
			int64 ServerTime = JsonObject->GetNumberField(TEXT("serverTime"));
			PlayerAuthData->LastShopUpdate = ServerTime; 
			PlayerAuthData->bCanUpdateShop = bCanUpdate;
			UGameplayStatics::SaveGameToSlot(PlayerAuthData, TEXT("AuthSlot"), 0);
			UE_LOG(LogTemp, Log, TEXT("AllSuceful"));
			TArray<UUserWidget*> FoundWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UShopMenuController::StaticClass(), false);
		}
		else
		{
			int64 TimeLeft = JsonObject->GetNumberField(TEXT("timeLeft"));
			PlayerAuthData->bCanUpdateShop = bCanUpdate;
			UGameplayStatics::SaveGameToSlot(PlayerAuthData, TEXT("AuthSlot"), 0);
			UE_LOG(LogTemp, Log, TEXT("NOOOOOOT"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from server"));
	}
}



