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
#include "Character/Hero.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EquipmentHeroSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnEquipmentUpdated,
	TSharedPtr<FJsonObject>
);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnEquipment,
	TSharedPtr<FJsonObject>
);

UCLASS()
class TERRAVEX_API UEquipmentHeroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FOnEquipment OnEquipment;
	FOnEquipmentUpdated OnEquipmentUpdated;
	UFUNCTION()
	void RequestUpdateEquipment();
	UFUNCTION()
	void RequestEquipment(UHero* Hero);
private:
	void ResponseEquipment(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ResponseEquipmentUpdate(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void DisplayDebugInfoEquipmentUpdate(TSharedPtr<FJsonObject> JsonObject);
	UPROPERTY()
	UTerravexInstance* GI;
	
};
