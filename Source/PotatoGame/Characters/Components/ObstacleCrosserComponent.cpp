#include "ObstacleCrosserComponent.h"

#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

void UObstacleCrosserComponent::Authority_ActivateObstacleCrossing()
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_AddTag(_activationTag);
		}
	}
}