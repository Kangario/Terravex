// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/InterfaceState.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SceneManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TERRAVEX_API USceneManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void TravelToLevel(FName LevelName);
	
private:
	AInterfaceState* InterfaceState;
};
