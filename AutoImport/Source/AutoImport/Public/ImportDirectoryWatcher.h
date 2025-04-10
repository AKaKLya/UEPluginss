#pragma once

#include "CoreMinimal.h"
#include "IDirectoryWatcher.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"


DECLARE_DELEGATE_OneParam(FOnFileChanged, const FString&);

class FImportDirectoryWatcher
{
public:
	FImportDirectoryWatcher(FString InWatchDirectory);
	~FImportDirectoryWatcher();
	void Shutdown();
    
	FOnFileChanged OnFileChangedDelegate;

private:
	void HandleFileChanged(const TArray<FFileChangeData>& FileChanges);

	FDelegateHandle DirectoryWatcherHandle;
	FString WatchedDirectory;
};