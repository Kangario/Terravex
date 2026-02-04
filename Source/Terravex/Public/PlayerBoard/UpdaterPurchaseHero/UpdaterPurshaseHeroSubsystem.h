// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Saves/SaveUserData.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "TerravexInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UpdaterPurshaseHeroSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnPurshaseUpdated,
	TSharedPtr<FJsonObject>
);

UCLASS()
class TERRAVEX_API UUpdaterPurshaseHeroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FOnPurshaseUpdated OnPurshaseUpdated;
	UFUNCTION()
	void RequestUpdatePurshase();
private:
	void ResponseUpdatePurshase(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void DisplayDebugInfoPurshaseUpdate(TSharedPtr<FJsonObject> JsonObject);
	UPROPERTY()
	UTerravexInstance* GI;
};
