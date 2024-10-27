#include "GameplayTagComponent.h"

UGameplayTagComponent::UGameplayTagComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UGameplayTagComponent::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = _gameplayTagContainer;
}