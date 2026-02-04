// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Saves/SaveUserData.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "TerravexInstance.h"
#include "MatchmakingSubsystem.generated.h"

UCLASS()
class TERRAVEX_API UMatchmakingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void StartSearch();
	void RequestSearchMatch();
	void RequestCheckMatch();
	void RequestMatchStatus(); 
	void StopTimer();
private:
	bool bSearchFinished = false;
	FTimerHandle MatchmakingTimerHandle;
	void ResponseSearchMath(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UPROPERTY()
	UTerravexInstance* GI;
};
