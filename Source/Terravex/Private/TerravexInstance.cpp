#include "TerravexInstance.h"
#include "AndroidPermissionFunctionLibrary.h"

void UTerravexInstance::Init()
{
	Super::Init();
	TArray<FString> Permissions;
	Permissions.Add("android.permission.MANAGE_EXTERNAL_STORAGE");
	UAndroidPermissionFunctionLibrary::AcquirePermissions(Permissions);
	
	if (UGameplayStatics::DoesSaveGameExist(TEXT("AuthSlot"), 0))
	{
		bHasSave = true;

		PlayerAuthData = Cast<USaveUserData>(
			UGameplayStatics::LoadGameFromSlot(TEXT("AuthSlot"), 0)
		);

		PlayerAuthData->bIsFirstLaunch = false;  
		
	}
	else
	{
		PlayerAuthData = Cast<USaveUserData>(
			UGameplayStatics::CreateSaveGameObject(USaveUserData::StaticClass())
		);
		PlayerAuthData->bIsFirstLaunch = true; 
	}
}

void UTerravexInstance::InitInterfaceStateController(AInterfaceState* InterfaceState)
{
	UE_LOG(LogTemp, Log, TEXT("This is a log message"));
	GoogleAuthManager = GetSubsystem<UGoogleAuthManager>();
	GoogleAuthManager->SetInterfaceController(InterfaceState);
	if (PlayerAuthData->bIsFirstLaunch == false)
	{
		InterfaceState->SwithInterface(EMenuState::MainMenu);
	}
}
