#include "PotatoEatingAbility.h"

#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Utils/PotatoUtilities.h"
#include "PotatoGame/Characters/Components/PotatoEatingGASComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffect.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

bool UPotatoEatingAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, 
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPotatoEatingAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Authority_ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPotatoEatingAbility::Authority_ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ensure(HasAuthority(&ActivationInfo)))
	{
		bool success = false;
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			TWeakObjectPtr<ACharacter> owner = Cast<ACharacter>(ActorInfo->AvatarActor);
			if (ensure(owner.IsValid()))
			{
				UPotatoEatingGASComponent* potatoEatingComponent = owner->GetComponentByClass<UPotatoEatingGASComponent>();
				if (ensure(IsValid(potatoEatingComponent)))
				{
					APotato* potato = potatoEatingComponent->GetTargetPotato();
					
					float calories = Authority_EatPotato(owner.Get(), potato);
					potatoEatingComponent->IncrementCaloriesEaten(calories);

					success = calories > 0;
				}
			}
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, success, false);
	}
}

float UPotatoEatingAbility::Authority_EatPotato(AActor* owner, APotato* potato)
{
	float newCalories = 0;
	if (ensure(PotatoUtilities::HasAuthority(owner)))
	{
		if (ensure(IsValid(potato)))
		{
			const FNutritionalInformations& nutritionalInformations = potato->GetNutritionalInformations();
			newCalories = nutritionalInformations.GetCalories(potato->GetWeight());

			potato->Destroy();

			UE_LOG(LogPotato, Log, TEXT("Ate potato %s by %s at %s"), *potato->GetName(), *owner->GetName(), *owner->GetTransform().ToString());
		}
	}
	return newCalories;
}