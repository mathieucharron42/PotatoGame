#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/PotatoGameplayTags.h"

#include "Abilities/GameplayAbility.h"

#include "ObstacleCrosserAbility.generated.h"

class ACharacter;

UCLASS()
class POTATOGAME_API UObstacleCrosserAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags, 
		const FGameplayTagContainer* TargetTags, 
		FGameplayTagContainer* OptionalRelevantTags) const override;
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	void Authority_ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> _effect;
};