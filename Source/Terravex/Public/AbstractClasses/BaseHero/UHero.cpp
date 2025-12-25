#include "UHero.h"



FString UHero::GetNameHero()
{
	return FString("Unknown Hero");
}

ETypeClass UHero::GetClassHero()
{
	return ETypeClass::Mage;
}

int32 UHero::GetLevelHero()
{
	return 1;
}

int32 UHero::GetHealthHero()
{
	return 100;
}

FTypeDamage UHero::GetDamageHero()
{
	return FTypeDamage{};
}

int32 UHero::GetCurrentXP()
{
	return 0;
}

void UHero::GetSkills()
{
	
}
int32 UHero::GetPrice()
{
	return 0;
}
void UHero::SetPrice(int32 price)
{
	
}

void UHero::SaveToJsonHero(int32 id,
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

}

void UHero::LoadJsonHero(int32 id)
{
	
}

void UHero::LoadJsonHero(TSharedPtr<FJsonObject> JsonObject)
{
	
}
