#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GoogleAuth/GoogleAuthManager.h"
#include "SignUp/SignUp.h"
#include "SignIn/SignIn.h"
#include "Structurs/NetworkStructurs/UserData.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/Saves/SaveUserData.h"
#include "TerravexInstance.generated.h"



UCLASS()
class TERRAVEX_API UTerravexInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Init();
	UPROPERTY()
	UGoogleAuthManager* GoogleAuthManager;
	UFUNCTION()
	void InitInterfaceStateController(AInterfaceState* InterfaceState);
	UFUNCTION()
	void CheckUpdateShop();
	void UpdateLastShopUpdate(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	UPROPERTY()
	USaveUserData* PlayerAuthData;
	bool bHasSave = false;
	UPROPERTY(VisibleAnywhere, Category = "Shop")
	int64 LastShopUpdate = 0;
	UPROPERTY()
	bool canUpdate = true;
};
