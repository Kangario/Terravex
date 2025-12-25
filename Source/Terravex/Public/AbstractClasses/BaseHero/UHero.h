#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "Structurs/DamageType/TypeDamage.h"
#include "Structurs/ProtectionType/TypeProtection.h"
#include "UHero.generated.h"

UCLASS(Abstract)
class  UHero : public UObject
{
	GENERATED_BODY()
public:
	virtual FString GetNameHero();
	virtual ETypeClass GetClassHero();
	virtual int32 GetLevelHero();
	virtual int32 GetHealthHero();
	virtual FTypeDamage GetDamageHero();
	virtual int32 GetCurrentXP();
	virtual void GetSkills();
	virtual int32 GetPrice();
	virtual void SetPrice(int32 price);
	virtual void SaveToJsonHero(int32 id,
	FString nameHero,
	ETypeClass heroClass,
	int32 health,
	FTypeDamage damage,
	FTypeProtection protection,
	int32 speed,
	int32 attackSpeed,
	int32 level,
	int32 currentXP,
	int32 maxXP);
	virtual void LoadJsonHero(int32 id);
	virtual void LoadJsonHero(TSharedPtr<FJsonObject> JsonObject);
};

