// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Saves/SaveUserData.h"

#include "Kismet/GameplayStatics.h"

void USaveUserData::SetUserData(FTUserData userData)
{
	UserData = userData;
}

void USaveUserData::SaveUserData()
{
	UGameplayStatics::SaveGameToSlot(this, "AuthSlot", 0);
}
