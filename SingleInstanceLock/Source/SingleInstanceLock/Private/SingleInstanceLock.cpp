// Copyright Epic Games, Inc. All Rights Reserved.

#include "SingleInstanceLock.h"

#include "Misc/MessageDialog.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "windows.h"
#include "Windows/HideWindowsPlatformTypes.h"

#define LOCTEXT_NAMESPACE "FSingleInstanceLockModule"

/*
Together we will make UE strong again,😠
we will make UE proud again,✋🤴🤚
we will make UE safe again.🎈🧷
And, yes, together we will make UE GREAT again.😎📢💨
God bless you.👨‍🍳
And good night🌙  I love you.💖💖💖💖💖
*/

void FSingleInstanceLockModule::StartupModule()
{
	const FString ProjectPath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	//const FString ProjectName = FApp::GetProjectName();
	
	FString MutexName = FString::Printf(TEXT("Global\\UE4_SingleInstance_%s"), *ProjectPath);
	MutexName.ReplaceCharInline(TEXT('/'), TEXT('_'));
	MutexName.ReplaceCharInline(TEXT(':'), TEXT('_'));
	
	//如果互斥体是命名互斥体，并且此函数调用之前存在该对象，则返回值是现有对象的句柄，GetLastError 函数返回 ERROR_ALREADY_EXISTS。
	//https://learn.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-createmutexw
	WindowsMutexHandle = CreateMutexW(nullptr, true, *MutexName);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//const FText Tittle = FText::FromString(TEXT("回答我🤬! Answer Me😡! Look in my eyes👀答えます!"));
		const FString TittleStr = FString::Printf(TEXT("%d年 UE科举模拟测试卷(-殿试-绝密-)"),FMath::RandRange(622,2060));
		const FString DisplayLog = FString::Printf(TEXT("阅读下面四段对话，从题中所给的两个选项中，选出最佳选项。(5分)\n"))
		+ FString::Printf(TEXT("---------------------------------------------------\n"))
		+ FString::Printf(TEXT("string X = %s\n"), *ProjectPath)
		+ FString::Printf(TEXT("X 已经运行，是否继续运行？\n"))
		+ FString::Printf(TEXT("X is running. Do you want to continue running?\n"))
		+ FString::Printf(TEXT("X は稼働していますが、引き続き稼働していますか?\n"))
		+ FString::Printf(TEXT("---------------------------------------------------"));
		
		
		//const EAppReturnType::Type Select = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(DisplayLog),&Tittle);
		int32 Return = MessageBox( nullptr, *DisplayLog, *TittleStr, MB_ICONQUESTION|MB_YESNO|MB_SYSTEMMODAL );
	
		if (Return == IDNO)
		{
			FPlatformMisc::RequestExit(true);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("SingleInstanceLockModule - Mutex Already Exists:%s"), *MutexName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("SingleInstanceLockModule - Create Mutex:%s"), *MutexName);
	}
}

void FSingleInstanceLockModule::ShutdownModule()
{
/*
	@  当进程退出时，Windows会自动清理Mutex，不必担心崩溃引起bug。这个函数里面的判断是为了有始有终。
	@  When a process exits, Windows automatically cleans up the mutex, so there is no need to worry about bugs caused by crashes.
	   The checks in this function are to ensure proper cleanup (to have a proper beginning and end).
	@  プロセスが終了する際、Windowsは自動的にミューテックスを解放するため、クラッシュによるバグを心配する必要はありません。
		この関数内の判定は、適切なクリーンアップ（始まりと終わりの完結性）を保証するために行われています。
	@  Когда процесс выходит из строя, Windows автоматически очищает Mutex, не опасаясь, что коллапс вызовет баг. Суждение внутри этой функции предназначено для начала и конца.
	
	A[进程A: CreateMutex] --> RefCount1[RefCount=1]
	B[进程B: CreateMutex] --> RefCount2[RefCount=2]
	C[进程C: CreateMutex] --> RefCount3[RefCount=3]
	A关闭句柄 --> RefCount2[RefCount=2]🧹
	B崩溃退出 --> RefCount1[RefCount=1]🧹
	C关闭句柄 --> RefCount0[RefCount=0]🧹
	RefCount0 --> Mutex销毁🔥
*/
	if (WindowsMutexHandle)
	{
		ReleaseMutex(WindowsMutexHandle);
		CloseHandle(WindowsMutexHandle);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSingleInstanceLockModule, SingleInstanceLock)