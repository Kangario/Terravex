// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Terravex/Private/Structurs/NetworkStructurs/UserData.h"
#include "SaveUserData.generated.h"

UCLASS()
class TERRAVEX_API USaveUserData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SaveUserData();
	UFUNCTION()
	void SetUserData(FTUserData userData);
	UPROPERTY(BlueprintReadWrite)
	FString SavedToken;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFirstLaunch = true;
	
protected:
	UPROPERTY()
	FTUserData UserData;
};
