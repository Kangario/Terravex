#pragma once
#include "GridCell.generated.h"

class APlayerCharacter;

USTRUCT()
struct FGridCell
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Y;

	UPROPERTY()
	int32 Z;

	UPROPERTY()
	bool bWalkable = true;
	UPROPERTY()
	bool bDeploymentAllowed = false; 
	UPROPERTY()
	bool bHighlighted = false;
	
	UPROPERTY()
	FString characterId;
	
};
