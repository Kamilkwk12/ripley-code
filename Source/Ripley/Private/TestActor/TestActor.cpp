// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor/TestActor.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Test actor has been recorded on a level"));
	
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

