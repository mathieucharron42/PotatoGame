#pragma once

#include "CoreMinimal.h"

#include "PotatoGame/PotatoGameplayTags.h"

#include "Abilities/GameplayAbility.h"


#include "PotatoPlantAbility.generated.h"

class ACharacter;
class UPotatoPlantingAbilityData;

USTRUCT()
struct FPotatoPlantAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName SpawnSocketName = FName("socket_spawn");

	UPROPERTY(EditAnywhere)
	float SpawnVelocity = 10;

	UPROPERTY(EditAnywhere)
	float PlantingRate = 1;
};

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
	FGameplayTag _cooldownTag;

	UPROPERTY(EditAnywhere)
	FPotatoPlantAbilityData _data;
};