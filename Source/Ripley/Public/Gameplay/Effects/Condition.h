// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "Condition.generated.h"


UENUM(BlueprintType)
enum EDataSource
{
	HealthPercent			UMETA(DisplayName = "Health Percent"),
	CurrentHealth			UMETA(DisplayName = "Health"),
	CurrentAmmo				UMETA(DisplayName = "Current Ammo"), 
	ShotsSinceReload		UMETA(DisplayName = "Shots Since Reload"),
	TimeSinceLastKill		UMETA(DisplayName = "Time Since Last Kill"),
};

UENUM(BlueprintType)
enum ECompareOperation : uint8
{
	LessThan				UMETA(DisplayName = "Less Than"), 
	GreaterThan				UMETA(DisplayName = "Greater Than"),
	Equal					UMETA(DisplayName = "Equal"),
	NotEqual				UMETA(DisplayName = "Not Equal"),
};

USTRUCT(BlueprintType)
struct RIPLEY_API FCondition
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPassive = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!IsPassive", EditConditionHides))
	TEnumAsByte<EDataSource> DataSource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!IsPassive", EditConditionHides))
	TEnumAsByte<ECompareOperation> CompareOperation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!IsPassive", EditConditionHides))
	float Value;
};