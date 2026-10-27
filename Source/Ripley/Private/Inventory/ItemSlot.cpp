// Project R.I.P.LEY - Szpont Studios


#include "Inventory/ItemSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "Inventory/InventoryDragWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Player/RipleyCharacter.h"


FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return ReplyResult.NativeReply;
	}
	return FReply::Unhandled();
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	//if we don't have item data - don't handle drag
	if (!ItemData)
	{
		return;
	}

	UInventoryDragWidget* CreatedDragWidget = CreateWidget<UInventoryDragWidget>(this, DragWidgetClass);
	CreatedDragWidget->SetImage(ItemData->ItemImage);
	CreatedDragWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	UInventoryDragDropOperation* DragDropOperation = NewObject<UInventoryDragDropOperation>();
	
	DragDropOperation->DefaultDragVisual = CreatedDragWidget;
	DragDropOperation->DraggedItemData = ItemData;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->SourceItemSlot = this; 
	OutOperation = DragDropOperation;
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	ARipleyCharacter* PlayerReference = Cast<ARipleyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (UInventoryDragDropOperation* DragDropOperation = Cast<UInventoryDragDropOperation>(InOperation))
	{
		//if target slot already has an item, don't handle the drop operation
		if (HasItem())
		{
			return false;
		}
		//do not handle drop if item type does not match item slot type
		if (SlotType == EItemSlotType::ActiveSkill)
		{
			
			if (GEngine && DragDropOperation->DraggedItemData->ItemType == nullptr || DragDropOperation->DraggedItemData->ItemType->GetClass() != UActiveSkillItem::StaticClass())
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red, "Dragged item is not an Active Skill item");
				return false;
			}
			
			AddItem(DragDropOperation->DraggedItemData);
			DragDropOperation->SourceItemSlot->RemoveItem();
			PlayerReference->GetInventoryComponent()->OnItemRegister.Broadcast(DragDropOperation->DraggedItemData, this);
		}
		
		if (SlotType == EItemSlotType::Equipment)
		{
			if (GEngine && (DragDropOperation->DraggedItemData->ItemType == nullptr || DragDropOperation->DraggedItemData->ItemType->GetClass() != UEquipmentItem::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3, FColor::Red, "Dragged item is not a valid equipment item");
				return false;
			}
			
			AddItem(DragDropOperation->DraggedItemData);
			DragDropOperation->SourceItemSlot->RemoveItem();
			PlayerReference->GetInventoryComponent()->OnItemRegister.Broadcast(DragDropOperation->DraggedItemData, this);
		}
		
		if (SlotType == EItemSlotType::Storage)
		{
			if (DragDropOperation->SourceItemSlot->SlotType == EItemSlotType::ActiveSkill || DragDropOperation->SourceItemSlot->SlotType == EItemSlotType::Equipment)
			{
				PlayerReference->GetInventoryComponent()->OnItemUnregister.Broadcast(DragDropOperation->SourceItemSlot);
			}
			
			AddItem(DragDropOperation->DraggedItemData);
			DragDropOperation->SourceItemSlot->RemoveItem();
		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UItemSlot::AddItem(UItemDataAsset* Item)
{
	ItemData = Item;
	ItemImage->SetBrushFromTexture(ItemData->ItemImage);
}

void UItemSlot::RemoveItem()
{
	ItemImage->SetBrushFromTexture(nullptr);
	ItemData = nullptr;
}

bool UItemSlot::HasItem()
{
	if (ItemData)
	{
		return true;
	}
	return false;
}