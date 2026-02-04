// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Character/Hero.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Saves/SaveUserData.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "TerravexInstance.h"
#include "HeroPurchaseSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHeroPurchased, UHero*);

UCLASS()
class TERRAVEX_API UHeroPurchaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void RequestPurchase(UHero* Hero);
	
	FOnHeroPurchased OnHeroPurchased;
private:
	void HandlePurchaseResponse(
		FHttpRequestPtr Request,
		FHttpResponsePtr Response,
		bool bWasSuccessful
	);

private:
	UPROPERTY()
	UHero* PendingHero = nullptr;
	UPROPERTY()
	UTerravexInstance* GI;
};
