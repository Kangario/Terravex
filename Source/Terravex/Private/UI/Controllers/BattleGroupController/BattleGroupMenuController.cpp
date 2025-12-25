// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/BattleGroupController/BattleGroupMenuController.h"

void UBattleGroupMenuController::NativeConstruct()
{
	Super::NativeConstruct();
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	BackToMenuButton->OnClicked.AddDynamic(this, &UBattleGroupMenuController::BackToMainMenu);
}

void UBattleGroupMenuController::BackToMainMenu()
{
	InterfaceState->SwithInterface(EMenuState::MainMenu);
}
