#include "ObstacleCrosserComponent.h"

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
	}
}