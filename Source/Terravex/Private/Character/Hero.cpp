// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero.h"

#include <string>

UTHero::UTHero()
{
}

UTHero::~UTHero()
{
}
void UTHero::SaveToJsonHero(int32 id,
	FString nameHero,
	ETypeClass heroClass,
	int32 health,
	FTypeDamage damage,
	FTypeProtection protection,
	int32 speed,
	int32 attackSpeed,
	int32 level,
	int32 currentXP,
	int32 maxXP)
{
	TSharedPtr<FJsonObject> HeroObject = MakeShared<FJsonObject>();
	
	HeroObject->SetNumberField("Id", id);
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
	FString outputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(HeroObject.ToSharedRef(), Writer);
	const FString FilePath = FPaths::ProjectSavedDir() +TEXT("Heros/")+ FString::FromInt(id) + TEXT(".json");
	
	FFileHelper::SaveStringToFile(outputString, *FilePath);
}

void UTHero::LoadJsonHero(int32 id)
{
	const FString FilePath = FPaths::ProjectSavedDir() +TEXT("Heros/")+ FString::FromInt(id) + TEXT(".json");

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

	m_id = JsonObject->GetIntegerField(TEXT("Id"));
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
	
	UE_LOG(LogTemp, Log, TEXT("ID: %d | Loaded: %s | Level: %d | HP: %d"), m_id , *m_nameHero, m_Level, m_health);
}

void UTHero::LoadJsonHero(TSharedPtr<FJsonObject> JsonObject)
{
	m_id = JsonObject->GetIntegerField(TEXT("Id"));
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
	
	UE_LOG(LogTemp, Log, TEXT("ID: %d | Loaded: %s | Level: %d | HP: %d"), m_id , *m_nameHero, m_Level, m_health);
}

ETypeClass UTHero::GetClassHero()
{
	return m_heroClass;
}
int32 UTHero::GetLevelHero()
{
	return m_Level;
}
int32 UTHero::GetHealthHero()
{
	return m_health;
}
FTypeDamage UTHero::GetDamageHero()
{
	return m_damage;
}
FString UTHero::GetNameHero()
{
	return m_nameHero;
}
int32 UTHero::GetCurrentXP()
{
	return m_currentXP;
}

int32 UTHero::GetPrice()
{
	return m_price;
}
void UTHero::SetPrice(int32 price)
{
	m_price = price;
}
