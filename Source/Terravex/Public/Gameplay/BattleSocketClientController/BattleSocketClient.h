// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h" 
#include "Json.h"
#include "JsonUtilities.h"
#include "BattleSocketClient.generated.h"

UCLASS()
class TERRAVEX_API UBattleSocketClient : public UObject
{
	GENERATED_BODY()
public:
	void Connect();
	void Close();
	void Send(const FString& Json);

	DECLARE_DELEGATE(FOnConnected);
	DECLARE_DELEGATE_OneParam(FOnMessage, const FString&);
	DECLARE_DELEGATE(FOnClosed);

	FOnConnected OnConnected;
	FOnMessage   OnMessage;
	FOnClosed    OnClosed;

private:
	TSharedPtr<IWebSocket> Socket;
};
