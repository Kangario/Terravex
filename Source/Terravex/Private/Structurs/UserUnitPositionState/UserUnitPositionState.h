#pragma once
#include "Structurs/UnitPositionState/UnitPositionState.h"
#include "UserUnitPositionState.generated.h"

USTRUCT()
struct FUserUnitPositionState
{
	GENERATED_BODY()
	FString userId;
	TArray<FUnitPositionState> units; 
};
