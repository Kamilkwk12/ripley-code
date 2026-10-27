// Project R.I.P.LEY - Szpont Studios


#include "Inventory/InventoryUI.h"
#include "Player/RipleyCharacter.h"


void UInventoryUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	EquipmentSlots = CreateSlots(EquipmentSlotsAmount, EItemSlotType::Equipment, InventorySlotWidgetClass, EquipmentSlotsBox);
	
	StorageSlots = CreateSlots(StorageSlotsAmount, EItemSlotType::Storage, InventorySlotWidgetClass, StorageSlotsBox);
	
	ActiveSkillSlots = CreateSlots(ActiveSkillSlotsAmount, EItemSlotType::ActiveSkill, InventorySlotWidgetClass, ActiveSkillSlotsBox);
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	const ARipleyCharacter* PlayerReference = Cast<ARipleyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	InventoryComponent = PlayerReference->GetInventoryComponent();
	InventoryComponent->OnInventoryStateChanged.AddDynamic(this, &UInventoryUI::UpdateStorage);
}
//TODO: rewrite the UpdateStorage call 
void UInventoryUI::UpdateStorage(UItemDataAsset* ItemData)
{
	UE_LOG(LogInventory, Log, TEXT("UI Update Storage"));
	
	for (UItemDataAsset* Item : InventoryComponent->InventoryItems)
	{
		for (UItemSlot* ItemSlot : EquipmentSlots)
		{
			if (ItemSlot->ItemData == Item)
			{
				goto continue2;
			}
		}
		
		for (UItemSlot* ItemSlot : ActiveSkillSlots)
		{
			if (ItemSlot->ItemData == Item)
			{
				goto continue2;
			}
		}
		//if not, we are finding first free slot for it
		for (int i = 0; i < StorageSlotsAmount; i++)
		{
			if (StorageSlots[i]->HasItem() && StorageSlots[i]->ItemData == Item)
			{
				break;
			}
			if (!StorageSlots[i]->HasItem())
			{
				StorageSlots[i]->AddItem(Item);
				break;
			}
		}
		continue2:;
	}
}

TArray<UItemSlot*> UInventoryUI::CreateSlots(int NumberOfSlots, EItemSlotType ItemSlotType, TSubclassOf<UUserWidget> SlotClass, TObjectPtr<UWrapBox> TargetWidget)
{
	if (TargetWidget == nullptr)
	{
		UE_LOG(LogConfig, Error, TEXT("%s - UI Widget has no properly bind WrapBoxWidget to create Inventory Slots"), *this->GetName());
		return TArray<UItemSlot*>();
	}
	
	TArray<UItemSlot*> Slots;
	
	TargetWidget->ClearChildren();
	for (int i = 0; i < NumberOfSlots; i++)
	{
		if (UItemSlot* ItemSlot = CreateWidget<UItemSlot>(this, SlotClass))
		{
			ItemSlot->SlotType = ItemSlotType;
			TargetWidget->AddChildToWrapBox(ItemSlot);
			Slots.Add(ItemSlot);
		}
	}
	
	return Slots;
}
