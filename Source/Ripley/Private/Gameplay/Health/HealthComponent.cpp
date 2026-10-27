// Project R.I.P.LEY - Szpont Studios


#include "Gameplay/Health/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Player/RipleyCharacter.h"
#include "UI/DeathScreen.h"

DEFINE_LOG_CATEGORY(LogHealthComponent);

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bCanTakeDamage = true;
}

void UHealthComponent::ApplyDamage(FHitResult Context, float DamageValue)
{
	UGameplayStatics::ApplyPointDamage(Context.GetActor(), DamageValue, Context.TraceStart, Context, GetWorld()->GetFirstPlayerController(), GetOwner(), nullptr);
	UE_LOG(LogTemp, Display, TEXT("Damage Applied"));
}

void UHealthComponent::TakeDamage(float DamageValue)
{
	if (bCanTakeDamage)
	{
		CalculateHealth(Damage, DamageValue);
		OnDamageReceivedDelegate.Broadcast();
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	CalculateHealth(Healing, HealAmount);
	OnHealingDelegate.Broadcast();
}

float UHealthComponent::CalculateHealth(EHealthActionType HealthActionType, float Value)
{
	switch (HealthActionType)
	{
		case Damage:
			CurrentHealth = FMath::Clamp(CurrentHealth -= Value, 0.0f, MaxHealth);
			if (CurrentHealth <= 0)
			{
				Death();
				bCanTakeDamage = false; 
			}
			break; 
		
		case Healing: 
			CurrentHealth = FMath::Clamp(CurrentHealth += Value, 0.0f, MaxHealth);
			break; 
		
		case None: 
		default:
			return CurrentHealth;
	}
	OnHealthChangedDelegate.Broadcast();
	return CurrentHealth;
}

void UHealthComponent::Death()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		
		if (DeathScreenWidgetClass == nullptr)
		{
			UE_LOG(LogHealthComponent, Fatal, TEXT("Death Screen Widget has not been assigned in Health Component."));
		}
		
		PlayerController->GetCharacter()->DisableInput(PlayerController);
		
		UDeathScreen* DeathScreenWidget = CreateWidget<UDeathScreen>(PlayerController, DeathScreenWidgetClass);
		
		DeathScreenWidget->AddToViewport(2);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		
		OnDeathDelegate.Broadcast();
	}
}

float& UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float& UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}