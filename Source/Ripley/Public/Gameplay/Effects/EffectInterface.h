// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EffectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEffectInterface : public UInterface
{
	GENERATED_BODY()
};

class RIPLEY_API IEffectInterface
{
	GENERATED_BODY()

public: 
	virtual void ApplyEffect(AActor* TargetActor);
	virtual void RemoveEffect(AActor* TargetActor);
};
