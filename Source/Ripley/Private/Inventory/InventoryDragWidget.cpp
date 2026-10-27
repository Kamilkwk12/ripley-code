// Project R.I.P.LEY - Szpont Studios


#include "Inventory/InventoryDragWidget.h"

void UInventoryDragWidget::SetImage(UTexture2D* ItemImage)
{
	DraggedItemImage->SetBrushFromTexture(ItemImage);
}
