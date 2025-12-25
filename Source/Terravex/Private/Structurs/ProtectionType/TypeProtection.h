#pragma once
#include "TypeProtection.generated.h"

USTRUCT(BlueprintType)
struct FTypeProtection
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 Physical;
	UPROPERTY()
	int32 Magic;
	
	FTypeProtection():
	Physical(0),
	Magic(0)
	{}
	
	FTypeProtection(int32 Physical, int32 Magic)
	{
		this->Physical = Physical;
		this->Magic = Magic;
	}
};