#include "ObstacleCrosserAbility.h"


#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffect.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

bool UObstacleCrosserAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, 
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UObstacleCrosserAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Authority_ActivateAbility(Handle, ActorInfo, ActivationInfo);
}

void UObstacleCrosserAbility::Authority_ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ensure(HasAuthority(&ActivationInfo)))
	{
		bool success = false;
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, _effect.GetDefaultObject(), 1);
			success = true;
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, success, false);
	}
}