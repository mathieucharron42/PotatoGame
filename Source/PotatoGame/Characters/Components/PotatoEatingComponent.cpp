#include "PotatoEatingComponent.h"

#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Characters/Components/PotatoPickUpComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/Utils/PotatoUtilities.h"
#include "PotatoGame/AbilitySystem/Abilities/PotatoEatingAbility.h"

#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

void UPotatoEatingComponent::Authority_EatPotato(APotato* potato)
{
	AActor* owner = GetOwner();
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		float newCalories = UPotatoEatingAbility::Authority_EatPotato(owner, potato);
		IncrementCaloriesEaten(newCalories);
	}
}