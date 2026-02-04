// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/BattleSocketClientController/BattleSocketClient.h"


void UBattleSocketClient::Connect()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	Socket = FWebSocketsModule::Get().CreateWebSocket(
		TEXT("wss://terravexgameserver.onrender.com")
	);
	
	TWeakObjectPtr<UBattleSocketClient> WeakThis(this);
	
	Socket->OnConnected().AddLambda([WeakThis]()
  {
	  AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	  {
		  if (!WeakThis.IsValid()) return;

		  UE_LOG(LogTemp, Log, TEXT("[BattleSocket] Connected"));
		  WeakThis->OnConnected.ExecuteIfBound();
	  });
  });

	Socket->OnMessage().AddLambda([WeakThis](const FString& Message)
   {
	   AsyncTask(ENamedThreads::GameThread, [WeakThis, Message]()
	   {
		   if (!WeakThis.IsValid()) return;

		   UE_LOG(LogTemp, Log, TEXT("[BattleSocket] << %s"), *Message);
		   WeakThis->OnMessage.ExecuteIfBound(Message);
	   });
   });

	Socket->OnClosed().AddLambda([WeakThis](int32 StatusCode, const FString& Reason, bool bWasClean)
   {
	   AsyncTask(ENamedThreads::GameThread, [WeakThis, StatusCode, Reason, bWasClean]()
	   {
		   if (!WeakThis.IsValid()) return;

		   UE_LOG(LogTemp, Warning,
			   TEXT("[BattleSocket] Closed (Code=%d Clean=%d Reason=%s)"),
			   StatusCode, bWasClean ? 1 : 0, *Reason);

		   WeakThis->OnClosed.ExecuteIfBound();
	   });
   });
	
	Socket->OnConnectionError().AddLambda([WeakThis](const FString& Error)
   {
	   AsyncTask(ENamedThreads::GameThread, [WeakThis, Error]()
	   {
		   if (!WeakThis.IsValid()) return;

		   UE_LOG(LogTemp, Error, TEXT("[BattleSocket] Connection error: %s"), *Error);
		   WeakThis->OnClosed.ExecuteIfBound();
	   });
   });

	Socket->Connect();
}

void UBattleSocketClient::Close()
{
	if (Socket.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[BattleSocketClient] Closing socket"));
		Socket->Close();
		Socket.Reset();
	}
}

void UBattleSocketClient::Send(const FString& Json)
{
	if (!Socket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleSocket] Send failed: Socket invalid"));
		return;
	}

	if (!Socket->IsConnected())
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleSocket] Send failed: Not connected"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleSocket] >> %s"), *Json);
	Socket->Send(Json);
}
