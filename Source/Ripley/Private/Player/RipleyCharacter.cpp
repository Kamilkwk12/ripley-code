// Project R.I.P.ley - Szpont Studios


#include "Player/RipleyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/Item.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Enemies/EnemyBase.h"
#include "Inventory/InventoryComponent.h"
#include "Player/PlayerUI.h"

DEFINE_LOG_CATEGORY(LogRipleyCharacter);

// Sets default values
ARipleyCharacter::ARipleyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//add inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("Inventory Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health Component"));
}

UInventoryComponent* ARipleyCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UHealthComponent* ARipleyCharacter::GetHealthComponent() const
{
	return HealthComponent;
}


void ARipleyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	UCameraComponent* PlayerCamera = this->FindComponentByClass<UCameraComponent>();
	PlayerCamera->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	EnableInput(PlayerController);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
	//Add mapping context
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(PlayerMappingContext, 0);
			
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT //debug mapping binding
			InputSubsystem->AddMappingContext(DebugMappingContext, 0);
#endif
			
		}
	}
	
	//Create PlayerUI widget
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
	PlayerUI->AddToViewport();
	PlayerUI->SetVisibility(ESlateVisibility::Visible);
	
	//configure capsule collision detection
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARipleyCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ARipleyCharacter::OnEndOverlap);
	
	//modifiers evaluation
	EvaluateModifiers();
	HealthComponent->OnHealthChangedDelegate.AddDynamic(this, &ARipleyCharacter::EvaluateModifiers);
}

// Called every frame
void ARipleyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FString CurrentDashCharges = "Dash Charges: " + FString::FromInt(DashCharges);
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, CurrentDashCharges, true);

}

// Called to bind functionality to input
void ARipleyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::MoveCharacter);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARipleyCharacter::SprintEnd);
		
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::MoveCamera);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::CharacterJump);
		
		EnhancedInputComponent->BindAction(ActiveSkillAction, ETriggerEvent::Started, this, &ARipleyCharacter::UseActiveSkill);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::Dash);
		
		EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Started, this, &ARipleyCharacter::StartFire);
		EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Completed, this, &ARipleyCharacter::StopFire);
		
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ARipleyCharacter::Reload);
		
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT //debug input binding
		EnhancedInputComponent->BindAction(DebugDeathAction, ETriggerEvent::Started, this, &ARipleyCharacter::DebugDeath);
		EnhancedInputComponent->BindAction(DebugHealthReductionAction, ETriggerEvent::Started, this, &ARipleyCharacter::DebugHealthReduction);
#endif
	}
}
 
void ARipleyCharacter::EvaluateModifiers()
{
	for (UItemDataAsset* Item : InventoryComponent->EquippedItems)
	{
		if (UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(Item->ItemType))
		{
			if (EquipmentItem->Condition.IsPassive)
			{
				EquipmentItem->Effect->ApplyEffect(this);
				break;
			}
			
			if (!EquipmentItem->Condition.IsPassive)
			{
				if (EquipmentItem->Compare(EquipmentItem->ReadData(GetWorld(), EquipmentItem->Condition.DataSource), EquipmentItem->Condition.CompareOperation, EquipmentItem->Condition.Value))
				{
					EquipmentItem->Effect->ApplyEffect(this);
				}
				else
				{
					EquipmentItem->Effect->RemoveEffect(this);
				}
			}
		}
	}
}


void ARipleyCharacter::MoveCharacter(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	const FVector ForwardVector = GetActorForwardVector();
	const FVector RightVector = GetActorRightVector();
	
	AddMovementInput(ForwardVector, InputVector.Y);
	AddMovementInput(RightVector, InputVector.X);
}

void ARipleyCharacter::SprintStart(const FInputActionValue& InputValue)
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
}

void ARipleyCharacter::SprintEnd(const FInputActionValue& InputValue)
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed;
}

void ARipleyCharacter::MoveCamera(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	AddControllerPitchInput(InputVector.Y);
	AddControllerYawInput(InputVector.X);
	
	CurrentCameraPitch += InputVector.Y;
	CurrentCameraPitch = FMath::Clamp(CurrentCameraPitch, -MaximumPitch, MaximumPitch);
}

void ARipleyCharacter::CharacterJump(const FInputActionValue& InputValue)
{
	if (GetMovementComponent()->IsMovingOnGround())
	{
		Jump();
		JumpDelegate.Broadcast();
	}
}

void ARipleyCharacter::UseActiveSkill(const FInputActionValue& InputValue)
{
	if (GetInventoryComponent()->EquippedActiveItems.Num() == 0)
	{
		return;
	}
	
	UE_LOG(LogRipleyCharacter, Display, TEXT("Using active skill"));
	OnActiveSkillUsed.Broadcast();
}
//TODO: Discuss if Dash should not be a active component
void ARipleyCharacter::Dash(const FInputActionValue& InputValue)
{
	if (IsDashing)
	{
		return;		
	}
	
	if (DashCharges > 0)
	{
		IsDashing = true;
		
		FVector3d DashVector = GetVelocity();
		DashVector = DashVector * DashDistance;
		DashVector.Z = 0;
		
		LaunchCharacter(DashVector, false, false);
		DashCharges--;
		
		IsDashRecharging = true;
		GetWorldTimerManager().SetTimer(DashRechargeTimerHandle, this, &ARipleyCharacter::RechargeDash, 1.0f, true);
	}
}

