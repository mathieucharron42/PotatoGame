#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/Characters/Components/PotatoPlantingBaseComponent.h"

#include "GameplayAbilitySpecHandle.h"

#include "PotatoPlantingGASComponent.generated.h"

class UPotatoPlantAbility;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoPlantingGASComponent : public UPotatoPlantingBaseComponent
{
	GENERATED_BODY()

public:	

private:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void Authority_PlantPotato() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoPlantAbility> _plantingAbility;

	FGameplayAbilitySpecHandle _abilityHandle;
};
