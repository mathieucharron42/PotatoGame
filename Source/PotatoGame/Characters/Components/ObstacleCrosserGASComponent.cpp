#include "ObstacleCrosserGASComponent.h"

#include "PotatoGame/AbilitySystem/Abilities/ObstacleCrosserAbility.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

void UObstacleCrosserGASComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			FGameplayAbilitySpec abilitySpec(_obstacleCrosserAbility);
			_abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			ensure(_abilityHandle.IsValid());
		}
	}
}

void UObstacleCrosserGASComponent::Deactivate()
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			abilitySystemComponent->ClearAbility(_abilityHandle);
		}
	}
}

void UObstacleCrosserGASComponent::Authority_ActivateObstacleCrossing()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (ensure(IsValid(owner)))
	{
		if (ensure(owner->HasAuthority()))
		{
			UAbilitySystemComponent* abilitySystemComponent = owner->GetComponentByClass<UAbilitySystemComponent>();
			if (ensure(IsValid(abilitySystemComponent)))
			{
				abilitySystemComponent->TryActivateAbility(_abilityHandle);
			}
		}
	}
}