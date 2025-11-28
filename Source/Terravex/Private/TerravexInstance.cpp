#include "TerravexInstance.h"

void UTerravexInstance::Init()
{
	Super::Init();
}

void UTerravexInstance::InitInterfaceStateController(AInterfaceState* InterfaceState)
{
	UE_LOG(LogTemp, Log, TEXT("This is a log message"));
	GoogleAuthManager = GetSubsystem<UGoogleAuthManager>();
	GoogleAuthManager->SetInterfaceController(InterfaceState);
}
