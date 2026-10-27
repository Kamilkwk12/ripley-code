// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkill.h"
#include "Components/ActorComponent.h"
#include "Bloodhalt.generated.h"


UCLASS( ClassGroup=(Skills), meta=(BlueprintSpawnableComponent), Blueprintable )
class RIPLEY_API UBloodhalt : public UActiveSkill
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBloodhalt();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Bloodhalt(); 
};
