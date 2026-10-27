// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ItemSlot.generated.h"

UENUM()
enum class EItemSlotType
{
	Equipment, 
	Storage, 
	ActiveSkill
};

UCLASS(Blueprintable)
class RIPLEY_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	
	UPROPERTY(BlueprintReadOnly)
	UItemDataAsset* ItemData;
	
	UPROPERTY(BlueprintReadOnly)
	EItemSlotType SlotType;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ItemImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DragWidgetClass;
	
	
	UFUNCTION(BlueprintCallable)
	void AddItem(UItemDataAsset* Item);

	UFUNCTION(BlueprintCallable)
	void RemoveItem();
	
	bool HasItem();
	
};
