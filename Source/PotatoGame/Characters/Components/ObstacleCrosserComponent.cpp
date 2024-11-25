#include "ObstacleCrosserComponent.h"

#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Components/CapsuleComponent.h"

UObstacleCrosserComponent::UObstacleCrosserComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

void UObstacleCrosserComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		_tagChangedHandle = gameplayTagComponent->RegisterAndNotifyTagChange(FGameplayTagChanged::FDelegate::CreateUObject(this, &UObstacleCrosserComponent::OnGameplayTagChanged));
	}
}

void UObstacleCrosserComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		gameplayTagComponent->UnregisterTagChange(_tagChangedHandle);
	}
}

void UObstacleCrosserComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_AddTag(Character_Behaviour_PotatoObstacleCrossingCapable);
		}
	}
}

void UObstacleCrosserComponent::Deactivate() 
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_RemoveTag(Character_Behaviour_PotatoObstacleCrossingCapable);
		}
	}
}

void UObstacleCrosserComponent::OnGameplayTagChanged(FGameplayTag tag, bool added)
{
	if (tag == Character_Behaviour_PotatoObstacleCrossingCapable)
	{
		const bool obastacleCrossingEnabled = added;
		UpdateObastacleCrossing(obastacleCrossingEnabled);
	}
}

void UObstacleCrosserComponent::UpdateObastacleCrossing(bool enabled)
{
	AActor* owner = GetOwner();
	if (IsValid(owner))
	{
		UPrimitiveComponent* primitiveComponent = Cast<UPrimitiveComponent>(owner->GetRootComponent());
		if (IsValid(primitiveComponent))
		{
			ECollisionResponse response = enabled ? ECollisionResponse::ECR_Ignore : ECollisionResponse::ECR_Block;
			primitiveComponent->SetCollisionResponseToChannel(_allowedCollisionChannel, response);
		}
	}
}