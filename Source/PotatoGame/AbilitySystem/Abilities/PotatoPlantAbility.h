#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "PotatoPlantAbility.generated.h"

class ACharacter;

UCLASS()
class POTATOGAME_API UPotatoPlantAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	static bool Authority_PlantPotato(ACharacter* character, FName spawnSocketName, float spawnVelocity);

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

	UPROPERTY(EditAnywhere)
	FName _spawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float _spawnVelocity;

	UPROPERTY(EditAnywhere)
	float _plantingRate;
};