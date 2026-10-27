// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Inventory/InventoryComponent.h"
#include "InventoryUI.generated.h"


UCLASS()
class RIPLEY_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
public: 
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Settings")
	TSubclassOf<UUserWidget> InventorySlotWidgetClass;
	
	//equipment settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Settings")
	int EquipmentSlotsAmount = 3; 
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWrapBox> EquipmentSlotsBox;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UItemSlot*> EquipmentSlots;

	//storage settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Settings")
	int StorageSlotsAmount = 30;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWrapBox> StorageSlotsBox;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UItemSlot*> StorageSlots;
	
	//active skills UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Settings")
	int ActiveSkillSlotsAmount = 1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWrapBox> ActiveSkillSlotsBox;
	
	UPROPERTY()
	TArray<UItemSlot*> ActiveSkillSlots; 
	
	
	UFUNCTION()
	void UpdateStorage(UItemDataAsset* ItemData); 
	
	TArray<UItemSlot*> CreateSlots(int NumberOfSlots, EItemSlotType ItemSlotType, TSubclassOf<UUserWidget> SlotClass, TObjectPtr<UWrapBox> TargetWidget);
};
