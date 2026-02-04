// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateUser/CreateUserSubsystem.h"
#include "UI/Saves/SaveUserData.h"
#include "Kismet/GameplayStatics.h"
#include "Json.h"
#include "UI/InterfaceState.h"

#define SAVE_SLOT TEXT("UserData")

void UCreateUserSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    GI = Cast<UTerravexInstance>(GetGameInstance());
}

void UCreateUserSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

bool UCreateUserSubsystem::HasUser() const
{
   
    return !GI->UserId.IsEmpty();
}


void UCreateUserSubsystem::LoadUserFromSave()
{
    
    if (!UGameplayStatics::DoesSaveGameExist(SAVE_SLOT + GI->UserId, 0))
        return;

    USaveUserData* Save = Cast<USaveUserData>(
        UGameplayStatics::LoadGameFromSlot(SAVE_SLOT + GI->UserId, 0)
    );

    if (Save)
    {
        GI->UserId = Save->UserId;
        UE_LOG(LogTemp, Log, TEXT("[AuthSubsystem] Loaded UserId: %s"), *GI->UserId);
    }
}

void UCreateUserSubsystem::SaveUserToSave()
{
    USaveUserData* Save = Cast<USaveUserData>(
        UGameplayStatics::CreateSaveGameObject(USaveUserData::StaticClass())
    );

    Save->UserId = GI->UserId;
    UGameplayStatics::SaveGameToSlot(Save, SAVE_SLOT + GI->UserId, 0);

    UE_LOG(LogTemp, Log, TEXT("[AuthSubsystem] Saved UserId: %s"), *GI->UserId);
    SwithInterface(EMenuState::MainMenu);
}

void UCreateUserSubsystem::SwithInterface(EMenuState interface)
{
    for (TActorIterator<AInterfaceState> It(GetWorld()); It; ++It)
    {
        AInterfaceState* interfaceState = *It;
       interfaceState->SwithInterface(interface);
        break;
    }
}

void UCreateUserSubsystem::CreateUser()
{
    if (HasUser())
    {
        UE_LOG(LogTemp, Warning, TEXT("[AuthSubsystem] User already exists"));
        return;
    }

    TSharedRef<IHttpRequest> Request =
        FHttpModule::Get().CreateRequest();

    Request->SetURL(TEXT("https://terravex-server.onrender.com/user/create"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(TEXT("{}"));

    Request->OnProcessRequestComplete().BindUObject(
        this,
        &UCreateUserSubsystem::OnCreateUserResponse
    );

    Request->ProcessRequest();
}

void UCreateUserSubsystem::OnCreateUserResponse(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful
)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[AuthSubsystem] HTTP failed"));
        return;
    }

    TSharedPtr<FJsonObject> Json;
    TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (!FJsonSerializer::Deserialize(Reader, Json))
    {
        UE_LOG(LogTemp, Error, TEXT("[AuthSubsystem] JSON parse error"));
        return;
    }

    const TSharedPtr<FJsonObject>* UserObj;
    if (!Json->TryGetObjectField(TEXT("user"), UserObj))
        return;

    if (!(*UserObj)->TryGetStringField(TEXT("userId"), GI->UserId))
        return;

    SaveUserToSave();
}
