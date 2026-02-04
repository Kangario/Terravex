#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "Character/Hero.h"
#include "Components/Button.h"
#include "CharacterCardController.generated.h"

DECLARE_DELEGATE_OneParam(FOnCardPurchased, UCharacterCardController*);

UCLASS()
class TERRAVEX_API UCharacterCardController : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void SetHero(TSharedPtr<FJsonObject> JsonObject);
	UHero* GetHero();
	UFUNCTION()
	void SetIcoCharacter();
	UFUNCTION()
	void SetTitleCharacter();
	UFUNCTION()
	void SetDescriptionCharacterLvl();
	UFUNCTION()
	void SetHpCharacter();
	UFUNCTION()
	void SetDamageCharacter();
	UFUNCTION()
	void SetDefenseCharacter();
	UFUNCTION()
	void SetSpeedCharacter();
	UFUNCTION()
	void SetAttackSpeedCharacter();
	UFUNCTION()
	void SetCharacterXPValue();
	UFUNCTION()
	void SetCharacterPrice();
	UFUNCTION()
	void OnClickCardPay();
	UFUNCTION()
	void SetTextButton(FString buttonText);
	FOnCardPurchased OnCardPurchased;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	TMap<ETypeClass , UTexture2D*> CharacterIcons;
protected:
	bool bUIReady = false;
	bool bHeroReady = false;

	void UpdateUI();
	UPROPERTY(meta = (BindWidget))
	class UImage* IcoCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LVLChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefenseChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpeedChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AsChar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterXPValue;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterPrice;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleCardButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CardButton;
private:
	UPROPERTY()
	UHero* Hero;
};
