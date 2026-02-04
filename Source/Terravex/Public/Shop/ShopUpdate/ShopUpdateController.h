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
#include "ShopUpdateController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnShopUpdated,
	TSharedPtr<FJsonObject>
);

UCLASS()
class TERRAVEX_API UShopUpdateController : public  UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FOnShopUpdated OnShopUpdated;
	UFUNCTION()
	void RequestUpdateShop();
private:
	void ResponseUpdateShop(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void DisplayDebugInfoShopUpdate(TSharedPtr<FJsonObject> JsonObject);
	UPROPERTY()
	UTerravexInstance* GI;
};
