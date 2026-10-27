// Project R.I.P.ley - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/Health/HealthComponent.h"
#include "Inventory/InventoryComponent.h"
#include "PlayerUI.h"	
#include "RipleyCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRipleyCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterJumpDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveSkillUsedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireDelegate, FHitResult, HitActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModifiersChanged);

USTRUCT(BlueprintType)
struct FPlayerModifiers
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float MaxAmmoModifier = 0; 
};

UCLASS(Blueprintable)
class RIPLEY_API ARipleyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARipleyCharacter();
	
	UPROPERTY(BlueprintAssignable)
	FOnCharacterJumpDelegate JumpDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnActiveSkillUsedDelegate OnActiveSkillUsed;
	
	UPROPERTY(BlueprintAssignable)
	FOnFireDelegate OnFire;
	
	UPROPERTY(BlueprintAssignable)
	FOnReloadDelegate OnReload; 
	
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnModifiersChanged OnModifiersChanged;
	
	UPROPERTY()
	APlayerController* PlayerController;
	
	//Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputActions")
	class UInputMappingContext* PlayerMappingContext;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking", meta=(Units="CentimetersPerSecond"))
	float WalkingSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking", meta=(Units="CentimetersPerSecond"))
	float SprintSpeed;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* MoveCameraAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* ActiveSkillAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* ShootingAction;
	
	UPROPERTY(EditAnywhere, Category = "InputActions")
	UInputAction* ReloadAction;
	
	UPROPERTY(BlueprintReadWrite, Category = "Dash Settings")
	bool IsDashing;
	
	UPROPERTY(BlueprintReadWrite, Category = "Dash Settings")
	bool IsDashRecharging;
	
	UPROPERTY(BlueprintReadWrite, Category = "Dash Settings")
	int DashCharges = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	int MaxDashCharges = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float DashDistance = 10;
	
	FTimerHandle DashRechargeTimerHandle;
	
	UPROPERTY(BlueprintReadOnly, Category= "Camera Controller")
	float CurrentCameraPitch = 0;
	
	UPROPERTY(BlueprintReadOnly, Category= "Camera Controller")
	float MaximumPitch = 36;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerUI")
	TSubclassOf<UPlayerUI> PlayerUIClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "PlayerUI")
	UPlayerUI* PlayerUI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(BlueprintReadOnly)
	FPlayerModifiers PlayerModifiers;
	
	UFUNCTION(BlueprintCallable)
	//getter for inventory component
	UInventoryComponent* GetInventoryComponent() const;
	
	//getter for health component
	UHealthComponent* GetHealthComponent() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	int CurrentAmmo = 30; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	int BaseMaxAmmo = 30; 
	
	UPROPERTY()
	int MaxAmmo = BaseMaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float RpmValue = 600;
	
	FTimerHandle RpmTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float BulletsPerShot = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float CameraTraceLength = 10000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float BaseDamage = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float HeadshotMult = 1.5;

	UPROPERTY()
	float DamageModifier; 
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frames
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void EvaluateModifiers();
	
	UFUNCTION()
	void MoveCharacter(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void SprintStart(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void SprintEnd(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void MoveCamera(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void CharacterJump(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void UseActiveSkill(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void Dash(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void RechargeDash();

	UFUNCTION()
	void StartFire(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void StopFire(const FInputActionValue& InputValue);

	UFUNCTION()
	void Fire();
	
	UFUNCTION()
	void Reload(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void SetCurrentAmmo(int Value);
	
	UFUNCTION()
	void ResetAmmoValue(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public: 
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
#if WITH_EDITORONLY_DATA || UE_BUILD_DEVELOPMENT  //player debug options
	
public: 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Tools")
	UInputMappingContext* DebugMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Tools")
	UInputAction* DebugDeathAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Tools")
	UInputAction* DebugHealthReductionAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Tools")
	float DebugHealthReductionValue;
	
#endif
	
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	
private: 
	
	void DebugDeath(); 
	
	void DebugHealthReduction();
	
#endif
};