// Project R.I.P.LEY - Szpont Studios

#include "Gameplay/Effects/ChangeMaxAmmo.h"
#include "Player/RipleyCharacter.h"


void UChangeMaxAmmo::ApplyEffect(AActor* TargetActor)
{
	UE_LOG(LogEffects, Display, TEXT("UChangeMaxAmmo::ApplyEffect"));
	ARipleyCharacter* Player = Cast<ARipleyCharacter>(TargetActor);
	Player->PlayerModifiers.MaxAmmoModifier = Value;
	
	{
		Player->MaxAmmo = Player->PlayerModifiers.MaxAmmoModifier;
	}
}

void UChangeMaxAmmo::RemoveEffect(AActor* TargetActor)
{
	UE_LOG(LogEffects, Display, TEXT("UChangeMaxAmmo::RemoveEffect"));
	ARipleyCharacter* Player = Cast<ARipleyCharacter>(TargetActor);
	Player->PlayerModifiers.MaxAmmoModifier = 0;
	
	{
		Player->MaxAmmo = Player->BaseMaxAmmo;
		if (Player->CurrentAmmo > Player->MaxAmmo)
		{
			Player->SetCurrentAmmo(Player->MaxAmmo);
		}
	}
}