#pragma once

UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	Grass = 0 UMETA(DisplayName = "Grass"),
	Fiery = 1 UMETA(DisplayName = "Fiery"),
	Icy = 2 UMETA(DisplayName = "Icy")
};