void ARipleyCharacter::RechargeDash()
{
	if (++DashCharges >= MaxDashCharges)
	{
		GetWorldTimerManager().ClearTimer(DashRechargeTimerHandle);
	}
	//TODO: create UI for dashes
	GEngine->AddOnScreenDebugMessage(2, 0.5, FColor::Green, TEXT("Dash recharge"), true);
}

void ARipleyCharacter::StartFire(const FInputActionValue& InputValue)
{
	Fire();
	
	//calculate seconds per round ( 1 sec / (rounds shot in a second) )
	float RoundDelay = 60 / RpmValue;
	GetWorldTimerManager().SetTimer(RpmTimerHandle, this, &ARipleyCharacter::Fire, RoundDelay, true);
}

void ARipleyCharacter::StopFire(const FInputActionValue& InputValue)
{
	GetWorldTimerManager().ClearTimer(RpmTimerHandle);
}

void ARipleyCharacter::Fire()
{
	if (CurrentAmmo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.3, FColor::Red, TEXT("No ammo"), true);
		return;
	}
	
	UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
	FVector3d CameraForwardVector = Camera->GetForwardVector();
	FVector3d CameraWorldLocation = Camera->GetComponentLocation();

	FHitResult HitResult;
	FCollisionQueryParams Params; 
	Params.AddIgnoredSourceObject(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, CameraWorldLocation, CameraWorldLocation + (CameraForwardVector * CameraTraceLength), ECC_Visibility, Params);
	
	OnFire.Broadcast(HitResult);
	
	EvaluateModifiers();
	
	if (HitResult.GetActor()) {
		DrawDebugSphere(GetWorld(), HitResult.Location, 2, 12, FColor::Red, false, 1, 0, 0);
		
		if (HitResult.GetActor()->IsA(AEnemyBase::StaticClass()))
		{
			if (HitResult.GetComponent()->ComponentHasTag("Collision_Head")) 
			{
				HealthComponent->ApplyDamage(HitResult, BaseDamage * HeadshotMult + (BaseDamage * DamageModifier));
			}
			else
			{
				HealthComponent->ApplyDamage(HitResult, BaseDamage + (BaseDamage * DamageModifier));
			}
		}
	};
	UAnimMontage* Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Player/Assets/UE5/Armed/Rifle/MM_Rifle_Fire_Montage.MM_Rifle_Fire_Montage"));
	PlayAnimMontage(Montage);
	CurrentAmmo -= BulletsPerShot;
	OnAmmoChanged.Broadcast();
	
}

void ARipleyCharacter::Reload(const FInputActionValue& InputValue)
{
	if (CurrentAmmo == MaxAmmo)
	{
		return;
	}
	UAnimMontage* Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Player/Assets/UE5/Armed/Rifle/MM_Rifle_Reload_Montage.MM_Rifle_Reload_Montage"));
	USkeletalMeshComponent* Skm = GetComponentByClass<USkeletalMeshComponent>();
	Skm->GetAnimInstance()->Montage_Play(Montage);
	Skm->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ARipleyCharacter::ResetAmmoValue);
}

void ARipleyCharacter::SetCurrentAmmo(int Value)
{
	CurrentAmmo = Value;
	OnAmmoChanged.Broadcast();
}

void ARipleyCharacter::ResetAmmoValue(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "Reload Completed")
	{
		CurrentAmmo = MaxAmmo; 
		OnReload.Broadcast();
	}
}

void ARipleyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Item interaction
	if (OtherActor->IsA<AItem>())
	{
		UE_LOG(LogRipleyCharacter, Display, TEXT("Item %s in range"), *OtherActor->GetName());
		PlayerUI->SetInteractionPromptVisibility(true);
	}
}

void ARipleyCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AItem>())
	{
		UE_LOG(LogRipleyCharacter, Display, TEXT("Item %s in range"), *OtherActor->GetName());
		PlayerUI->SetInteractionPromptVisibility(false);
	}
}	

void ARipleyCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	//end dash after landing on the ground
	if (PrevMovementMode == MOVE_Falling)
	{
		if (IsDashing)
		{
			IsDashing = false;
		}
	}
}


#if WITH_EDITOR || UE_BUILD_DEVELOPMENT //debug functions

void ARipleyCharacter::DebugDeath()
{
	HealthComponent->TakeDamage(HealthComponent->GetMaxHealth());
}

void ARipleyCharacter::DebugHealthReduction()
{
	HealthComponent->TakeDamage(DebugHealthReductionValue);
}

#endif

