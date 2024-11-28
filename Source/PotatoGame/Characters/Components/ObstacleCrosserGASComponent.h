#pragma once

#include "PotatoGame/Characters/Components/ObstacleCrosserBaseComponent.h"

#include "CoreMinimal.h"

#include "GameplayAbilitySpecHandle.h"

#include "ObstacleCrosserGASComponent.generated.h"

class UObstacleCrosserAbility;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UObstacleCrosserGASComponent : public UObstacleCrosserBaseComponent
{
	GENERATED_BODY()

public:
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

protected:
	virtual void Authority_ActivateObstacleCrossing() override;

	void Test(FGameplayTag tag, int32 count);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObstacleCrosserAbility> _obstacleCrosserAbility;

	FGameplayAbilitySpecHandle _abilityHandle;
};
