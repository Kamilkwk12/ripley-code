// Project R.I.P.LEY - Szpont Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathScreen.generated.h"

UCLASS()
class RIPLEY_API UDeathScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* RestartButton;

private: 
	UFUNCTION()
	void Restart(); 
	
};
