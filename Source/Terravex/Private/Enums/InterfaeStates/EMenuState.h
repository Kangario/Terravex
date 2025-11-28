#pragma once

UENUM(BlueprintType)
enum class EMenuState : uint8
{
	MainMenu = 0 UMETA(DisplayName = "MainMenu"),
	SignInMenu = 1 UMETA(DisplayName = "SignInMenu"),
	SignUpMenu = 2 UMETA(DisplayName = "SignUpMenu"),
	UserProfileMenu = 3 UMETA(DisplayName = "UserProfileMenu")
};
