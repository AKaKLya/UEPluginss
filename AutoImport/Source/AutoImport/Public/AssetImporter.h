#pragma once

#include "CoreMinimal.h"
#include "IAssetImporter.h"

class FTextureImporter : public IAssetImporter
{
public:
	FTextureImporter();
	virtual UFactory* GetFactory() override;
	virtual bool CanImport(const FString& FileName) override;
};

class FStaticMeshImporter : public IAssetImporter
{
public:
	FStaticMeshImporter();
	virtual UFactory* GetFactory() override;
	virtual bool CanImport(const FString& FileName) override;
};
