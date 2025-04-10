// Fill out your copyright notice in the Description page of Project Settings.


#include "IAssetImporter.h"

#include "AssetImportTask.h"
#include "AssetToolsModule.h"
#include "AssetTools/Private/AssetTools.h"

#define ENABLE_LOG 0

IAssetImporter::IAssetImporter(const FString& InImportPath)
: ImportPath(InImportPath) 
{
	AssetToolsModule = &FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
}

IAssetImporter::~IAssetImporter()
{
}

bool IAssetImporter::IsFileReady(const FString& FilePath)
{
	constexpr int32 MaxRetries = 5;
	constexpr float RetryDelay = 0.01f;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	for (int32 i = 0; i < MaxRetries; ++i)
	{
		if (PlatformFile.FileExists(*FilePath) && 
		   PlatformFile.FileSize(*FilePath) > 0 && 
		   !PlatformFile.IsReadOnly(*FilePath))
		{
			return true;
		}
		FPlatformProcess::Sleep(RetryDelay);
	}
	return false;
}

void IAssetImporter::DeleteSourceFileWithRetry(const FString& FilePath)
{
	constexpr int32 MaxRetries = 3;
	constexpr float RetryInterval = 0.01f;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	for (int32 Attempt = 1; Attempt <= MaxRetries; ++Attempt)
	{
		if (!PlatformFile.FileExists(*FilePath))
		{
			#if ENABLE_LOG
			UE_LOG(LogTemp, Warning, TEXT("File already deleted: %s"), *FilePath);
			#endif
			
			return;
		}

		if (PlatformFile.DeleteFile(*FilePath))
		{
			#if ENABLE_LOG
			UE_LOG(LogTemp, Log, TEXT("Successfully deleted source file: %s"), *FilePath);
			#endif
			return;
		}
		#if ENABLE_LOG
		UE_LOG(LogTemp, Warning, TEXT("Delete failed (Attempt %d/%d): %s"), Attempt, MaxRetries, *FilePath);
		#endif
		
		FPlatformProcess::Sleep(RetryInterval);
	}
	
	#if ENABLE_LOG
	UE_LOG(LogTemp, Error, TEXT("Final delete failed: %s"), *FilePath);
	#endif
}

bool IAssetImporter::ImportAsset(const FString& FilePath)
{
	if (IsFileReady(FilePath) == false)
	{
		#if ENABLE_LOG
		UE_LOG(LogTemp, Warning, TEXT("File is locked or missing: %s"), *FilePath);
		#endif
		
		return false;
	}

	if (CanImport(FilePath) == false)
	{
		return false;
	}

	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->Filename = FilePath;
	ImportTask->DestinationPath = ImportPath;
	ImportTask->bReplaceExisting = true;
	ImportTask->bAutomated = true;
	ImportTask->bSave = true;
	
	UFactory* Factory = GetFactory();
	if (Factory == nullptr)
	{
		return false;
	}
	
	ImportTask->Factory = Factory;
	TArray<UAssetImportTask*> ImportTasks;
	ImportTasks.Add(ImportTask);
    
	AssetToolsModule->Get().ImportAssetTasks(ImportTasks);

	DeleteSourceFileWithRetry(FilePath);
	return true;
}
