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
	FIntPoint GetCoordGrid(int32 Index);
	bool IsCellWalkable(const FIntPoint& Cell) const;
	bool IsValidCell(const FIntPoint& Cell) const;
	
	void CellIllumination(const FDeployData& DeploymentData,TSubclassOf<AActor> higliter, int32 teamId);
	void SpawnHighlighter();
	float GetRotationByTeam(int32 teamId) const;
	
	
	UPROPERTY(EditAnywhere, Category="Deployment")
	TSubclassOf<AActor> HighlighterClass;

	UPROPERTY()
	AActor* HighlighterActor;
	UPROPERTY()
	UWorld* CachedWorld;
	UPROPERTY()
	UTerrainGenerator* TerrainGenerator;
	
	TArray<FGridCell> Grid;
	int32 GridWidth;
	int32 GridHeight;
	float CellSize;
	int32 Scale = 100;
	FVector Origin;
};
