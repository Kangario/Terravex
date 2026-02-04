#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/InterfaceState.h"
#include "Components/ScrollBox.h"
#include "UI/Controllers/CharacterCardController/CharacterCardController.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "ShopMenuController.generated.h"

class UShopCardFactory;

UCLASS()
class TERRAVEX_API UShopMenuController : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	UFUNCTION()
	void BackToMainMenu();
	UFUNCTION()
	void UpdateShop();
	void OnShopUpdated(TSharedPtr<FJsonObject> ShopJson);
	UFUNCTION()
	void HandleCardPurchased(UCharacterCardController* Card);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AInterfaceState* InterfaceState;
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UUserWidget> CardWidgetClass;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollCards;
protected:
	UPROPERTY()
	UShopCardFactory* CardFactory;
	UPROPERTY()
	TArray<UCharacterCardController*> cardContainer;
	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonUpdateShop;
	UPROPERTY()
	int64 LastShopUpdate;
};
