// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/TerrainType/TerrainType.h"
#include "Enums/BattlePhase/BattlePhase.h"
#include "Structurs/UserState/UserState.h"
#include "Structurs/UnitState/UnitState.h"
#include "Structurs/TerrainState/TerrainState.h"
#include "BattleState.generated.h"

USTRUCT()
struct TERRAVEX_API FBattleState
{
	GENERATED_BODY()
public:
	//World state
	UPROPERTY()
	FString MatchId;
	//TerrainStates
	UPROPERTY()
	int32 teamId;
	
	UPROPERTY()
	FTerrainState terrain;
	
	UPROPERTY()
	int32 turnNumber;
	UPROPERTY()
	int64  activeUnitId;
	//Characters
	UPROPERTY()
	TArray<FUnitState> Units;
	
	UPROPERTY()
	TArray<int32> InitiativeOrder;
	
	UPROPERTY()
	bool finished;
	UPROPERTY()
	int32 winnerTeam;
};
