// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Character/Hero.h"
#include "Math/UnrealMathUtility.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "EquipmentHeroCardController.generated.h"

UCLASS()
class TERRAVEX_API UEquipmentHeroCardController : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void SetHero(TSharedPtr<FJsonObject> JsonObject);
	UHero* GetHero();
	UFUNCTION()
	void SetIconCharacter();
	UFUNCTION()
	void SetTitleCharacter();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	TMap<ETypeClass , UTexture2D*> CharacterIcons;
	
protected:	
	bool bUIReady = false;
	bool bHeroReady = false;
	
	void UpdateUI();
	UPROPERTY(meta = (BindWidget))
	class UImage* IcoCharacter;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleCard;
	
private:
	UPROPERTY()
	UHero* Hero;
};
