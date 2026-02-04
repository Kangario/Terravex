#include "TerravexInstance.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "GoogleAuth/GoogleAuthManager.h"
#include "UI/Controllers/ShopMenuController/ShopMenuController.h"
#include "Misc/CoreDelegates.h"

#define SAVE_SLOT TEXT("UserData")

void UTerravexInstance::Init()
{
	Super::Init();
	TArray<FString> Permissions;
	Permissions.Add("android.permission.MANAGE_EXTERNAL_STORAGE");
	UAndroidPermissionFunctionLibrary::AcquirePermissions(Permissions);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(
	this,
	&UTerravexInstance::OnDeepLink
);
}

void UTerravexInstance::OnDeepLink()
{
	GetSubsystem<UGoogleAuthManager>()->CheckAuthResult();
}