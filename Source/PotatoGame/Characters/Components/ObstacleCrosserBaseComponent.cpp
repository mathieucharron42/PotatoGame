#include "ObstacleCrosserBaseComponent.h"

#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Components/CapsuleComponent.h"

UObstacleCrosserBaseComponent::UObstacleCrosserBaseComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	SetIsReplicatedByDefault(true);
}

void UObstacleCrosserBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		_tagChangedHandle = gameplayTagComponent->RegisterTagChange(FGameplayTagChanged::FDelegate::CreateUObject(this, &UObstacleCrosserBaseComponent::OnGameplayTagChanged));
	}
}

void UObstacleCrosserBaseComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		gameplayTagComponent->UnregisterTagChange(_tagChangedHandle);
	}
}

void UObstacleCrosserBaseComponent::Activate(bool reset)
{
	Super::Activate(reset);
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_AddTag(GameplayTag_Ability_PotatoObstacleCrossing_Capable);
		}
	}
}

void UObstacleCrosserBaseComponent::Deactivate()
{
	Super::Deactivate();
	if (PotatoUtilities::HasAuthority(this))
	{
		UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
		if (ensure(IsValid(gameplayTagComponent)))
		{
			gameplayTagComponent->Authority_RemoveTag(GameplayTag_Ability_PotatoObstacleCrossing_Capable);
		}
	}
}

void UObstacleCrosserBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	if (PotatoUtilities::HasAuthority(this))
	{
		Authority_ActivateObstacleCrossing();
	}
}

void UObstacleCrosserBaseComponent::OnGameplayTagChanged(FGameplayTag tag, bool added)
{
	if (tag == _activationTag)
	{
		OnGameplayTagObstacleCrossing(added);
	}
}

void UObstacleCrosserBaseComponent::OnGameplayTagObstacleCrossing(bool enabled)
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