// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetImporter.h"

#include "Factories/ReimportFbxStaticMeshFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxStaticMeshImportData.h"

FStaticMeshImporter::FStaticMeshImporter()
:Super(FString("/Game/Z_StaticMesh"))
{
	
}

UFactory* FStaticMeshImporter::GetFactory()
{
	UFbxFactory* FBXFactory = NewObject<UFbxFactory>();
	UFbxImportUI* UI = FBXFactory->ImportUI;
	UI->StaticMeshImportData->VertexColorImportOption = EVertexColorImportOption::Replace;
	return FBXFactory;
}

bool FStaticMeshImporter::CanImport(const FString& FileName)
{
	if (FileName.EndsWith(".fbx"))
	{
		return true;
	}
	return false;
}
