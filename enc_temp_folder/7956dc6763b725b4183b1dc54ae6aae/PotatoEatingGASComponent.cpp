#include "PotatoEatingGASComponent.h"

#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Utils/PotatoUtilities.h"
#include "PotatoGame/AbilitySystem/Abilities/PotatoEatingAbility.h"

#include "AbilitySystemComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

void UPotatoEatingGASComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UAbilitySystemComponent* abilitySystemComponent = PotatoUtilities::GetComponentByClass<UAbilitySystemComponent>(this);
		if (ensure(IsValid(abilitySystemComponent)))
		{
			FGameplayAbilitySpec abilitySpec(_potatoEatingAbility);
			_abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			ensure(_abilityHandle.IsValid());
		}
	}
}

void UPotatoEatingGASComponent::Deactivate()
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

void UPotatoEatingGASComponent::Authority_EatPotato(APotato* potato)
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