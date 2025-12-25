#include "UI/Saves/JSONSaver.h"

void UJSONSaver::SetFilePath(const FString& FilePath)
{
	m_filePath = FilePath;
}

FString UJSONSaver::GetFilePath(int32 id)
{
	FString SubFolder = TEXT("Heros");
	FString FileName = FString::FromInt(id) + TEXT(".json");
	FString FullDir = FPaths::Combine(m_filePath, SubFolder);
	IFileManager::Get().MakeDirectory(*FullDir, true);
	FString FullPath = FPaths::Combine(FullDir, FileName);
	return FullPath;
}

void UJSONSaver::SaveJsonToFile(TSharedPtr<FJsonObject> JsonObject)
{
	if (!JsonObject.IsValid())
	{
		return;
	}
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		int32 id = JsonObject->GetIntegerField(TEXT("Id"));
		FFileHelper::SaveStringToFile(OutputString, *GetFilePath(id));
	}
	
}

TSharedPtr<FJsonObject> UJSONSaver::LoadJsonFromFile(int32 id)
{
	FString FileContent;
	if (FFileHelper::LoadFileToString(FileContent, *GetFilePath(id)))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			return JsonObject;
		}
	}
	return nullptr;
}

TArray<TSharedPtr<FJsonObject>> UJSONSaver::LoadAllJsonFromFiles()
{
	TArray<TSharedPtr<FJsonObject>> Result;

	FString FullDir = FPaths::Combine(m_filePath, TEXT("Heros"));
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> FoundFiles;
	FileManager.FindFiles(FoundFiles, *FullDir, TEXT("*.json"));

	for (const FString& FileName : FoundFiles)
	{
		FString FullPath = FPaths::Combine(FullDir, FileName);
		FString FileContent;
		if (FFileHelper::LoadFileToString(FileContent, *FullPath))
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				Result.Add(JsonObject);
			}
		}
	}

	return Result;
}

void UJSONSaver::DeleteJsonFromFile(int32 id)
{
	FString FilePath = GetFilePath(id);
	if (FPaths::FileExists(FilePath))
	{
		IFileManager::Get().Delete(*FilePath);
	}
}

void UJSONSaver::DeleteAllJsonFiles()
{
	FString FullDir = FPaths::Combine(m_filePath, TEXT("Heros"));
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> FoundFiles;
	FileManager.FindFiles(FoundFiles, *FullDir, TEXT("*.json"));

	for (const FString& FileName : FoundFiles)
	{
		FString FullPath = FPaths::Combine(FullDir, FileName);
		FileManager.Delete(*FullPath);
	}
}