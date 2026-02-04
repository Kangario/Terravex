#pragma once

UENUM(BlueprintType)
enum class EMenuState : uint8
{
	MainMenu = 0 UMETA(DisplayName = "MainMenu"),
	SignInMenu = 1 UMETA(DisplayName = "SignInMenu"),
	SignUpMenu = 2 UMETA(DisplayName = "SignUpMenu"),
	UserProfileMenu = 3 UMETA(DisplayName = "UserProfileMenu"),
	ShopMenu = 4 UMETA(DisplayName = "ShopMenu"),
	BattleGroup = 5 UMETA(DisplayName = "BattleGroup"),
	SearchBattle = 6 UMETA(DisplayName = "SearchBattle"),
	GameHUD = 7 UMETA(DisplayName = "GameHUD"),
	WinWindow = 8 UMETA(DisplayName = "WinWindow"),
	LoseWindow = 9 UMETA(DisplayName = "LoseWindow"),
};
