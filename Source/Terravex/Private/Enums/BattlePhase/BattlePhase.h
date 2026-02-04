#pragma once


UENUM(BlueprintType)
enum class EBattlePhase : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	//BattleStart
	Initializing = 1 UMETA(DisplayName = "Initializing"),
	Deploying = 2 UMETA(DisplayName = "Deploying"),
	//BattleRun
	TurnStart = 3 UMETA(DisplayName = "TurnStart"),
	PlayerPlanning = 4 UMETA(DisplayName = "PlayerPlanning"),
	WaitingServer = 5 UMETA(DisplayName = "WaitingServer"),
	Simulating = 6 UMETA(DisplayName = "Simulating"),
	TurnEnd = 7 UMETA(DisplayName = "TurnEnd"),
	//Service
	Paused = 8 UMETA(DisplayName = "Paused"),
	Reconnecting = 9 UMETA(DisplayName = "Reconnecting"),
	Syncing = 10 UMETA(DisplayName = "Syncing"),
	//Finished
	BattleFinished = 11 UMETA(DisplayName = "BattleFinished")
};
