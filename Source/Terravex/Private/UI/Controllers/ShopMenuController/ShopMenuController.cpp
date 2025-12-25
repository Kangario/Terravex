#include "UI/Controllers/ShopMenuController/ShopMenuController.h"

#include "TerravexInstance.h"


void UShopMenuController::NativeConstruct()
{
	Super::NativeConstruct();
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	BackToMenuButton->OnClicked.AddDynamic(this, &UShopMenuController::BackToMainMenu);
	UTerravexInstance* terravexInstance = Cast<UTerravexInstance>(GetGameInstance());
	if (terravexInstance->canUpdate == true)
	{
		for (int i=0; i< NumberOfCards; i++)
		{
			UpdateShop();
		}
	}
	terravexInstance->canUpdate = false;
}

void UShopMenuController::BackToMainMenu()
{
	InterfaceState->SwithInterface(EMenuState::MainMenu);
}

void UShopMenuController::UpdateShop()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AuthSlot"), 0))
	{
		USaveUserData* PlayerAuthData = Cast<USaveUserData>(
			UGameplayStatics::LoadGameFromSlot(TEXT("AuthSlot"), 0)
		);
		if (PlayerAuthData->bCanUpdateShop)
		{
			RequestUpdateShop();
		}
		
	}
}

void UShopMenuController::RequestUpdateShop()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL("http://localhost:3000/CreateHero");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	
	Request->OnProcessRequestComplete().BindUObject(this, &UShopMenuController::MakeCharacter);

	Request->ProcessRequest();
}

void UShopMenuController::MakeCharacter(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		return;
	}

	FString ResponseString = Response->GetContentAsString();
	UE_LOG(LogTemp, Log, TEXT("Server response: %s"), *ResponseString);
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		AddHeroToScrollView(JsonObject);
	}
	
}

void UShopMenuController::AddHeroToScrollView(TSharedPtr<FJsonObject> JsonObject)
{
	if (!JsonObject) return;
	
	UCharacterCardController* Card = CreateWidget<UCharacterCardController>(GetWorld(), CardWidgetClass);
	if (!Card) return;

	Card->SetHero(JsonObject);
	
	cardContainer.Add(Card);
	
	if (cardContainer.Num() >= 2)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		for (UCharacterCardController* C : cardContainer)
		{
			UHorizontalBoxSlot* HorizontalLine = NewRow->AddChildToHorizontalBox(C);
			if (HorizontalLine)
			{
				HorizontalLine->SetPadding(FMargin(50, 0, 50, 25));
				HorizontalLine->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
			}
		}
		
		ScrollCards->AddChild(NewRow);
		
		cardContainer.Empty();
	}
}