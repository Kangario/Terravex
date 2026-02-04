// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MatchSessionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TERRAVEX_API UMatchSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void SetSession(FString InUserID, FString InMatchID);
	FString GetUserId();
	FString GetMatchId();
	void RemoveSession(FString InMatchID);
private:
	FString MatchId;
	FString UserID;
		
};
