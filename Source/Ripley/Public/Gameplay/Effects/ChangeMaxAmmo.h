// Project R.I.P.LEY - Szpont Studios

#pragma once
#include "Effect.h"
#include "ChangeMaxAmmo.generated.h"

UCLASS(EditInlineNew)
class RIPLEY_API UChangeMaxAmmo : public UEffect
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value;
	
	virtual void ApplyEffect(AActor* TargetActor) override;
	virtual void RemoveEffect(AActor* TargetActor) override;
};
