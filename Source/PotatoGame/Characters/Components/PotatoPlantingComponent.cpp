#include "PotatoPlantingComponent.h"

#include "PotatoGame/AbilitySystem/Abilities/PotatoPlantAbility.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/PotatoLogs.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

//extern TAutoConsoleVariable<float> CVarPotatoPlantRate;

void UPotatoPlantingComponent::Authority_PlantPotato()
{
	if (CanPlantPotato())
	{
		ACharacter* character = GetOwner<ACharacter>();
		if (IsValid(character))
		{
			bool success = UPotatoPlantAbility::Authority_PlantPotato(character, _spawnSocketName, _spawnVelocity);
			if (success)
			{
				//const float effectivePlantRate = CVarPotatoPlantRate.GetValueOnGameThread() > 0? CVarPotatoPlantRate.GetValueOnGameThread() : _plantingRate;
				const float effectivePlantRate = _plantingRate;

				UGameplayTagComponent* gameplayTagComponent = character->GetComponentByClass<UGameplayTagComponent>();
				if (ensure(IsValid(gameplayTagComponent)))
				{
					gameplayTagComponent->Authority_AddTag(GameplayTag_Ability_PotatoPlanting_Cooldown, effectivePlantRate);
				}
			}
		}
	}
}

