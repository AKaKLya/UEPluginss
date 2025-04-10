#include "ImportDirectoryWatcher.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "DirectoryWatcherModule.h"


/* 在 Windows 平台下，FDirectoryWatcherModule 最终调用了 ReadDirectoryChangesW 函数，
 * 对于文件的更改会触发函数 ImportDirectoryWatcher::HandleFileChanged 的调用.
 * 详见:
 * Engine\Source\Developer\DirectoryWatcher\Private\Windows\DirectoryWatchRequestWindows.cpp
 * https://learn.microsoft.com/zh-cn/windows/win32/api/winbase/nf-winbase-readdirectorychangesw
 */
FImportDirectoryWatcher::FImportDirectoryWatcher(FString InWatchDirectory)
	: WatchedDirectory(InWatchDirectory)
{
	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>("DirectoryWatcher");
    
	IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();
	if (DirectoryWatcher)
	{
		DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(WatchedDirectory,
			IDirectoryWatcher::FDirectoryChanged::CreateRaw(this, &FImportDirectoryWatcher::HandleFileChanged),
			DirectoryWatcherHandle
		);
	}
}

FImportDirectoryWatcher::~FImportDirectoryWatcher()
{
	Shutdown();
}

void FImportDirectoryWatcher::Shutdown()
{
	FDirectoryWatcherModule* DirectoryWatcherModule = FModuleManager::GetModulePtr<FDirectoryWatcherModule>("DirectoryWatcher");
    
	if (DirectoryWatcherModule)
	{
		IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule->Get();
		if (DirectoryWatcher && DirectoryWatcherHandle.IsValid())
		{
			DirectoryWatcher->UnregisterDirectoryChangedCallback_Handle(WatchedDirectory, DirectoryWatcherHandle);
		}
	}
}

/*如果监测的文件夹发生变动，这个函数将被触发 --> 发出变更通知
 */
void FImportDirectoryWatcher::HandleFileChanged(const TArray<FFileChangeData>& FileChanges)
{
	for (const FFileChangeData& Change : FileChanges)
	{
		if (Change.Action == FFileChangeData::FCA_Added || Change.Action == FFileChangeData::FCA_Modified)
		{
			OnFileChangedDelegate.ExecuteIfBound(Change.Filename);
		}
	}
}