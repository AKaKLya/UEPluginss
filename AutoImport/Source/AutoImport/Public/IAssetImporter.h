// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FAssetToolsModule;

class IAssetImporter
{
public:
	using Super = IAssetImporter;
	
	/* 子类必须传入一个参数 用于初始化 ImportPath 变量.*/
	explicit IAssetImporter(const FString& InImportPath);
	
	virtual ~IAssetImporter();

	/* 导入资产的实现 */
	bool ImportAsset(const FString& FilePath);

protected:
	/* 检测文件是否准备就绪 */
	bool IsFileReady(const FString& FilePath);

	/* 资产入之后，删除文件夹中的文件 .*/
	void DeleteSourceFileWithRetry(const FString& FilePath);

	/* 应对不同资产的导入方式 .*/
	virtual UFactory* GetFactory() = 0;

	/* 通过文件名 判断是否导入 .*/
	virtual bool CanImport(const FString& FileName) = 0;

	/* 资产导入到工程中的哪个文件夹? . 例如: "/Game/Z_Texture" */
	FString ImportPath;
	
	FAssetToolsModule* AssetToolsModule;
};

