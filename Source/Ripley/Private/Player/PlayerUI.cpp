// Project R.I.P.LEY - Szpont Studios


#include "Player/PlayerUI.h"
#include "Gameplay/Health/HealthComponent.h"
#include "Player/RipleyCharacter.h"


void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);
	
	if (ARipleyCharacter* PlayerReference = Cast<ARipleyCharacter>(GWorld->GetFirstPlayerController()->GetPawn()))
	{
		HealthComponent = PlayerReference->GetHealthComponent();
		HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &UPlayerUI::UpdateUI);
	}
	
}

void UPlayerUI::UpdateAmmoCounter(int NewAmmoValue)
{
	AmmoCounter->SetText(FText::AsNumber(NewAmmoValue));
}

void UPlayerUI::UpdateUI()
{
	HealthBar->Update();
}

void UPlayerUI::SetInteractionPromptVisibility(bool Visible)
{
	if (Visible)
	{
		InteractionPrompt->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);
	}
}

