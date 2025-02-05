#include "GameplayTagComponent.h"

#include "PotatoGame/Utils/PotatoUtilities.h"

#include "AbilitySystemComponent.h"
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
	return _gameplayTagChanged.Add(delegate);
}

void UGameplayTagComponent::UnregisterTagChange(FDelegateHandle handle)
{
	_gameplayTagChanged.Remove(handle);
}

void UGameplayTagComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AActor* owner = GetOwner();
	if (ensure(IsValid(owner)))
	{
		// _abilitySystemComponent = owner->GetComponentByClass<UAbilitySystemComponent>();
		if (_abilitySystemComponent.IsValid())
		{
			_abilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &UGameplayTagComponent::OnAbilitySystemGameplayTagChanged);
		}
	}
}

void UGameplayTagComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
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
	if (_abilitySystemComponent.IsValid())
	{
		_abilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	}
	else
	{
		TagContainer = _gameplayTagContainer;
	}
}

void UGameplayTagComponent::Authority_AddTag(FGameplayTag tag)
{
	if (ensure(PotatoUtilities::HasAuthority(this)))
	{
		bool added = !HasTag(tag);

		if (_abilitySystemComponent.IsValid())
		{
			_abilitySystemComponent->AddLooseGameplayTag(tag);
			_abilitySystemComponent->AddReplicatedLooseGameplayTag(tag);
		}
		else
		{
			_gameplayTagContainer.AddTag(tag);
		}
		
		if (added)
		{
			_gameplayTagChanged.Broadcast(tag, true);
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
		bool removed = HasTag(tag);

		if (_abilitySystemComponent.IsValid())
		{
			_abilitySystemComponent->RemoveLooseGameplayTag(tag);
			_abilitySystemComponent->RemoveReplicatedLooseGameplayTag(tag);
		}
		else
		{
			_gameplayTagContainer.RemoveTag(tag);
		}

		if (removed)
		{
			_gameplayTagChanged.Broadcast(tag, false);
		}
	}
}

bool UGameplayTagComponent::HasTag(FGameplayTag tag) const
{
	return GetOwnedGameplayTags().HasTag(tag);
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
			_gameplayTagChanged.Broadcast(oldTag, false);
		}
	}

	for (FGameplayTag newTag : newTags)
	{
		if (!oldTags.Contains(newTag))
		{
			_gameplayTagChanged.Broadcast(newTag, true);
		}
	}
}

void UGameplayTagComponent::NotifyTags(bool added)
{
	auto tags = GetOwnedGameplayTags();
	for (FGameplayTag tag : tags)
	{
		_gameplayTagChanged.Broadcast(tag, added);
	}
}

FGameplayTagContainer UGameplayTagComponent::GetOwnedGameplayTags() const
{ 
	FGameplayTagContainer container;
	GetOwnedGameplayTags(container);
	return container;
}

void UGameplayTagComponent::OnAbilitySystemGameplayTagChanged(FGameplayTag tag, int32 count)
{
	const bool added = count > 0;
	_gameplayTagChanged.Broadcast(tag, added);
}