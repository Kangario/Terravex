// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/BattleGroupController/BattleGroupMenuController.h"
#include "Shop/ShopShowing/ShopCardFactory.h"
#include "TerravexInstance.h"
#include "PlayerBoard/EquipmentHero/EquipmentHeroSubsystem.h"
#include "PlayerBoard/UpdaterPurchaseHero/UpdaterPurshaseHeroSubsystem.h"
#include "UI/Controllers/EquipmentHeroCardController/EquipmentHeroCardController.h"

void UBattleGroupMenuController::NativeConstruct()
{
	Super::NativeConstruct();
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	CardFactory = NewObject<UShopCardFactory>(this);
	CardFactory->Init(GetWorld());
	BackToMenuButton->OnClicked.AddDynamic(this, &UBattleGroupMenuController::BackToMainMenu);
	if (auto* PurshaseSubsystem =
		GetGameInstance()->GetSubsystem<UUpdaterPurshaseHeroSubsystem>())
	{
		PurshaseSubsystem->OnPurshaseUpdated.AddUObject(
			this,
			&UBattleGroupMenuController::UpdatePurchasedHero
		);
	}
	EquipmentSlotHeroes.Empty();
	EquipmentSlotHeroes.Reserve(6);

	if (Slot_1) EquipmentSlotHeroes.Add(Slot_1);
	if (Slot_2) EquipmentSlotHeroes.Add(Slot_2);
	if (Slot_3) EquipmentSlotHeroes.Add(Slot_3);
	if (Slot_4) EquipmentSlotHeroes.Add(Slot_4);
	if (Slot_5) EquipmentSlotHeroes.Add(Slot_5);
	if (Slot_6) EquipmentSlotHeroes.Add(Slot_6);
	
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UEquipmentHeroSubsystem* EquipmentSubsystem =
				GI->GetSubsystem<UEquipmentHeroSubsystem>())
			{
				EquipmentSubsystem->RequestUpdateEquipment();
				EquipmentSubsystem->OnEquipmentUpdated.AddUObject(
					this,
					&UBattleGroupMenuController::GenerateEquipmentHeroCardHandler
					);
			}
		}
	}
}

void UBattleGroupMenuController::UpdatePurchase()
{
	UTerravexInstance* TerravexInstance = Cast<UTerravexInstance>(GetWorld()->GetGameInstance());
	UUpdaterPurshaseHeroSubsystem* PurchaseUpdateController = TerravexInstance->GetSubsystem<UUpdaterPurshaseHeroSubsystem>();
	PurchaseUpdateController->RequestUpdatePurshase();
}

void UBattleGroupMenuController::UpdatePurchasedHero(TSharedPtr<FJsonObject> ShopJson)
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
			Card->SetTextButton("Equipment");
			Card->SetHero(Data);
			Card->OnCardPurchased.BindUObject(
				this,
				&UBattleGroupMenuController::EquipmentHeroHandler
			);
			
		}
	}
);
}

void UBattleGroupMenuController::EquipmentHeroHandler(UCharacterCardController* characterCardController)
{
	if (!characterCardController)
		return;

	UHero* Hero = characterCardController->GetHero();

	if (!Hero)
		return;

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UEquipmentHeroSubsystem* PurchaseSubsystem =
				GI->GetSubsystem<UEquipmentHeroSubsystem>())
			{
				PurchaseSubsystem->RequestEquipment(Hero);
				PurchaseSubsystem->OnEquipment.AddUObject(
					this,
					&UBattleGroupMenuController::GenerateEquipmentHeroCardHandler
					);
			}
		}
	}

	characterCardController->RemoveFromParent();
}

void UBattleGroupMenuController::GenerateEquipmentHeroCardHandler(TSharedPtr<FJsonObject> heroes)
{
	CardFactory->BuildCardsInSlots(
	heroes,
	TEXT("equipmentHeroes"),
	this,
	EquipmentSlotHeroes,
	EquipmentHeroCardWidgetClass,
	[this](UUserWidget* CardWidget, const TSharedPtr<FJsonObject>& Data)
	{
		if (auto* Card = Cast<UEquipmentHeroCardController>(CardWidget))
		{
			Card->SetHero(Data);
		}
	}
);
}


void UBattleGroupMenuController::BackToMainMenu()
{
	InterfaceState->SwithInterface(EMenuState::MainMenu);
}
