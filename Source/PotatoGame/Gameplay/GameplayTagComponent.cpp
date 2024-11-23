#include "GameplayTagComponent.h"

#include "Net/UnrealNetwork.h"

UGameplayTagComponent::UGameplayTagComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UGameplayTagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameplayTagComponent, _gameplayTagContainer);
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
	bool added = _gameplayTagContainer.HasTagExact(tag);
	_gameplayTagContainer.AddTag(tag);
	if (added)
	{
		GameplayTagChanged.Broadcast(tag, true);
	}
}

void UGameplayTagComponent::AddTag(FGameplayTag tag, float expiration)
{
	AddTag(tag);
	if (ensure(GetWorld()))
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateUObject(this,& UGameplayTagComponent::OnTagExpired, tag), expiration, false);
		_tagExpirations.Add(tag, handle);
	}
}

void UGameplayTagComponent::RemoveTag(FGameplayTag tag)
{
	bool removed = _gameplayTagContainer.RemoveTag(tag);
	if (removed)
	{
		GameplayTagChanged.Broadcast(tag, false);
	}
}

bool UGameplayTagComponent::HasTag(FGameplayTag tag) const
{
	return _gameplayTagContainer.HasTag(tag);
}

void UGameplayTagComponent::OnTagExpired(FGameplayTag tag)
{
	_tagExpirations.Remove(tag);
	RemoveTag(tag);
}