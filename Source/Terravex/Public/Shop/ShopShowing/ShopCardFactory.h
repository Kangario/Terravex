// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/Controllers/CharacterCardController/CharacterCardController.h"

#include "UI/Controllers/ShopMenuController/ShopMenuController.h"
#include "ShopCardFactory.generated.h"

UCLASS()
class TERRAVEX_API UShopCardFactory : public UObject
{
	GENERATED_BODY()
public: 
	void Init(UWorld* InWorld);

	void BuildCardsInSlots(
	const TSharedPtr<FJsonObject>& Json,
	const FString& ArrayFieldName,
	UUserWidget* OwnerWidget,
	const TArray<UPanelWidget*>& Slots,
	TSubclassOf<UUserWidget> CardWidgetClass,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)> InitCardFunc
);
	void BuildCards(
	const TSharedPtr<FJsonObject>& Json,
	const FString& ArrayFieldName,
	UUserWidget* OwnerWidget,
	UPanelWidget* Container,
	TSubclassOf<UUserWidget> CardWidgetClass,
	int32 CardsPerRow,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)> InitCardFunc);
	void MakeGenericCard(
	const TSharedPtr<FJsonObject>& ItemData,
	UUserWidget* OwnerWidget,
	UHorizontalBox* Row,
	TSubclassOf<UUserWidget> CardWidgetClass,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)>);
private:
	UPROPERTY()
	UWorld* World;
};
