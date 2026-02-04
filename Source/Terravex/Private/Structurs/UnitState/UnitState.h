#pragma once

#include "CoreMinimal.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "Structurs/DamageType/TypeDamage.h"
#include "Structurs/ProtectionType/TypeProtection.h"
#include "UnitState.generated.h"

USTRUCT()
struct FUnitState
{
	GENERATED_BODY()
	UPROPERTY()
	int32 id;
	UPROPERTY()
	int32 team;
	UPROPERTY()
	FString templateId;
	UPROPERTY()
	FString ownerId;
	
	UPROPERTY()
	FString name;
	UPROPERTY()
	ETypeClass classHero;
	
	UPROPERTY()
	int32 hp;
	UPROPERTY()
	int32 maxHp;
	UPROPERTY()
	int32 ap;
	UPROPERTY()
	int32 initiative;
	UPROPERTY()
	int32 damageP;
	UPROPERTY()
	int32 damageM;
	UPROPERTY()
	int32 defenceP;
	UPROPERTY()
	int32 defenceM;
	UPROPERTY()
	int32 speed;
	UPROPERTY()
	int32 attackSpeed;
	UPROPERTY()
	int32 level;
	UPROPERTY()
	int32 x;
	UPROPERTY()
	int32 y;
};
