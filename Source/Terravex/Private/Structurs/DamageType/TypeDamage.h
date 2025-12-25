#pragma once
#include "TypeDamage.generated.h"

USTRUCT(BlueprintType)
struct FTypeDamage
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 Physical;
	UPROPERTY()
	int32 Magic;
	
	FTypeDamage():
	Physical(0),
	Magic(0)
	{}
	
	FTypeDamage(int32 Physical, int32 Magic)
	{
		this->Physical = Physical;
		this->Magic = Magic;
	}
};