#pragma once
#include "CoreMinimal.h"
#include "Enums/BattleEventType/BattleEventType.h"
#include "BattleEvent.generated.h"

USTRUCT()
struct FBattleEvent
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 EventIndex = 0;
	
	UPROPERTY()
	EBattleEventType Type;
	
	UPROPERTY()
	int32 UnitId = -1;
	
	UPROPERTY()
	int32 TargetUnitId = -1;
	
	UPROPERTY()
	int32 FromX = -1;

	UPROPERTY()
	int32 FromY = -1;

	UPROPERTY()
	int32 ToX = -1;

	UPROPERTY()
	int32 ToY = -1;

	UPROPERTY()
	int32 Value = 0;
	
	UPROPERTY()
	int32 Extra = 0;
};
