#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "PotatoPlantingBaseComponent.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPotatoPlantingBaseComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_PlantPotato();

	UFUNCTION(BlueprintCallable)
	bool CanPlantPotato() const;

protected:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Authority_PlantPotato() PURE_VIRTUAL(UPotatoPlantingBaseComponent::Authority_PlantPotato);

private:
	void SetupPlayerInput(UInputComponent* inputComponent);
};
