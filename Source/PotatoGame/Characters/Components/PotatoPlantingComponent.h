#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/Characters/Components/PotatoPlantingBaseComponent.h"

#include "Components/SceneComponent.h"

#include "PotatoPlantingComponent.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingComponent : public UPotatoPlantingBaseComponent
{
	GENERATED_BODY()

public:	
	UPotatoPlantingComponent();

	virtual bool CanPlantPotato() const override;

private:
	virtual void Server_PlantPotato_Implementation() override;

	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	void Authority_PlantPotato();

	void OnSetupPlayerInput(UInputComponent* inputComponent);
};
