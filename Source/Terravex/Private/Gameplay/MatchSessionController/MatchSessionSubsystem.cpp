// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/MatchSessionController/MatchSessionSubsystem.h"

#include "Gameplay/BattleSceneManagerController/BattleSceneManager.h"

void UMatchSessionSubsystem::SetSession(FString InUserID, FString InMatchID)
{
	UE_LOG(LogHAL, Log, TEXT("[UMatchSessionSubsystem] USERID: %s | Match ID: %s." ), *InUserID, *InMatchID);
	UserID = InUserID;
	MatchId = InMatchID;
}

void UMatchSessionSubsystem::RemoveSession(FString InUserID)
{
	
}

FString UMatchSessionSubsystem::GetMatchId()
{
	return MatchId;
}

FString UMatchSessionSubsystem::GetUserId()
{
	return UserID;
}