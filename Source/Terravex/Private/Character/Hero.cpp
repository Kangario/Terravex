// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero.h"

#include <string>

UHero::UHero()
{
}

UHero::~UHero()
{
}
void UHero::SaveToJsonHero(FString id,
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
	FString instanceId)
{
	TSharedPtr<FJsonObject> HeroObject = MakeShared<FJsonObject>();
	
	HeroObject->SetStringField("Id", id);
	HeroObject->SetStringField("Name", nameHero);
	HeroObject->SetNumberField("Class", static_cast<uint8>(heroClass));
	HeroObject->SetNumberField("Health", health);
	HeroObject->SetNumberField("DamagePhysical", damage.Physical);
	HeroObject->SetNumberField("DamageMagic", damage.Magic);
	HeroObject->SetNumberField("ProtectionPhysical", protection.Physical);
	HeroObject->SetNumberField("ProtectionMagic", protection.Magic);
	HeroObject->SetNumberField("Speed", speed);
	HeroObject->SetNumberField("AttackSpeed", attackSpeed);
	HeroObject->SetNumberField("Level", level);
	HeroObject->SetNumberField("CurrentXP", currentXP);
	HeroObject->SetNumberField("MaxXP", maxXP);
	HeroObject->SetStringField("InstanceId", instanceId);
	FString outputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(HeroObject.ToSharedRef(), Writer);
	const FString FilePath = FPaths::ProjectSavedDir() +TEXT("Heros/")+ id + TEXT(".json");
	
	FFileHelper::SaveStringToFile(outputString, *FilePath);
}

void UHero::LoadJsonHero(FString id)
{
	const FString FilePath = FPaths::ProjectSavedDir() +TEXT("Heros/")+ id + TEXT(".json");

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file"));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid JSON"));
		return;
	}

	m_id = JsonObject->GetStringField(TEXT("Id"));
	m_nameHero = JsonObject->GetStringField(TEXT("Name"));
	m_heroClass = static_cast<ETypeClass>(JsonObject->GetNumberField(TEXT("Class")));
	m_health = JsonObject->GetNumberField(TEXT("Health"));
	m_damage.Physical = JsonObject->GetNumberField(TEXT("DamagePhysical"));
	m_damage.Magic = JsonObject->GetNumberField(TEXT("DamageMagic"));
	m_protection.Physical = JsonObject->GetNumberField(TEXT("ProtectionPhysical"));
	m_protection.Magic = JsonObject->GetNumberField(TEXT("ProtectionMagic"));
	m_speed = JsonObject->GetNumberField(TEXT("Speed"));
	m_attackSpeed = JsonObject->GetNumberField(TEXT("AttackSpeed"));
	m_Level = JsonObject->GetNumberField(TEXT("Level"));
	m_currentXP = JsonObject->GetNumberField(TEXT("CurrentXP"));
	m_maxXP = JsonObject->GetNumberField(TEXT("MaxXP"));
	m_instanceId = JsonObject->GetStringField(TEXT("InstanceId"));
	UE_LOG(LogTemp, Log, TEXT("ID: %s | Loaded: %s | Level: %d | HP: %d"), *m_id , *m_nameHero, m_Level, m_health);
}

void UHero::LoadJsonHero(TSharedPtr<FJsonObject> JsonObject)
{//"Name":"Hero_0","TypeClass":2,"Hp":136,"DamageP":29,"DamageM":17,"DefenceP":6,"DefenceM":32,"Speed":95,"AttackSpeed":35,"Lvl":1,"Xp":0}
	m_id = JsonObject->GetStringField(TEXT("Id"));
	m_nameHero = JsonObject->GetStringField(TEXT("Name"));
	m_heroClass = static_cast<ETypeClass>(JsonObject->GetNumberField(TEXT("TypeClass")));
	m_health = JsonObject->GetNumberField(TEXT("Hp"));
	m_damage.Physical = JsonObject->GetNumberField(TEXT("DamageP"));
	m_damage.Magic = JsonObject->GetNumberField(TEXT("DamageM"));
	m_protection.Physical = JsonObject->GetNumberField(TEXT("DefenceP"));
	m_protection.Magic = JsonObject->GetNumberField(TEXT("DefenceM"));
	m_speed = JsonObject->GetNumberField(TEXT("Speed"));
	m_attackSpeed = JsonObject->GetNumberField(TEXT("AttackSpeed"));
	m_Level = JsonObject->GetNumberField(TEXT("Lvl"));
	m_currentXP = JsonObject->GetNumberField(TEXT("Xp"));
	m_instanceId = JsonObject->GetStringField(TEXT("InstanceId"));
	UE_LOG(LogTemp, Log, TEXT("ID: %s | Loaded: %s | Level: %d | HP: %d"), *m_id , *m_nameHero, m_Level, m_health);
}

FString UHero::GetId()
{
	return m_id;
}

ETypeClass UHero::GetClassHero()
{
	return m_heroClass;
}
int32 UHero::GetLevelHero()
{
	return m_Level;
}
int32 UHero::GetHealthHero()
{
	return m_health;
}
FTypeDamage UHero::GetDamageHero()
{
	return m_damage;
}
FTypeProtection UHero::GetDefenceHero()
{
	return m_protection;
}
int32 UHero::GetSpeed()
{
	return m_speed;
}
int32 UHero::GetAttackSpeed()
{
	return m_attackSpeed;
}
FString UHero::GetNameHero()
{
	return m_nameHero;
}
int32 UHero::GetCurrentXP()
{
	return m_currentXP;
}

int32 UHero::GetPrice()
{
	return m_price;
}
FString UHero::GetInstanceId()
{
	return m_instanceId;
}
void UHero::SetPrice(int32 price)
{
	m_price = price;
}
