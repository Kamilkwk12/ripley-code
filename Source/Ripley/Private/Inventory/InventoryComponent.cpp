// Project R.I.P.ley - Szpont Studios

#include "Inventory/InventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Player/RipleyCharacter.h"
#include "Inventory/InventoryUI.h"

DEFINE_LOG_CATEGORY(LogInventory);

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerReference = Cast<ARipleyCharacter>(GetOwner());
	
	OnInventoryStateChanged.AddDynamic(this, &UInventoryComponent::ShowStorageItems);
	OnItemRegister.AddDynamic(this, &UInventoryComponent::RegisterItem);
	OnItemUnregister.AddDynamic(this, &UInventoryComponent::RemoveItem);
	
	PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	
	//construct inventory widget
	InventoryUIWidget = CreateWidget<UInventoryUI>(this->GetWorld(), InventoryWidgetClass);
	InventoryUIWidget->AddToViewport(1);
	InventoryUIWidget->SetVisibility(ESlateVisibility::Hidden);
	
	//bind inventory action input
	if (PlayerReference)
	{
		PlayerReference->EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UInventoryComponent::OpenInventory);
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UInventoryComponent::OpenInventory(const FInputActionValue& Value)
{
	if (!bInventoryOpened)
	{
		InventoryUIWidget->SetVisibility(ESlateVisibility::Visible);
		bInventoryOpened = true;
		
		if (PlayerController)
		{
			PlayerController->SetPause(true);
			PlayerController->SetInputMode(FInputModeGameAndUI());
			PlayerController->SetShowMouseCursor(true);
		}
		UE_LOG(LogInventory, Display, TEXT("Inventory Opened"));
		
	} else
	{
		InventoryUIWidget->SetVisibility(ESlateVisibility::Hidden);
		bInventoryOpened = false;
		
		if (PlayerController)
		{
			PlayerController->SetPause(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}
		
		UE_LOG(LogInventory, Display, TEXT("Inventory Closed"));
	}
}

void UInventoryComponent::ShowStorageItems(UItemDataAsset* ItemData)
{
	UE_LOG(LogInventory, Display, TEXT("New Item: %s"), *ItemData->ItemName.ToString());
	UE_LOG(LogInventory, Display, TEXT("Currently registered items in the inventory: %d"), (int)InventoryItems.Num());
	
	for (UItemDataAsset* Item : InventoryItems)
	{
		UE_LOG(LogInventory, Display, TEXT("%s"), *Item->ItemName.ToString());
	}
}

void UInventoryComponent::RegisterItem(UItemDataAsset* ItemData, UItemSlot* ItemSlot)
{
	if (ItemData->ItemType->IsA(UEquipmentItem::StaticClass()))
	{
		if (!EquippedItems.Contains(ItemData))
		{
			EquippedItems.Add(ItemData);
			UE_LOG(LogInventory, Display, TEXT("Equipment item registered and activated, current number equipment items: %d"), (int)EquippedItems.Num());
		}
		UEquipmentItem* EquippedItem = Cast<UEquipmentItem>(ItemData->ItemType);
		PlayerReference->EvaluateModifiers();
	}
	
	if (ItemData->ItemType->IsA(UActiveSkillItem::StaticClass()))
	{
		if (!EquippedActiveItems.Contains(ItemData))
		{
			UActiveSkillItem* ActiveSkillDescriptor = Cast<UActiveSkillItem>(ItemData->ItemType);
				
			ItemSlot->ItemData->Component = PlayerReference->AddComponentByClass(ActiveSkillDescriptor->ActiveSkillComponent, false, FTransform(), false);
			EquippedActiveItems.Add(ItemData);
				
			UE_LOG(LogInventory, Display, TEXT("Active Item registered and activated, current number of active items in inventory: %d"), (int)EquippedActiveItems.Num());
		}
	}
}

void UInventoryComponent::RemoveItem(UItemSlot* ItemSlot)
{
	if (ItemSlot->ItemData->ItemType->IsA(UEquipmentItem::StaticClass()))
	{
		EquippedItems.Remove(ItemSlot->ItemData);
		UE_LOG(LogInventory, Display, TEXT("Equipment item unregistered, current number of active items in inventory: %d"), (int)EquippedActiveItems.Num());
		UEquipmentItem* EquippedItem = Cast<UEquipmentItem>(ItemSlot->ItemData->ItemType);
		EquippedItem->Effect->RemoveEffect(PlayerReference);
		return;
	}
	
	if (ItemSlot->ItemData->ItemType->IsA(UActiveSkillItem::StaticClass()))
	{
		EquippedActiveItems.Remove(ItemSlot->ItemData);
		ItemSlot->ItemData->Component->DestroyComponent();
		
		UE_LOG(LogInventory, Display, TEXT("Active Item unregistered, current number of active items in inventory: %d"), (int)EquippedActiveItems.Num());
	}
}
