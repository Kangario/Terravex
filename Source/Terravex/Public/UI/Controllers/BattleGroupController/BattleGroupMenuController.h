// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "UI/InterfaceState.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Dom/JsonObject.h"
#include "Shop/ShopShowing/ShopCardFactory.h"
#include "BattleGroupMenuController.generated.h"

/**
 * 
 */
UCLASS()
class TERRAVEX_API UBattleGroupMenuController : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	UFUNCTION()
	void BackToMainMenu();
	void UpdatePurchase();
	void UpdatePurchasedHero(TSharedPtr<FJsonObject> ShopJson);
	void EquipmentHeroHandler(UCharacterCardController* characterCardController);
	void GenerateEquipmentHeroCardHandler(TSharedPtr<FJsonObject> heroes);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AInterfaceState* InterfaceState;
	UPROPERTY(EditDefaultsOnly, Category = "BattleGroup")
	TSubclassOf<UUserWidget> CardWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "BattleGroup")
	TSubclassOf<UUserWidget> EquipmentHeroCardWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "BattleGroup")
	TArray<UPanelWidget*> EquipmentSlotHeroes;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollCards;
protected:
	UPROPERTY()
	UShopCardFactory* CardFactory;
	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_1;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_2;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_3;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_4;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_5;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Slot_6;
};
