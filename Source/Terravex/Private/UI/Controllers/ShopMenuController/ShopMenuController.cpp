#include "UI/Controllers/ShopMenuController/ShopMenuController.h"
#include "Shop/ShopShowing/ShopCardFactory.h"
#include "TerravexInstance.h"
#include "Shop/PurchaseHero/HeroPurchaseSubsystem.h"
#include "Shop/ShopUpdate/ShopUpdateController.h"


void UShopMenuController::NativeConstruct()
{
	Super::NativeConstruct();
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	BackToMenuButton->OnClicked.AddDynamic(this, &UShopMenuController::BackToMainMenu);
	ButtonUpdateShop->OnClicked.AddDynamic(this, &UShopMenuController::UpdateShop);
	CardFactory = NewObject<UShopCardFactory>(this);
	CardFactory->Init(GetWorld());

	if (auto* ShopSubsystem =
		GetGameInstance()->GetSubsystem<UShopUpdateController>())
	{
		ShopSubsystem->OnShopUpdated.AddUObject(
			this,
			&UShopMenuController::OnShopUpdated
		);
	}
}

void UShopMenuController::BackToMainMenu()
{
	InterfaceState->SwithInterface(EMenuState::MainMenu);
}

void UShopMenuController::UpdateShop()
{
	UTerravexInstance* TerravexInstance = Cast<UTerravexInstance>(GetWorld()->GetGameInstance());
	UShopUpdateController* ShopUpdateController = TerravexInstance->GetSubsystem<UShopUpdateController>();
	ShopUpdateController->RequestUpdateShop();
}

void UShopMenuController::OnShopUpdated( TSharedPtr<FJsonObject> ShopJson)
{
	ScrollCards->ClearChildren(); 
	CardFactory->BuildCards(
	ShopJson,
	TEXT("heroes"),
	this,
	this->ScrollCards,
	this->CardWidgetClass,
	2,
	[this](UUserWidget* CardWidget, const TSharedPtr<FJsonObject>& Data)
	{
		if (auto* Card = Cast<UCharacterCardController>(CardWidget))
		{
			Card->SetHero(Data);
			Card->SetTextButton("Buy");
			Card->OnCardPurchased.BindUObject(
				this,
				&UShopMenuController::HandleCardPurchased
			);
		}
	}	
);
}
void UShopMenuController::HandleCardPurchased(UCharacterCardController* Card)
{
	if (!Card)
		return;

	UHero* Hero = Card->GetHero();

	if (!Hero)
		return;

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UHeroPurchaseSubsystem* PurchaseSubsystem =
				GI->GetSubsystem<UHeroPurchaseSubsystem>())
			{
				PurchaseSubsystem->RequestPurchase(Hero);
			}
		}
	}

	Card->RemoveFromParent();
}