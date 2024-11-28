#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/PotatoGameplayTags.h"

#include "Abilities/GameplayAbility.h"


#include "PotatoEatingAbility.generated.h"

class ACharacter;
class APotato;

UCLASS()
class POTATOGAME_API UPotatoEatingAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	static float Authority_EatPotato(AActor* actor, APotato* potato);
	
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
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData);
};