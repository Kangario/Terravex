#pragma once
#include "DeployData.generated.h"

USTRUCT()
struct FDeployData
{
	GENERATED_BODY()
	UPROPERTY()
	FString Team;
	UPROPERTY()
	TArray<int32> AllowedRows;
	
};
