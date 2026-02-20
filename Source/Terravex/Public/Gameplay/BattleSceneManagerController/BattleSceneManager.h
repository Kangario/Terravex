#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Gameplay/BattleSceneController/BattleSceneController.h"
#include "Gameplay/BattleState/BattleState.h"
#include "Gameplay/BattleSocketClientController/BattleSocketClient.h"
#include "Gameplay/MatchSessionController/MatchSessionSubsystem.h"
#include "Enums/BattleConnectionStage/BattleConnectionState.h"
#include "Structurs/DeployData/DeployData.h"
#include "BattleSceneManager.generated.h"

class UBattleSocketClient;
class UMatchSessionSubsystem;

UCLASS()
class TERRAVEX_API UBattleSceneManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    //Constructors
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    
    // Lifecycle
    void StartBattle();
    void EndBattle();
    
    void RegisterSceneController(ABattleSceneController* Controller);
    
    // State access
    bool IsBattleReady() const { return State == EBattleConnectionState::InBattle; }
    const FBattleState& GetBattleState() const { return CurrentBattleState; }

    // Event fired when battle_init is received and parsed
    DECLARE_MULTICAST_DELEGATE(FOnEndDeploy);
    FOnEndDeploy OnEndDeploy;
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBattleReady,FString, const FBattleState&);
    FOnBattleReady OnBattleReady;
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitMove, FUserUnitPositionState);
    FOnUnitMove OnUnitMove;
    DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTurnStartDuration,int32,FBattleState, FString );
    FOnTurnStartDuration OnTurnStartDuration;
    DECLARE_MULTICAST_DELEGATE(FOnTurnEnd);
    FOnTurnEnd OnTurnEnd;
    
    DECLARE_MULTICAST_DELEGATE_FourParams(FOnApplyDamage, FString,FString,int32,bool);
    FOnApplyDamage OnApplyDamage;
    
    DECLARE_MULTICAST_DELEGATE_ThreeParams(
    FOnDeployReady,
    int32,
    TArray<FDeployData>,
    FBattleState& 
);
    FOnDeployReady OnDeployReady;
    
private:
    UPROPERTY()
    ABattleSceneController* SceneController;
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
    void SendMessage(TSharedPtr<FJsonObject> Json);
    void SendJoinRequest();
    void HandleJoinRetry();
    //Deploy
    void SendDeployRequest();
    void SendDeployReadyRequest( TArray<FString> unitId, TArray<TArray<int32>> unitPosition);
    void DeployEvent(TSharedPtr<FJsonObject> EventObj);
    //Event
    void HandleTurnStart(TSharedPtr<FJsonObject> Root);
    void HandleTurnEnd();
    void HandlerBattleEvents(TSharedPtr<FJsonObject> Root);
    void HandleMove(TSharedPtr<FJsonObject> Root);
    void SendMoveRequest(FString unitId, FIntPoint position);
    void SendAttackRequest(FString unitId, FString targetUnitId);
    void HandleApplyDamage(TSharedPtr<FJsonObject> Root);
    void HandleBattleInit(TSharedPtr<FJsonObject> Root);
    void HandleServerError(TSharedPtr<FJsonObject> Root);
};
