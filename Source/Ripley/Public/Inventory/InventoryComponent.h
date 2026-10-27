// Project R.I.P.ley - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "Item.h"
#include "ItemSlot.h"
#include "InventoryComponent.generated.h"

class ARipleyCharacter;
class UInventoryUI;

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryStateChanged, UItemDataAsset*, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAddedDelegate, UItemDataAsset*, ItemData, UItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedDelegate, UItemSlot*, ItemSlot);

UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class RIPLEY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnInventoryStateChanged OnInventoryStateChanged; 
	FOnItemAddedDelegate OnItemRegister;
	FOnItemRemovedDelegate OnItemUnregister;
	
	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	UPROPERTY()
	ARipleyCharacter* PlayerReference;
	
	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputActions")
	class UInputAction* OpenInventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	UPROPERTY()
	UInventoryUI* InventoryUIWidget;
	
	UPROPERTY()
	TArray<UItemDataAsset*> InventoryItems;
	
	UPROPERTY()
	TArray<UItemDataAsset*> EquippedItems;
	
	UPROPERTY()
	TArray<UItemDataAsset*> EquippedActiveItems;
	
	bool bInventoryOpened = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()	
	void OpenInventory(const FInputActionValue& Value);
	
	UFUNCTION()
	void ShowStorageItems(UItemDataAsset* ItemData);
	
	UFUNCTION()
	void RegisterItem(UItemDataAsset* ItemData, UItemSlot* ItemSlot);
	
	UFUNCTION()
	void RemoveItem(UItemSlot* ItemSlot);
	
};
