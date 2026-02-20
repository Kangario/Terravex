#pragma once
#include "Gameplay/Grid/GridCell/GridCell.h"
#include "UnitPositionState.generated.h"

USTRUCT()
struct FUnitPositionState
{
	GENERATED_BODY()
	FString unitId;
	FGridCell cell;
};
