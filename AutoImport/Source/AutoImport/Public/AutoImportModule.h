// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAssetImporter;
class FImportDirectoryWatcher;

class FAutoImportModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TUniquePtr<FImportDirectoryWatcher> DirectoryWatcher;
	TUniquePtr<IAssetImporter> TextureImporter;
	TUniquePtr<IAssetImporter> StaticMeshImporter;
	
	void HandleFileChanged(const FString& FilePath);
	
};
