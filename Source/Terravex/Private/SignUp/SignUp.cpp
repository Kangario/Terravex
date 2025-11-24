#include "SignUp/SignUp.h"
#include "JsonUtilities.h"
#include "Json.h"

void USignUp::RegisterUser(FString Username, FString Password)
{
	FString Content = FString::Printf(TEXT("{\"username\":\"%s\",\"password\":\"%s\"}"), *Username, *Password);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("https://serverterravex.onrender.com/register"));
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(Content);

	Request->OnProcessRequestComplete().BindUObject(this, &USignUp::OnRegisterResponse);
	Request->ProcessRequest();
}

void USignUp::OnRegisterResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SignUp] HTTP Request failed"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[SignUp] Response: %s"), *Response->GetContentAsString());
}