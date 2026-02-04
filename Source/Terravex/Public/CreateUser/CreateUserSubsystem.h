// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "TerravexInstance.h"
#include "UI/InterfaceState.h"
#include "EngineUtils.h"
#include "Enums/InterfaeStates/EMenuState.h"
#include "CreateUserSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TERRAVEX_API UCreateUserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	void CreateUser();

	UFUNCTION(BlueprintCallable)
	bool HasUser() const;

private:
	void LoadUserFromSave();
	void SaveUserToSave();
	void SwithInterface(EMenuState interface);

	void OnCreateUserResponse(
		FHttpRequestPtr Request,
		FHttpResponsePtr Response,
		bool bWasSuccessful
	);
	UPROPERTY()
	UTerravexInstance* GI;
};
