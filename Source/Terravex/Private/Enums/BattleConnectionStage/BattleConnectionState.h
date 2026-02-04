#pragma once

UENUM(BlueprintType)
enum class EBattleConnectionState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Connecting = 1 UMETA(DisplayName = "Connecting"),
	InBattle = 2 UMETA(DisplayName = "InBattle")
};