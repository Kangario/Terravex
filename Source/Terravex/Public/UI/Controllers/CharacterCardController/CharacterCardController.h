// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "AbstractClasses/BaseHero/UHero.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "Character/Hero.h"
#include "CharacterCardController.generated.h"

/**
 * 
 */
UCLASS()
class TERRAVEX_API UCharacterCardController : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHero(TSharedPtr<FJsonObject> JsonObject);
	UFUNCTION()
	void SetIcoCharacter();
	UFUNCTION()
	void SetTitleCharacter();
	UFUNCTION()
	void SetDescriptionCharacterLvl();
	UFUNCTION()
	void SetStatCharacter();
	UFUNCTION()
	void SetCharacterXPValue();
	UFUNCTION()
	void SetCharacterPrice();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	TMap<ETypeClass , UTexture2D*> CharacterIcons;
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* IcoCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionCharacterLvl;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterXPValue;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterPrice;
private:
	UPROPERTY()
	UTHero* Hero;
};
