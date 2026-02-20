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
	UPROPERTY(VisibleAnywhere)
	FString id;
	UPROPERTY(VisibleAnywhere)
	int32 team;
	UPROPERTY(VisibleAnywhere)
	FString templateId;
	UPROPERTY(VisibleAnywhere)
	FString ownerId;
	
	UPROPERTY(VisibleAnywhere)
	FString name;
	UPROPERTY(VisibleAnywhere)
	ETypeClass classHero;
	
	UPROPERTY(VisibleAnywhere)
	int32 hp;
	UPROPERTY(VisibleAnywhere)
	int32 maxHp;
	UPROPERTY(VisibleAnywhere)
	int32 ap;
	UPROPERTY(VisibleAnywhere)
	int32 initiative;
	UPROPERTY(VisibleAnywhere)
	int32 damageP;
	UPROPERTY(VisibleAnywhere)
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
	UPROPERTY(VisibleAnywhere)
	int32 x;
	UPROPERTY(VisibleAnywhere)
	int32 y;
};
