// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/InterfaceState.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SearchingMenu.generated.h"

UCLASS()
class TERRAVEX_API USearchingMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	UFUNCTION()
	void BackToMainMenu();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AInterfaceState* InterfaceState;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* CancleButton;
};
