// Project R.I.P.LEY - Szpont Studios

#pragma once
#include "EffectInterface.h"
#include "Effect.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEffects, Log, All);

UCLASS()
class RIPLEY_API UEffect : public UObject, public IEffectInterface
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(AActor* TargetActor) override;
	virtual void RemoveEffect(AActor* TargetActor) override;
};
