// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/DeathScreen.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageApplied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageReceived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Log, All);

UENUM(BlueprintType, Category="Health")
enum EHealthActionType
{
	None,
	Damage, 
	Healing
};

UCLASS( ClassGroup=(Gameplay), meta=(BlueprintSpawnableComponent), BlueprintType )
class RIPLEY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	
	UPROPERTY(BlueprintAssignable)
	FOnDamageReceived OnDamageReceivedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealing OnHealingDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float CurrentHealth = 100.0f; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY(BlueprintReadWrite, Category="Health")
	bool bCanTakeDamage = true;
	//gameplay functions
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Calculates heal value after the action and broadcasts a message to bind delegates
	virtual void ApplyDamage(FHitResult Context, float DamageValue);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Calculates heal value after the action and broadcasts a message to bind delegates
	virtual void TakeDamage(float DamageValue);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Calculates heal value after the action and broadcasts a message to bind delegates
	virtual void Heal(float HealAmount);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Calculates new health value based on action and given value
	virtual float CalculateHealth(EHealthActionType HealthActionType, float Value);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Death state with delegate broadcast
	virtual void Death();
	
	//TODO: Move Death Screen reference to General UI HUD class (UI HUD class should contain all references to blueprint widgets)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	TSubclassOf<UDeathScreen> DeathScreenWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Get Current Health from the health component
	float& GetCurrentHealth();
	
	UFUNCTION(BlueprintCallable, Category="Health")
	//Get Max Health from the health component
	float& GetMaxHealth();
	
};
