#include "Shop/ShopShowing/ShopCardFactory.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Shop/PurchaseHero/HeroPurchaseSubsystem.h"

void UShopCardFactory::Init(UWorld* InWorld)
{
	World = InWorld;
}

void UShopCardFactory::BuildCardsInSlots(
	const TSharedPtr<FJsonObject>& Json,
	const FString& ArrayFieldName,
	UUserWidget* OwnerWidget,
	const TArray<UPanelWidget*>& Slots,
	TSubclassOf<UUserWidget> CardWidgetClass,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)> InitCardFunc
)
{
	if (!Json.IsValid() || !OwnerWidget || !*CardWidgetClass)
		return;

	const TArray<TSharedPtr<FJsonValue>>* ItemsArray;
	if (!Json->TryGetArrayField(ArrayFieldName, ItemsArray))
		return;

	const int32 Count = FMath::Min(ItemsArray->Num(), Slots.Num());

	for (int32 i = 0; i < Count; i++)
	{
		UPanelWidget* Slot = Slots[i];
		if (!Slot)
			continue;

		Slot->ClearChildren();

		const TSharedPtr<FJsonObject> ItemObj = (*ItemsArray)[i]->AsObject();
		if (!ItemObj.IsValid())
			continue;

		UUserWidget* Card = CreateWidget<UUserWidget>(
			OwnerWidget,
			CardWidgetClass
		);

		if (!Card)
			continue;

		if (InitCardFunc)
		{
			InitCardFunc(Card, ItemObj);
		}
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot->AddChild(Card));

		if (CanvasSlot)
		{
			CanvasSlot->SetAutoSize(false);
			CanvasSlot->SetSize(FVector2D(275.f, 275.f));
			CanvasSlot->SetAlignment(FVector2D(0, 0));
		}
	}
}

void UShopCardFactory::BuildCards(
	const TSharedPtr<FJsonObject>& Json,
	const FString& ArrayFieldName,
	UUserWidget* OwnerWidget,
	UPanelWidget* Container,
	TSubclassOf<UUserWidget> CardWidgetClass,
	int32 CardsPerRow,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)> InitCardFunc
)
{
	if (!Json.IsValid() || !OwnerWidget || !Container || !*CardWidgetClass)
		return;

	const TArray<TSharedPtr<FJsonValue>>* ItemsArray;
	if (!Json->TryGetArrayField(ArrayFieldName, ItemsArray))
		return;

	UHorizontalBox* CurrentRow = nullptr;
	int32 CardIndex = 0;

	for (const TSharedPtr<FJsonValue>& Value : *ItemsArray)
	{
		const TSharedPtr<FJsonObject> ItemObj = Value->AsObject();
		if (!ItemObj.IsValid())
			continue;

		if (CardIndex % CardsPerRow == 0)
		{
			CurrentRow = NewObject<UHorizontalBox>(OwnerWidget);
			Container->AddChild(CurrentRow);
		}

		MakeGenericCard(
			ItemObj,
			OwnerWidget,
			CurrentRow,
			CardWidgetClass,
			InitCardFunc
		);

		CardIndex++;
	}
}



void UShopCardFactory::MakeGenericCard(
	const TSharedPtr<FJsonObject>& ItemData,
	UUserWidget* OwnerWidget,
	UHorizontalBox* Row,
	TSubclassOf<UUserWidget> CardWidgetClass,
	TFunction<void(UUserWidget*, const TSharedPtr<FJsonObject>&)> InitCardFunc
)
{
	if (!ItemData.IsValid() || !OwnerWidget || !Row || !*CardWidgetClass)
		return;

	UUserWidget* Card = CreateWidget<UUserWidget>(
		OwnerWidget,
		CardWidgetClass
	);
	if (!Card)
		return;

	// Внешняя инициализация карточки
	if (InitCardFunc)
	{
		InitCardFunc(Card, ItemData);
	}

	if (auto* Slot = Cast<UHorizontalBoxSlot>(Row->AddChild(Card)))
	{
		Slot->SetPadding(FMargin(40.f, 0.f, 40.f, 25.f));
		Slot->SetHorizontalAlignment(HAlign_Center);
	}
}
