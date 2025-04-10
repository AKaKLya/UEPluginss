// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoImportModule.h"

#include "ImportDirectoryWatcher.h"
#include "AssetImporter.h"

#define LOCTEXT_NAMESPACE "FAutoImportTextureModule"

bool CheckAndCreateDirectory(IPlatformFile& PlatformFile,const FString& DirectoryPath)
{
	FString NormalizedPath = FPaths::ConvertRelativePathToFull(DirectoryPath);
	
	if (!PlatformFile.DirectoryExists(*NormalizedPath))
	{
		const bool bTreeCreated = PlatformFile.CreateDirectoryTree(*NormalizedPath);
        
		if (bTreeCreated)
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("目录创建失败：%s"), *NormalizedPath);
			return false;
		}
	}
	
	return true;
}

void FAutoImportModule::StartupModule()
{
	// 读配置文件获取文件夹
	FString WatchDirectory = TEXT("D:/UEImport");
	
	FString PluginConfigPath = FPaths::Combine(FPaths::ProjectPluginsDir(),TEXT("AutoImport"),TEXT("Config"),TEXT("DefaultAutoImport.ini"));

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (!PlatformFile.FileExists(*PluginConfigPath))
	{
		UE_LOG(LogTemp, Fatal, TEXT("插件配置文件缺失：%s , 默认目录:%s"), *PluginConfigPath,*WatchDirectory);
	}

	CheckAndCreateDirectory(PlatformFile,WatchDirectory);
	
	GConfig->Flush(true, *FPaths::GetPath(PluginConfigPath));
	GConfig->GetString(TEXT("AutoImport"),TEXT("WatchDirectory"),WatchDirectory,PluginConfigPath);
	// 读配置文件获取文件夹

	// 监视文件夹
	DirectoryWatcher = MakeUnique<FImportDirectoryWatcher>(WatchDirectory);

	DirectoryWatcher->OnFileChangedDelegate.BindRaw(this, &FAutoImportModule::HandleFileChanged);
    
	TextureImporter = MakeUnique<FTextureImporter>();
	StaticMeshImporter = MakeUnique<FStaticMeshImporter>();
	// 监视文件夹
}

void FAutoImportModule::ShutdownModule()
{

}
/* 文件夹发生变更时触发这个函数 */
void FAutoImportModule::HandleFileChanged(const FString& FilePath)
{
    if (TextureImporter.IsValid())
    {
        if (TextureImporter->ImportAsset(FilePath)) {return;}
    }

	if (StaticMeshImporter.IsValid())
	{
		if (StaticMeshImporter->ImportAsset(FilePath)) {return;}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutoImportModule, AutoImport)