// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/CharacterCardController/CharacterCardController.h"




void UCharacterCardController::SetHero(TSharedPtr<FJsonObject> JsonObject)
{
	Hero = NewObject<UTHero>(this);
	Hero->LoadJsonHero(JsonObject);
	SetIcoCharacter();
	SetTitleCharacter();
	SetDescriptionCharacterLvl();
	SetStatCharacter();
	SetCharacterXPValue();
	SetCharacterPrice();
}

void UCharacterCardController::SetIcoCharacter()
{
	if (!Hero || !IcoCharacter)
		return;

	if (UTexture2D** FoundTexture = CharacterIcons.Find(Hero->GetClassHero()))
	{
		IcoCharacter->SetBrushFromTexture(*FoundTexture, true);
	}
}

void UCharacterCardController::SetTitleCharacter()
{
	TitleCharacter->SetText(FText::FromString(Hero->GetNameHero()));

}

void UCharacterCardController::SetDescriptionCharacterLvl()
{
	DescriptionCharacterLvl->SetText(FText::FromString("Level:" + FString::FromInt(Hero->GetLevelHero())));
}

void UCharacterCardController::SetStatCharacter()
{
	FString CharacterDamageTemp;
	FTypeDamage damageTypeTemp = Hero->GetDamageHero();
	int32 healhtTemp = Hero->GetHealthHero();
	CharacterDamageTemp =FString::Printf(
	TEXT("Health: %d, \nDamage: Physical: %d, Magical: %d"),
	healhtTemp,
	damageTypeTemp.Physical,
	damageTypeTemp.Magic);
	DamageCharacter->SetText(FText::FromString(CharacterDamageTemp));
}

void UCharacterCardController::SetCharacterXPValue()
{
	CharacterXPValue->SetText(FText::AsNumber(Hero->GetCurrentXP()));
}

void UCharacterCardController::SetCharacterPrice()
{
	int32 price = FMath::RandRange(0, 100);
	Hero->SetPrice(price);
	CharacterPrice->SetText(FText::AsNumber(price));
}