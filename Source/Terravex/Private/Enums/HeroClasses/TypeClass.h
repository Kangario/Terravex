#pragma once

UENUM(BlueprintType)
enum class ETypeClass : uint8
{
	Peasant = 0 UMETA(DisplayName = "Peasant"),
	Guard = 1 UMETA(DisplayName = "Guard"),
	Archer = 2 UMETA(DisplayName = "Archer"),
	Mage = 3 UMETA(DisplayName = "Mage")
};
