#include "ObstacleCrosserComponent.h"

#include "PotatoGame/PotatoGame.h"
#include "PotatoGame/Gameplay/GameplayTagComponent.h"

#include "Components/CapsuleComponent.h"

UObstacleCrosserComponent::UObstacleCrosserComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UObstacleCrosserComponent::BeginPlay()
{
	Super::BeginPlay();
	AllowObastacleCrossing();
}

void UObstacleCrosserComponent::AllowObastacleCrossing()
{
	AActor* owner = GetOwner();
	if (IsValid(owner))
	{
		UPrimitiveComponent* primitiveComponent = Cast<UPrimitiveComponent>(owner->GetRootComponent());
		if (IsValid(primitiveComponent))
		{
			primitiveComponent->SetCollisionResponseToChannel(_allowedCollisionChannel, ECollisionResponse::ECR_Ignore);
		}

		UGameplayTagComponent* tagsComponent = owner->GetComponentByClass<UGameplayTagComponent>();
		if (ensure(IsValid(tagsComponent)))
		{
			tagsComponent->GetContainer().AddTag(Character_Behaviour_PotatoObstacleCrossing);
		}
	}
}