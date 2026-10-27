// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Gameplay/Health/HealthComponent.h"
#include "UI/HealthBar.h"
#include "PlayerUI.generated.h"


UCLASS()
class RIPLEY_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	UHealthComponent* HealthComponent;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* AmmoCounter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UHealthBar* HealthBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* InteractionPrompt;
	
	UFUNCTION(BlueprintCallable)
	void UpdateAmmoCounter(int NewAmmoValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateUI();
	
	UFUNCTION(BlueprintCallable)
	void SetInteractionPromptVisibility(bool Visible);
};
