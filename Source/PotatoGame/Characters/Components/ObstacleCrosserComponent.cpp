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
		gameplayTagComponent->GameplayTagChanged.AddUObject(this, &UObstacleCrosserComponent::OnGameplayTagChanged);
	}
}

void UObstacleCrosserComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (IsValid(gameplayTagComponent))
	{
		gameplayTagComponent->GameplayTagChanged.RemoveAll(this);
	}
}

void UObstacleCrosserComponent::Activate(bool reset)
{
	Super::Activate(reset);
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (ensure(IsValid(gameplayTagComponent)))
	{
		gameplayTagComponent->AddTag(Character_Behaviour_PotatoObstacleCrossing);
	}
}

void UObstacleCrosserComponent::Deactivate() 
{
	Super::Deactivate();
	UGameplayTagComponent* gameplayTagComponent = PotatoUtilities::GetComponentByClass<UGameplayTagComponent>(this);
	if (ensure(IsValid(gameplayTagComponent)))
	{
		gameplayTagComponent->RemoveTag(Character_Behaviour_PotatoObstacleCrossing);
	}
}

void UObstacleCrosserComponent::OnGameplayTagChanged(FGameplayTag tag, bool added)
{
	if (tag == Character_Behaviour_PotatoObstacleCrossing)
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