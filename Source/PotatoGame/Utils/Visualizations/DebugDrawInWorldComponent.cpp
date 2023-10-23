#include "DebugDrawInWorldComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"

static TAutoConsoleVariable<int32> CVarEnableDebugDrawInWorld(
	TEXT("potato.DebugDrawInWorld"),
	0,
	TEXT("Enables debug draw in world components")
);

UDebugDrawInWorldComponent::UDebugDrawInWorldComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDebugDrawInWorldComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDebugDrawInWorldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CVarEnableDebugDrawInWorld.GetValueOnGameThread())
	{
		UWorld* world = GetWorld();
		if (IsValid(world))
		{
			const float DepthRenderingPriority = 100.f;

			FVector position = GetComponentTransform().GetLocation();
			FQuat rotation = GetComponentTransform().GetRotation();
			AActor* parent = GetOwner();
			FVector origin;
			FVector parentSize;
			parent->GetActorBounds(true, origin, parentSize);

			if (_pointEnabled)
			{
				DrawDebugPoint(world, position, _pointProperties.Size, _pointProperties.Color, false, _pointProperties.Lifetime, DepthRenderingPriority);
			}

			if (_boxEnabled)
			{
				FQuat boxRotation = _boxProperties.InheritRotation ? rotation : FQuat::Identity;
				DrawDebugBox(world, position, _boxProperties.Size, boxRotation, _boxProperties.Color, false, _boxProperties.Lifetime, DepthRenderingPriority, _boxProperties.Thickness);
			}

			if (_sphereEnabled)
			{
				DrawDebugSphere(world, position, _sphereProperties.Radius, _sphereProperties.NbSegments, _sphereProperties.Color, false, _sphereProperties.Lifetime, DepthRenderingPriority, _sphereProperties.Thickness);
			}

			if (_arrowEnabled)
			{		
				FVector begin = position;
				FVector direction = _arrowProperties.InheritRotation ? rotation.RotateVector(_arrowProperties.Direction) : _arrowProperties.Direction;
				FVector end = position + (direction.GetSafeNormal() * _arrowProperties.Lenght);
				DrawDebugDirectionalArrow(world, position, end, _arrowProperties.Size, _arrowProperties.Color, false, _arrowProperties.Lifetime, DepthRenderingPriority, _arrowProperties.Thickness);
			}

			if (_textEnabled)
			{
				FVector textPosition = position + (_textProperties.InheritRotation ? rotation.RotateVector(_textProperties.Position) : _textProperties.Position);

				DrawDebugString(world, textPosition, _textProperties.Text, nullptr, _textProperties.Color, _textProperties.Lifetime, true, _textProperties.Size);
			}
		}
	}
}

