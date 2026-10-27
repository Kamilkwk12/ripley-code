// Project R.I.P.LEY - Szpont Studios


#include "UI/HealthBar.h"
#include "Player/RipleyCharacter.h"

void UHealthBar::Update()
{
	if (UHealthComponent* HealthComponent = Cast<ARipleyCharacter>(GWorld->GetFirstPlayerController()->GetPawn())->GetHealthComponent())
	{
		float NewPercent = HealthComponent->GetCurrentHealth() / HealthComponent->GetMaxHealth();
		HealthBarComponent->SetPercent(NewPercent);
		
		HealthCounter->SetText(FText::AsNumber(HealthComponent->GetCurrentHealth()));
	}
}
