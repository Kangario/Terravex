#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JSONSaver.generated.h"

UCLASS()
class TERRAVEX_API UJSONSaver : public UObject
{
	GENERATED_BODY()
public:
	void SetFilePath(const FString &FilePath);
	FString GetFilePath(int32 Id);
	void SaveJsonToFile(TSharedPtr<FJsonObject> JsonObject);
	TSharedPtr<FJsonObject> LoadJsonFromFile(int32 id);
	TArray<TSharedPtr<FJsonObject>> LoadAllJsonFromFiles();
	void DeleteJsonFromFile(int32 id);
	void DeleteAllJsonFiles();
private:
	FString m_filePath = FPaths::ProjectSavedDir();
};
