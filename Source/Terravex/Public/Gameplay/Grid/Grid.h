// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BattleState/BattleState.h"
#include "GridCell/GridCell.h"
#include "ProceduralMeshComponent.h"
#include "Gameplay/TerrainGenerators/TerrainGenerator.h"
#include "Structurs/DeployData/DeployData.h"
#include "Grid.generated.h"

struct FGridCell;

UCLASS()
class TERRAVEX_API UGrid : public UObject
{
	GENERATED_BODY()
public:
	void Init(
	int32 Width,
	int32 Height,
	const TArray<int32>& HeightMap,
	const FVector& InOrigin,
	UProceduralMeshComponent* TerrainMesh
);
	FVector GridToWorld(int32 X, int32 Y, float ZOffset );
	FIntPoint WorldToGrid(const FVector& WorldLocation) const;
	int32 GetIndexGrid(int32 X, int32 Y) const;
	int32 GetIndexGrid(const FIntPoint& cell) const;
	FGridCell& GetCellByCoord(const FIntPoint& cell);
	FIntPoint GetCoordGrid(int32 Index);
	UProceduralMeshComponent* GetGridMesh();
	bool IsCellWalkable(const FIntPoint& Cell) const;
	bool IsValidCell(const FIntPoint& Cell) const;
	
	void ClearCells();
	void CellIllumination(const FDeployData& DeploymentData);
	
	void UpdateAllWalkableCells(bool canMove);
	float Normalize360(float Angle);
	bool TryMoveUnit(APlayerCharacter* Unit, const FIntPoint& TargetCell);
	void ForceMoveUnit(APlayerCharacter* Unit, const FIntPoint& TargetCell);
	
	
	UPROPERTY(EditAnywhere, Category="Deployment")
	TSubclassOf<AActor> HighlighterClass;

	UPROPERTY()
	AActor* HighlighterActor;
	UPROPERTY()
	UTerrainGenerator* TerrainGenerator;
	UPROPERTY()
	TArray<FGridCell> Grid;
	UPROPERTY()
	int32 GridWidth;
	UPROPERTY()
	int32 GridHeight;
	UPROPERTY()
	float CellSize;
	UPROPERTY()
	int32 Scale = 100;
	UPROPERTY()
	FVector Origin;
};
