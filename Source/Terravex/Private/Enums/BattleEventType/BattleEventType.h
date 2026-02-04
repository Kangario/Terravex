#pragma once

UENUM(BlueprintType)
enum class EBattleEventType : uint8
{
	Move = 0 UMETA(DisplayName = "Move"),
	Attack = 1 UMETA(DisplayName = "Attack"),
	Damage = 2 UMETA(DisplayName = "Damage"),
	Heal = 3 UMETA(DisplayName = "Heal"),
	TurnStart = 4 UMETA(DisplayName = "TurnStart"),
	TurnEnd = 5 UMETA(DisplayName = "TurnEnd"),
	Death = 6 UMETA(DisplayName = "Death")
};
