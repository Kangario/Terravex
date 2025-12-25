#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/InterfaceState.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Saves/SaveUserData.h"
#include "Components/ScrollBox.h"
#include "UI/Controllers/CharacterCardController/CharacterCardController.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "ShopMenuController.generated.h"


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
	UFUNCTION()
	void RequestUpdateShop();
	void AddHeroToScrollView(TSharedPtr<FJsonObject> JsonObject);
	void MakeCharacter(TSharedPtr<IHttpRequest> request, TSharedPtr<IHttpResponse> response, bool bWasSucceful);
public:	
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfCards = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AInterfaceState* InterfaceState;
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UUserWidget> CardWidgetClass;
protected:
	UPROPERTY()
	TArray<UCharacterCardController*> cardContainer;
	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollCards;
	UPROPERTY()
	int64 LastShopUpdate;
};
