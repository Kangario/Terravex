#pragma once
#include "CoreMinimal.h"
#include "Interfaces/ISkill/ISkill.h"
#include "UObject/Interface.h"
#include "USkill.generated.h"

UCLASS(Abstract)
class UTSkill : public UObject, public ISkill
{
	GENERATED_BODY()
public:
	
};
