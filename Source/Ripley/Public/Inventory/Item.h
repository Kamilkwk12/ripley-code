// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Effects/Condition.h"
#include "Interfaces/Interact.h"
#include "Gameplay/Effects/Effect.h"
#include "Item.generated.h"

class UInventoryComponent;

UCLASS()
class UItemType : public UObject
{
	GENERATED_BODY()
};

UCLASS(EditInlineNew)
class UActiveSkillItem : public UItemType
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TODO: Change to custom active skill class
	TSubclassOf<UActorComponent> ActiveSkillComponent;
};

UCLASS(EditInlineNew)
class UEquipmentItem : public UItemType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCondition Condition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	UEffect* Effect;
	
	
	static bool Compare(float Data, TEnumAsByte<ECompareOperation> CompareOperationEnum, float Value);
	float ReadData(UWorld* World, TEnumAsByte<EDataSource> DataSource);
};

UCLASS()
class RIPLEY_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UItemDataAsset()
	{
		ItemGuid = FGuid::NewGuid();
	}
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid ItemGuid;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	//Select Item Type (active skill, property enhancements, maybe more...) 
	UItemType* ItemType;
	
	UPROPERTY()
	//Component that was created during skill initialization
	UActorComponent* Component;
};

UCLASS(Blueprintable, category = "Item")
class RIPLEY_API AItem : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	AItem();
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY()
	USphereComponent* CollisionComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> ItemData;


protected:
	virtual void BeginPlay() override;
public:	
	UFUNCTION()
	void Interact_Implementation() override;  
	
};

