#include "ObstacleCrosserComponent.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"
#include "PotatoGame/PotatoGameplayTags.h"
#include "PotatoGame/Utils/PotatoUtilities.h"
#include "Components/CapsuleComponent.h"

UObstacleCrosserComponent::UObstacleCrosserComponent()
{
	_allowedCollisionChannel = ECC_GameTraceChannel1;
	_activationTag = GameplayTag_Ability_PotatoObstacleCrossing_Active;
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
		_tagChangedHandle = gameplayTagComponent->RegisterTagChange(FGameplayTagChanged::FDelegate::CreateUObject(this, &UObstacleCrosserComponent::OnGameplayTagChanged));
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
			gameplayTagComponent->Authority_AddTag(GameplayTag_Ability_PotatoObstacleCrossing_Capable);
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
			gameplayTagComponent->Authority_RemoveTag(GameplayTag_Ability_PotatoObstacleCrossing_Capable);
		}
	}
}

void UObstacleCrosserComponent::BeginPlay()
{
	Super::BeginPlay();
	if (PotatoUtilities::HasAuthority(this))
	{
		Authority_ActivateObstacleCrossing();
	}
}

void UObstacleCrosserComponent::OnGameplayTagChanged(FGameplayTag tag, bool added)
{
	if (tag == _activationTag)
	{
		OnGameplayTagObstacleCrossing(added);
	}
}

void UObstacleCrosserComponent::OnGameplayTagObstacleCrossing(bool enabled)
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
