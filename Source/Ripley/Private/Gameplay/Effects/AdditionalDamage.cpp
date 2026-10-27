// Project R.I.P.LEY - Szpont Studios


#include "Gameplay/Effects/AdditionalDamage.h"

#include "Player/RipleyCharacter.h"


void UAdditionalDamage::ApplyEffect(AActor* TargetActor)
{
	UE_LOG(LogEffects, Display, TEXT("AdditionalDamage::ApplyEffect"));
	ARipleyCharacter* Player = Cast<ARipleyCharacter>(TargetActor);
	Player->DamageModifier = DamageBonusPercentage / 100;
}

void UAdditionalDamage::RemoveEffect(AActor* TargetActor)
{
	UE_LOG(LogEffects, Display, TEXT("AdditionalDamage::RemoveEffect"));
	ARipleyCharacter* Player = Cast<ARipleyCharacter>(TargetActor);
	Player->DamageModifier = 0;
}
