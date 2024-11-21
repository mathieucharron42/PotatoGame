#include "GameplayTagComponent.h"

UGameplayTagComponent::UGameplayTagComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();

	for (FGameplayTag tag : GetContainer())
	{
		GameplayTagChanged.Broadcast(tag, true);
	}
}

void UGameplayTagComponent::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	for (FGameplayTag tag : GetContainer())
	{
		GameplayTagChanged.Broadcast(tag, false);
	}
}

void UGameplayTagComponent::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = _gameplayTagContainer;
}

void UGameplayTagComponent::AddTag(FGameplayTag tag)
{
	_gameplayTagContainer.AddTag(tag);
	GameplayTagChanged.Broadcast(tag, true);
}

void UGameplayTagComponent::RemoveTag(FGameplayTag tag)
{
	_gameplayTagContainer.RemoveTag(tag);
	GameplayTagChanged.Broadcast(tag, false);
}