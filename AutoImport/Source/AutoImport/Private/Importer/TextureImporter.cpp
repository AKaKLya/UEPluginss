#include "AssetImporter.h"
#include "Factories/TextureFactory.h"


FTextureImporter::FTextureImporter()
: Super(FString("/Game/Z_Texture"))
{
	
}

UFactory* FTextureImporter::GetFactory()
{
	UTextureFactory* TextureFactory = NewObject<UTextureFactory>();
	TextureFactory->SuppressImportOverwriteDialog();
	TextureFactory->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	TextureFactory->CompressionSettings = TextureCompressionSettings::TC_Default;
	TextureFactory->ColorSpaceMode = ETextureSourceColorSpace::SRGB;
	return TextureFactory;
}

bool FTextureImporter::CanImport(const FString& FileName)
{
	if (FileName.EndsWith(".png") || FileName.EndsWith(".jpg") || FileName.EndsWith(".TGA"))
	{
		return true;
	}
	return false;
}
