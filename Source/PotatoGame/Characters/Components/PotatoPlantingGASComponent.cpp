#include "PotatoPlantingGASComponent.h"

#include "PotatoGame/AbilitySystem/Abilities/PotatoPlantAbility.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

void UPotatoPlantingGASComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			FGameplayAbilitySpec abilitySpec(_plantingAbility);
			_abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			ensure(_abilityHandle.IsValid());
		}
	}
}

void UPotatoPlantingGASComponent::Deactivate()
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

void UPotatoPlantingGASComponent::Authority_PlantPotato()
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