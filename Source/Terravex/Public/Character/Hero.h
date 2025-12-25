#pragma once

#include "CoreMinimal.h"
#include "Enums/HeroClasses/TypeClass.h"
#include "AbstractClasses/BaseHero/UHero.h"
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
class TERRAVEX_API UTHero : public UHero
{
	GENERATED_BODY()
private:
	int32 m_id;
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
	
public:
	UTHero();
	~UTHero();
	virtual FString GetNameHero() override;
	virtual ETypeClass GetClassHero() override;
	virtual int32 GetLevelHero() override;
	virtual int32 GetHealthHero() override;
	virtual FTypeDamage GetDamageHero() override;
	virtual int32 GetCurrentXP() override;
	virtual int32 GetPrice() override;
	virtual void SetPrice(int32 price) override;
	virtual void SaveToJsonHero(int32 id,
	FString nameHero,
	ETypeClass heroClass,
	int32 health,
	FTypeDamage damage,
	FTypeProtection protection,
	int32 speed,
	int32 attackSpeed,
	int32 level,
	int32 currentXP,
	int32 maxXP) override;
	virtual  void LoadJsonHero(int32 id) override;
	virtual void LoadJsonHero(TSharedPtr<FJsonObject> JsonObject) override;
};
