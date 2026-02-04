#include "Gameplay/Grid/Grid.h"


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
	CachedWorld = TerrainMesh->GetWorld();
	
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

void UGrid::CellIllumination(
	const FDeployData& DeploymentData,
	TSubclassOf<AActor> Highlighter,
	int32 teamId
)
{
	UE_LOG(LogTemp, Log,
		TEXT("[Grid] CellIllumination start | Team=%s | Rows=%d"),
		*DeploymentData.Team,
		DeploymentData.AllowedRows.Num()
	);

	HighlighterClass = Highlighter;

	if (!CachedWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grid] CachedWorld is NULL"));
		return;
	}

	if (!HighlighterClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Grid] HighlighterClass is NULL"));
	}

	// 1️⃣ Сброс состояния клеток
	for (FGridCell& Cell : Grid)
	{
		Cell.bDeploymentAllowed = false;
		Cell.bHighlighted = false;
	}

	// 2️⃣ Ограничиваем по рядам
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
			Grid[Index].bDeploymentAllowed = true;
			Grid[Index].bHighlighted = true;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[Grid] Cells illumination applied"));

	// 3️⃣ Highlighter
	if (!HighlighterActor)
	{
		UE_LOG(LogTemp, Log, TEXT("[Grid] HighlighterActor not found, spawning"));
		SpawnHighlighter();
	}

	// 4️⃣ Поворот по команде
	if (HighlighterActor)
	{
		const float Yaw = GetRotationByTeam(teamId);
		HighlighterActor->SetActorRotation(FRotator(0.f, Yaw, 0.f));

		UE_LOG(LogTemp, Log,
			TEXT("[Grid] Highlighter rotated | Yaw=%.1f"),
			Yaw
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Grid] HighlighterActor is NULL after spawn"));
	}
}



void UGrid::SpawnHighlighter()
{
	UE_LOG(LogTemp, Log, TEXT("[Grid] SpawnHighlighter called"));

	if (!CachedWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grid] Cannot spawn Highlighter: CachedWorld is NULL"));
		return;
	}

	if (!HighlighterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grid] Cannot spawn Highlighter: HighlighterClass is NULL"));
		return;
	}

	if (HighlighterActor)
	{
		UE_LOG(LogTemp, Log, TEXT("[Grid] Destroying existing HighlighterActor"));
		HighlighterActor->Destroy();
		HighlighterActor = nullptr;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Origin);
	SpawnTransform.SetRotation(FQuat::Identity);

	HighlighterActor = CachedWorld->SpawnActor<AActor>(
		HighlighterClass,
		SpawnTransform
	);

	if (!HighlighterActor)
	{
		UE_LOG(LogTemp, Error, TEXT("[Grid] Failed to spawn HighlighterActor"));
	}
	else
	{
		UE_LOG(LogTemp, Log,
			TEXT("[Grid] HighlighterActor spawned successfully at %s"),
			*Origin.ToString()
		);
	}
}


float UGrid::GetRotationByTeam(int32 teamId) const
{
	float Rotation = 0.f;

	if (teamId == 1)
	{
		Rotation = 180.f;
	}

	return Rotation;
}
