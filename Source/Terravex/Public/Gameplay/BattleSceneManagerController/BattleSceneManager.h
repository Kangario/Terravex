#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/BattleState/BattleState.h"
#include "Gameplay/BattleSocketClientController/BattleSocketClient.h"
#include "Gameplay/MatchSessionController/MatchSessionSubsystem.h"
#include "Enums/BattleConnectionStage/BattleConnectionState.h"
#include "Structurs/DeployData/DeployData.h"
#include "BattleSceneManager.generated.h"

class UBattleSocketClient;
class UMatchSessionSubsystem;

UCLASS()
class TERRAVEX_API UBattleSceneManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    //Constructors
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    
    // Lifecycle
    void StartBattle();
    void EndBattle();

    // State access
    bool IsBattleReady() const { return State == EBattleConnectionState::InBattle; }
    const FBattleState& GetBattleState() const { return CurrentBattleState; }

    // Event fired when battle_init is received and parsed
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleReady, const FBattleState&);
    FOnBattleReady OnBattleReady;
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(
    FOnDeployReady,
    int32,
    TArray<FDeployData>
);
    FOnDeployReady OnDeployReady;
    
private:

    EBattleConnectionState State = EBattleConnectionState::Idle;

    // Networking
    UPROPERTY()
    UBattleSocketClient* SocketClient = nullptr;

    // Session data
    FString LocalUserID;
    FString MatchID;

    // Battle data
    FBattleState CurrentBattleState;

    // Retry logic
    int32 JoinRetryCount = 0;
    static constexpr int32 MaxJoinRetries = 20;

    FTimerHandle JoinRetryTimer;

private:
    // Internal helpers
    bool InitializeSessionData();
    void CreateSocket();

    // Socket callbacks
    void OnSocketConnected();
    void OnSocketMessage(const FString& Message);
    void OnSocketClosed();

    // Protocol handlers
    void SendJoinRequest();
    void HandleJoinRetry();
    void HandlerBattleDeployment(TSharedPtr<FJsonObject> Root);
    void HandleBattleInit(TSharedPtr<FJsonObject> Root);
    void HandleServerError(TSharedPtr<FJsonObject> Root);
};
