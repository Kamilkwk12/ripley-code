// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FRIPConfirmEditorExitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void OnEditorInitFinished(TSharedPtr<SWindow> Window, bool bSuccess);
	
	void ExitEditorChoice(const TSharedRef<SWindow>& Window);
};
