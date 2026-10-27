// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "InventoryDragWidget.generated.h"


UCLASS()
class RIPLEY_API UInventoryDragWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetImage(UTexture2D* ItemImage);
	
private:
	UPROPERTY(meta=(BindWidget))
	UImage* DraggedItemImage;
};
