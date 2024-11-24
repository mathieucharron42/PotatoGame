#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameplayAbilitySpecHandle.h"

#include "PotatoPlantingGASComponent.generated.h"

class UPotatoPlantAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingGASComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPotatoPlantingGASComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_PlantPotato();

	UFUNCTION(BlueprintCallable)
	bool CanPlantPotato() const;

private:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	void Authority_PlantPotato();

	void OnSetupPlayerInput(UInputComponent* inputComponent);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPlantAbility> _plantingAbility;

	FGameplayAbilitySpecHandle _abilityHandle;
};
