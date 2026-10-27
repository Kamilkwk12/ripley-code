// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effects/Effect.h"
#include "AdditionalDamage.generated.h"

UCLASS(EditInlineNew)
class RIPLEY_API UAdditionalDamage : public UEffect
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Units="Percent"))
	//Damage bonus calculated from player's BaseDamage 
	float DamageBonusPercentage = 0;
	
	virtual void ApplyEffect(AActor* TargetActor) override;
	virtual void RemoveEffect(AActor* TargetActor) override;
};
