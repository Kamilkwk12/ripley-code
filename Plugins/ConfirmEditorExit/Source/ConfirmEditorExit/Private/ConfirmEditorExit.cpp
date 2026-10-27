// Copyright Szpont Studios. All Rights Reserved.

#include "ConfirmEditorExit.h"

#include "FileHelpers.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FRIPConfirmEditorExitModule"

void FRIPConfirmEditorExitModule::StartupModule()
{
	IMainFrameModule::Get().OnMainFrameCreationFinished().AddRaw(this, &FRIPConfirmEditorExitModule::OnEditorInitFinished);
}

void FRIPConfirmEditorExitModule::ShutdownModule()
{
	IMainFrameModule::Get().OnMainFrameCreationFinished().RemoveAll(this);
}

void FRIPConfirmEditorExitModule::OnEditorInitFinished(TSharedPtr<SWindow> Window, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("FRIPConfirmEditorExitModule::StartupModule()"));
	
	TArray<TSharedRef<SWindow>> AllWindows = FSlateApplication::Get().GetTopLevelWindows();;
	TSharedPtr<SWindow> MainWindow;
	
	if (AllWindows.Num() > 0)
	{
		MainWindow = AllWindows[0];
	}
	if (!MainWindow.IsValid())
	{
		return;
	}
	
	MainWindow->SetRequestDestroyWindowOverride(FRequestDestroyWindowOverride::CreateRaw(this, &FRIPConfirmEditorExitModule::ExitEditorChoice));
}

void FRIPConfirmEditorExitModule::ExitEditorChoice(const TSharedRef<SWindow>& Window)
{
	EAppReturnType::Type Choice = FMessageDialog::Open(EAppMsgType::YesNo, INVTEXT("Are you sure to close the editor?"));
	
	//on confirm, close the editor
	if (Choice == EAppReturnType::Yes)
	{
		UEditorLoadingAndSavingUtils::SaveDirtyPackagesWithDialog(true, true);
		GEngine->DeferredCommands.Add(TEXT("QUIT_EDITOR"));
	}
	
	//do nothing and discard exit
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRIPConfirmEditorExitModule, ConfirmEditorExit)