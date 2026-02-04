// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controllers/CharacterCardController/CharacterCardController.h"


void UCharacterCardController::NativeConstruct()
{
	Super::NativeConstruct();

	bUIReady = true;
	UpdateUI();
	
	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &UCharacterCardController::OnClickCardPay);
	}
}

void UCharacterCardController::SetHero(TSharedPtr<FJsonObject> JsonObject)
{
	Hero = NewObject<UHero>(this);
	Hero->LoadJsonHero(JsonObject);

	bHeroReady = true;
	UpdateUI();
}

void UCharacterCardController::UpdateUI()
{
	if (!bUIReady || !bHeroReady)
		return;

	SetIcoCharacter();
	SetTitleCharacter();
	SetDescriptionCharacterLvl();
	SetHpCharacter();
	SetDamageCharacter();
	SetDefenseCharacter();
	SetSpeedCharacter();
	SetAttackSpeedCharacter();
	SetCharacterXPValue();
	SetCharacterPrice();
}

void UCharacterCardController::OnClickCardPay()
{
	OnCardPurchased.ExecuteIfBound(this);
	
	UE_LOG(LogTemp, Log, TEXT("Card clicked: %s"), *GetName());
}

void UCharacterCardController::SetTextButton(FString buttonText)
{
	if (buttonText.IsEmpty())
	{
		buttonText = "Play";
	}
	
	TitleCardButton->SetText(FText::FromString(buttonText));
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
	LVLChar->SetText(FText::FromString("Level:" + FString::FromInt(Hero->GetLevelHero())));
}

void UCharacterCardController::SetHpCharacter()
{
	
	HPChar->SetText(FText::FromString("Hp: " + FString::FromInt(Hero->GetHealthHero())));
}

void UCharacterCardController::SetDamageCharacter()
{
	DamageChar->SetText(FText::FromString("Damage: Physycal:" + FString::FromInt(Hero->GetDamageHero().Physical) + " Magical: " + FString::FromInt(Hero->GetDamageHero().Magic)));
}

void UCharacterCardController::SetDefenseCharacter()
{
	DefenseChar->SetText(FText::FromString("Defence: Physycal:" + FString::FromInt(Hero->GetDefenceHero().Physical) + " Magical: " + FString::FromInt(Hero->GetDefenceHero().Magic)));
}

void UCharacterCardController::SetSpeedCharacter()
{
	SpeedChar->SetText(FText::FromString("Speed:" + FString::FromInt(Hero->GetSpeed())));
}

void UCharacterCardController::SetAttackSpeedCharacter()
{
	AsChar->SetText(FText::FromString("Attack speed:" + FString::FromInt(Hero->GetAttackSpeed())));
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

UHero* UCharacterCardController::GetHero()
{
	return Hero;
}
