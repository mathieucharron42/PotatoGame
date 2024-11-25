#include "GameplayTagComponent.h"

#include "PotatoGame/Utils/PotatoUtilities.h"

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

FDelegateHandle UGameplayTagComponent::RegisterTagChange(const FGameplayTagChanged::FDelegate& delegate)
{
	return GameplayTagChanged.Add(delegate);
}

FDelegateHandle UGameplayTagComponent::RegisterAndNotifyTagChange(const FGameplayTagChanged::FDelegate& delegate)
{
	FDelegateHandle handle = RegisterTagChange(delegate);
	NotifyTags(true);
	return handle;
}

void UGameplayTagComponent::UnregisterTagChange(FDelegateHandle handle)
{
	GameplayTagChanged.Remove(handle);
}

void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();
	NotifyTags(true);
}

void UGameplayTagComponent::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);
	NotifyTags(false);
}

void UGameplayTagComponent::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = _gameplayTagContainer;
}

void UGameplayTagComponent::Authority_AddTag(FGameplayTag tag)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		bool added = _gameplayTagContainer.HasTagExact(tag);
		_gameplayTagContainer.AddTag(tag);
		if (added)
		{
			GameplayTagChanged.Broadcast(tag, true);
		}
	}
}

void UGameplayTagComponent::Authority_AddTag(FGameplayTag tag, float expiration)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		Authority_AddTag(tag);
		if (ensure(GetWorld()))
		{
			FTimerHandle handle;
		
			FTimerDelegate callback = FTimerDelegate::CreateUObject(this, &UGameplayTagComponent::Authority_OnTagExpired, tag);
			if (expiration > 0)
			{
				GetWorld()->GetTimerManager().SetTimer(handle, callback, expiration, false);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimerForNextTick(callback);
			}
			_tagExpirations.Add(tag, handle);
		}
	}
}

void UGameplayTagComponent::Authority_RemoveTag(FGameplayTag tag)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		bool removed = _gameplayTagContainer.RemoveTag(tag);
		if (removed)
		{
			GameplayTagChanged.Broadcast(tag, false);
		}
	}
}

bool UGameplayTagComponent::HasTag(FGameplayTag tag) const
{
	return _gameplayTagContainer.HasTag(tag);
}

void UGameplayTagComponent::Authority_OnTagExpired(FGameplayTag tag)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		_tagExpirations.Remove(tag);
		Authority_RemoveTag(tag);
	}
}

void UGameplayTagComponent::OnReplication_GameplayTagContainer(FGameplayTagContainer old)
{
	TArray<FGameplayTag> oldTags = old.GetGameplayTagArray();
	TArray<FGameplayTag> newTags = _gameplayTagContainer.GetGameplayTagArray();

	for (FGameplayTag oldTag : oldTags)
	{
		if (!newTags.Contains(oldTag))
		{
			GameplayTagChanged.Broadcast(oldTag, false);
		}
	}

	for (FGameplayTag newTag : newTags)
	{
		if (!oldTags.Contains(newTag))
		{
			GameplayTagChanged.Broadcast(newTag, true);
		}
	}
}

void UGameplayTagComponent::NotifyTags(bool added)
{
	for (FGameplayTag tag : GetContainer())
	{
		GameplayTagChanged.Broadcast(tag, added);
	}
}