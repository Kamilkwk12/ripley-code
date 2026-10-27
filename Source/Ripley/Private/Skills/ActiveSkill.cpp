// Project R.I.P.LEY - Szpont Studios


#include "Skills/ActiveSkill.h"

// Sets default values for this component's properties
UActiveSkill::UActiveSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UActiveSkill::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UActiveSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

