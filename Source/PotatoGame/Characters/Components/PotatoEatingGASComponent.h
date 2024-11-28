// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotatoGame/Characters/Components/PotatoEatingBaseComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameplayAbilitySpecHandle.h"

#include "PotatoEatingGASComponent.generated.h"

class APotato;
class UPotatoEatingAbility;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class POTATOGAME_API UPotatoEatingGASComponent : public UPotatoEatingBaseComponent
{
	GENERATED_BODY()

public:
	APotato* GetTargetPotato() { return _targetPotato; }
	
private:
	virtual void Activate(bool reset) override;
	virtual void Deactivate() override;

	virtual void Authority_EatPotato(APotato* potato) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UPotatoEatingAbility> _potatoEatingAbility;

	UPROPERTY(Transient)
	APotato* _targetPotato;

	FGameplayAbilitySpecHandle _abilityHandle;
};
