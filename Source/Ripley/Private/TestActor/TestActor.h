// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ATestActor();
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
