// Project R.I.P.LEY - Szpont Studios


#include "Inventory/Item.h"
#include "Player/RipleyCharacter.h"
#include "Inventory/InventoryComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	ARipleyCharacter* PlayerReference = Cast<ARipleyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	InventoryComponent = PlayerReference->GetInventoryComponent();
}

void AItem::Interact_Implementation()
{
	IInteract::Interact_Implementation();
	
	if (ItemData == nullptr)
	{
		UE_LOG(LogInventory, Error, TEXT("Item Data is nullptr"));
		return;
	}
	
	InventoryComponent->InventoryItems.Add(ItemData);
	InventoryComponent->OnInventoryStateChanged.Broadcast(ItemData);
	
	Destroy();
}

bool UEquipmentItem::Compare(float Data, TEnumAsByte<ECompareOperation> CompareOperationEnum, float Value)
{
	switch (CompareOperationEnum)
	{
	case Equal:	
		return Data == Value;
	case NotEqual:
		return Data != Value;
	case GreaterThan:
		return Data > Value;
	case LessThan:
		return Data < Value;
	default: return true;
	}
}

float UEquipmentItem::ReadData(UWorld* World, TEnumAsByte<EDataSource> Source)
{
	ARipleyCharacter* PlayerRef = Cast<ARipleyCharacter>(World->GetFirstPlayerController()->GetPawn());
	UHealthComponent* PlayerHealthComponent = PlayerRef->GetHealthComponent();
	
	switch (Source)
	{
	case HealthPercent: 
		return PlayerHealthComponent->GetCurrentHealth() / PlayerHealthComponent->GetMaxHealth() * 100;
		
	case CurrentAmmo:
		return PlayerRef->CurrentAmmo;
		
	case CurrentHealth:
		return PlayerHealthComponent->GetCurrentHealth();
		
	default: return 0; 
	}
}




