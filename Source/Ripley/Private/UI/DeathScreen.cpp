// Project R.I.P.LEY - Szpont Studios


#include "UI/DeathScreen.h"

#include "Kismet/GameplayStatics.h"


void UDeathScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	RestartButton->OnPressed.AddDynamic(this, &UDeathScreen::Restart);
}

void UDeathScreen::Restart()
{
	UObject* WorldContext = GWorld->GetFirstPlayerController()->GetPawn();
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(WorldContext);
	
	UGameplayStatics::OpenLevel(WorldContext, FName(CurrentLevelName));
	
	RemoveFromParent();
	
	APlayerController* PlayerController = GWorld->GetFirstPlayerController();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}
