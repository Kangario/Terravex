#include "Gameplay/Grid/Grid.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/Character/PlayerCharacter.h"


void UGrid::Init(
	int32 Width,
	int32 Height,
	const TArray<int32>& HeightMap,
	const FVector& InOrigin,
	UProceduralMeshComponent* TerrainMesh
)
{
	GridWidth = Width;
	GridHeight = Height;
	CellSize = 100.f;
	Origin = InOrigin;
	Grid.SetNum(GridWidth * GridHeight);

	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			int32 Index = x + y * GridWidth;
			FGridCell& Cell = Grid[Index];
			Cell.X = x;
			Cell.Y = y;
			Cell.Z = HeightMap[Index];
			Cell.bWalkable = true;

		}
	}


	TerrainGenerator = NewObject<UTerrainGenerator>(this);

	TerrainGenerator->GenerateTerrain(
		GridWidth,
		GridHeight,
		HeightMap,
		CellSize,
		TerrainMesh
	);
}

UProceduralMeshComponent* UGrid::GetGridMesh()
{
	return TerrainGenerator->GetTerrainMesh();
}

FVector UGrid::GridToWorld(int32 X, int32 Y, float ZOffset) 
{
	const int32 Index = GetIndexGrid(X, Y);
	const float Z = Grid[Index].Z;

	return Origin + FVector(
		X * CellSize + CellSize ,
		Y * CellSize + CellSize,
		Z + ZOffset
	);
}

FIntPoint UGrid::WorldToGrid(const FVector& WorldLocation) const
{
	const FVector Local = WorldLocation - Origin;

	const int32 X = FMath::FloorToInt(Local.X / Scale);
	const int32 Y = FMath::FloorToInt(Local.Y / Scale);

	return FIntPoint(X, Y);
}


int32 UGrid::GetIndexGrid(int32 X, int32 Y) const
{
	return Y * GridWidth + X;
}

int32 UGrid::GetIndexGrid(const FIntPoint& cell) const
{
	return cell.Y * GridWidth + cell.X;
}

FGridCell& UGrid::GetCellByCoord(const FIntPoint& cell)
{
	int32 index = cell.Y * GridWidth + cell.X;
	check(Grid.IsValidIndex(index));
	return Grid[index];
}

FIntPoint UGrid::GetCoordGrid(int32 Index)
{
	int32 X = Index % GridWidth;
	int32 Y = Index / GridWidth;

	return FIntPoint(X, Y);
}

bool UGrid::IsValidCell(const FIntPoint& Cell) const
{
	return Cell.X >= 0 && Cell.X < GridWidth
		&& Cell.Y >= 0 && Cell.Y < GridHeight;
}

bool UGrid::IsCellWalkable(const FIntPoint& Cell) const
{
	if (!IsValidCell(Cell))
	{
		return false;
	}

	const int32 Index = GetIndexGrid(Cell);
	return Grid[Index].bWalkable;
}

void UGrid::UpdateAllWalkableCells(bool canMove)
{
	for (FGridCell& Cell : Grid)
	{
		Cell.bWalkable = canMove;
	}
	
}

void UGrid::ClearCells()
{
	for (FGridCell& Cell : Grid)
	{
		Cell.bDeploymentAllowed = false;
		Cell.bHighlighted = false;
	}
	if (IsValid(HighlighterActor))
	{
		HighlighterActor->SetActorHiddenInGame(true);
	}
}

void UGrid::CellIllumination(
	const FDeployData& DeploymentData
)
{
	UE_LOG(LogTemp, Log,
		TEXT("[Grid] CellIllumination start | Team=%s | Rows=%d"),
		*DeploymentData.Team,
		DeploymentData.AllowedRows.Num()
	);

	// 1️⃣ Сброс состояния клеток
	for (FGridCell& Cell : Grid)
	{
		Cell.bDeploymentAllowed = false;
	}

	// 2️⃣ Разрешаем клетки по AllowedRows
	for (int32 Row : DeploymentData.AllowedRows)
	{
		if (Row < 0 || Row >= GridHeight)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[Grid] Invalid row index: %d (GridHeight=%d)"),
				Row, GridHeight
			);
			continue;
		}

		for (int32 X = 0; X < GridWidth; ++X)
		{
			const int32 Index = GetIndexGrid(X, Row);

			if (Grid.IsValidIndex(Index))
			{
				Grid[Index].bDeploymentAllowed = true;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[Grid] Deployment rows applied"));
}

bool UGrid::TryMoveUnit(APlayerCharacter* Unit, const FIntPoint& TargetCell)
{
	if (!IsValidCell(TargetCell))
		return false;

	FGridCell& NewCell = GetCellByCoord(TargetCell);

	if (!NewCell.bWalkable)
		return false;

	if (!NewCell.characterId.IsEmpty())
		return false;

	// освобождаем старую клетку
	FGridCell& OldCell = GetCellByCoord(Unit->CurrentCellCoord);
	OldCell.characterId.Empty();

	// занимаем новую
	NewCell.characterId = Unit->UnitState.id;

	Unit->CurrentCellCoord = TargetCell;

	return true;
}

void UGrid::ForceMoveUnit(APlayerCharacter* Unit, const FIntPoint& TargetCell)
{
	if (!Unit)
		return;

	if (!IsValidCell(TargetCell))
		return;

	// освободить старую клетку
	FGridCell& OldCell = GetCellByCoord(Unit->CurrentCellCoord);
	OldCell.characterId.Empty();

	// занять новую
	FGridCell& NewCell = GetCellByCoord(TargetCell);
	NewCell.characterId = Unit->UnitState.id;

	Unit->CurrentCellCoord = TargetCell;
}