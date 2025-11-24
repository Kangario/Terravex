#include "SignIn/SignIn.h"
#include "JsonUtilities.h"
#include "Json.h"

void USignIn::LoginUser(FString Username, FString Password)
{
	FString Content = FString::Printf(TEXT("{\"username\":\"%s\",\"password\":\"%s\"}"), *Username, *Password);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("https://serverterravex.onrender.com/login"));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(Content);

	Request->OnProcessRequestComplete().BindUObject(this, &USignIn::OnLoginResponse);
	Request->ProcessRequest();
}

void USignIn::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SignIn] HTTP Request failed"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[SignIn] Response: %s"), *Response->GetContentAsString());
}