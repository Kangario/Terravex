// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserState.generated.h"


USTRUCT()
struct FUserState
{
	GENERATED_BODY()
	UPROPERTY()
	FString PlayerId;
	UPROPERTY()
	int32 TeamId;
	UPROPERTY()
	bool Connected;
};
