// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneController/SceneManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"

void USceneManagerSubsystem::TravelToLevel(FName LevelName)
{
	for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
	{
		InterfaceState = *It;
		break; 
	}
	InterfaceState->RemoveWidgetsFromViewport();
	UGameplayStatics::OpenLevel(GetWorld(),LevelName);
}
