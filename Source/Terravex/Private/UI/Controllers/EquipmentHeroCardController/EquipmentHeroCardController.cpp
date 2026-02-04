// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/EquipmentHeroCardController/EquipmentHeroCardController.h"


void UEquipmentHeroCardController::NativeConstruct()
{
	Super::NativeConstruct();

	bUIReady = true;
	UpdateUI();
	
}

void UEquipmentHeroCardController::SetHero(TSharedPtr<FJsonObject> JsonObject)
{
	Hero = NewObject<UHero>(this);
	Hero->LoadJsonHero(JsonObject);

	bHeroReady = true;
	UpdateUI();
}

UHero* UEquipmentHeroCardController::GetHero()
{
	return Hero;
}


void UEquipmentHeroCardController::UpdateUI()
{
	if (!bUIReady || !bHeroReady)
		return;

	SetIconCharacter();
	SetTitleCharacter();
}

void UEquipmentHeroCardController::SetIconCharacter()
{
	if (!Hero || !IcoCharacter)
		return;

	if (UTexture2D** FoundTexture = CharacterIcons.Find(Hero->GetClassHero()))
	{
		IcoCharacter->SetBrushFromTexture(*FoundTexture, true);
	}
}

void UEquipmentHeroCardController::SetTitleCharacter()
{
	TitleCard->SetText(FText::FromString(Hero->GetNameHero()));
}