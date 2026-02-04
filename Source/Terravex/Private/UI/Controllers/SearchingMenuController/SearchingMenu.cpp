// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/SearchingMenuController/SearchingMenu.h"

#include "Matchmaking/MatchmakingController/MatchmakingSubsystem.h"

void USearchingMenu::NativeConstruct()
{
	Super::NativeConstruct();
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	CancleButton->OnClicked.AddDynamic(this, &USearchingMenu::BackToMainMenu);
}

void USearchingMenu::BackToMainMenu()
{
	GetGameInstance()->GetSubsystem<UMatchmakingSubsystem>()->StopTimer();
	InterfaceState->SwithInterface(EMenuState::MainMenu);
}
