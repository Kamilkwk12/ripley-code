// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActiveSkill.generated.h"

//Base Class for Active Player Skills
UCLASS( ClassGroup=(Skills), meta=(BlueprintSpawnableComponent) )
class RIPLEY_API UActiveSkill : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActiveSkill();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillDescription;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
