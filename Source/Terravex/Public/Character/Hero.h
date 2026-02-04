#pragma once

#include "CoreMinimal.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "Structurs/DamageType/TypeDamage.h"
#include "Interfaces/ISkill/ISkill.h"
#include "Interfaces/IEquipment/IEquipment.h"
#include "Structurs/ProtectionType/TypeProtection.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Hero.generated.h"

UCLASS()
class TERRAVEX_API UHero : public UObject
{
	GENERATED_BODY()
private:
	FString m_id;
	FString m_nameHero;
	ETypeClass m_heroClass;
	int32 m_health;
	FTypeDamage m_damage;
	FTypeProtection m_protection;
	int32 m_speed;
	int32 m_attackSpeed;
	int32 m_Level;
	int32 m_currentXP;
	int32 m_maxXP;
	int32 m_price;
	IEquipment* m_equipment;
	ISkill* m_skill;
	FString m_instanceId;
	
public:
	UHero();
	~UHero();
	virtual FString GetId();
	virtual FString GetNameHero();
	virtual ETypeClass GetClassHero();
	virtual int32 GetLevelHero();
	virtual int32 GetHealthHero();
	virtual FTypeDamage GetDamageHero();
	virtual FTypeProtection GetDefenceHero();
	virtual int32 GetSpeed();
	virtual int32 GetAttackSpeed();
	virtual int32 GetCurrentXP();
	virtual int32 GetPrice();
	virtual FString GetInstanceId();
	virtual void SetPrice(int32 price);
	virtual void SaveToJsonHero(FString id,
	FString nameHero,
	ETypeClass heroClass,
	int32 health,
	FTypeDamage damage,
	FTypeProtection protection,
	int32 speed,
	int32 attackSpeed,
	int32 level,
	int32 currentXP,
	int32 maxXP,
	FString instanceId);
	virtual  void LoadJsonHero(FString id);
	virtual void LoadJsonHero(TSharedPtr<FJsonObject> JsonObject);
};
