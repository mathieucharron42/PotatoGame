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

void UDebugDrawInWorldComponent::EnableDrawPoint(const FDrawInWorldPointProperties& properties)
{
	_pointEnabled = true;
	_pointProperties = properties;
}

void UDebugDrawInWorldComponent::EnableDrawSphere(const FDrawInWorldSphereProperties& properties)
{
	_sphereEnabled = true;
	_sphereProperties = properties;
}

void UDebugDrawInWorldComponent::EnableDrawBox(const FDrawInWorldBoxProperties& properties)
{
	_boxEnabled = true;
	_boxProperties = properties;
}

void UDebugDrawInWorldComponent::EnableDrawArrow(const FDrawInWorldArrowProperties& properties)
{
	_arrowEnabled = true;
	_arrowProperties = properties;
}

void UDebugDrawInWorldComponent::EnableDrawText(const FDrawInWorldTextProperties& properties)
{
	_textEnabled = true;
	_textProperties = properties;
}

void UDebugDrawInWorldComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDebugDrawInWorldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		const uint8 DepthRenderingPriority = 0;

		FVector position = GetComponentTransform().GetLocation();
		FQuat rotation = GetComponentTransform().GetRotation();
		AActor* parent = GetOwner();
		FVector origin;
		FVector parentSize;
		parent->GetActorBounds(true, origin, parentSize);

		if (ShouldDraw(_pointEnabled, _hasDrawPoint, _pointProperties.DrawRate))
		{
			DrawDebugPoint(world, position, _pointProperties.Size, _pointProperties.Color, false, _pointProperties.Lifetime, DepthRenderingPriority);
			_hasDrawPoint = true;
		}

		if (ShouldDraw(_boxEnabled, _hasDrawnBox, _boxProperties.DrawRate))
		{
			FQuat boxRotation = _boxProperties.InheritRotation ? rotation : FQuat::Identity;
			DrawDebugBox(world, position, _boxProperties.Size, boxRotation, _boxProperties.Color, false, _boxProperties.Lifetime, DepthRenderingPriority, _boxProperties.Thickness);
			_hasDrawnBox = true;
		}

		if (ShouldDraw(_sphereEnabled, _hasDrawnSphere, _sphereProperties.DrawRate))
		{
			DrawDebugSphere(world, position, _sphereProperties.Radius, _sphereProperties.NbSegments, _sphereProperties.Color, false, _sphereProperties.Lifetime, DepthRenderingPriority, _sphereProperties.Thickness);
			_hasDrawnSphere = true;
		}

		if (ShouldDraw(_arrowEnabled, _hasDrawnArrow, _arrowProperties.DrawRate))
		{		
			FVector begin = position;
			FVector direction = _arrowProperties.InheritRotation ? rotation.RotateVector(_arrowProperties.Direction) : _arrowProperties.Direction;
			FVector end = position + (direction.GetSafeNormal() * _arrowProperties.Lenght);
			DrawDebugDirectionalArrow(world, position, end, _arrowProperties.Size, _arrowProperties.Color, false, _arrowProperties.Lifetime, DepthRenderingPriority, _arrowProperties.Thickness);
			_hasDrawnArrow = true;
		}

		if (ShouldDraw(_textEnabled, _hasDrawnText, _textProperties.DrawRate))
		{
			FVector textPosition = position + (_textProperties.InheritRotation ? rotation.RotateVector(_textProperties.Position) : _textProperties.Position);

			DrawDebugString(world, textPosition, _textProperties.Text, nullptr, _textProperties.Color, _textProperties.Lifetime, true, _textProperties.Size);
			_hasDrawnText = true;
		}
	}

	if (_selfDestructDelay > 0)
	{
		if (_selfDestructTime.IsSet())
		{
			// Countdown started
			_selfDestructTime = _selfDestructTime.GetValue() - DeltaTime;
		}
		else
		{
			// Starting countdown
			_selfDestructTime = _selfDestructDelay;
		}

		if (_selfDestructTime.GetValue() <= 0)
		{
			if (_selfDestructType == ESelfDestructType::Actor)
			{
				AActor* actor = GetOwner();
				if (ensure(actor))
				{
					actor->Destroy();
				}
			}
			else if (_selfDestructType == ESelfDestructType::Component)
			{
				DestroyComponent();
			}
		}
	}
}

void UDebugDrawInWorldComponent::EnableSelfDestructActor(float delay)
{
	_selfDestructType = ESelfDestructType::Actor;
	_selfDestructDelay = delay;
	_selfDestructTime.Reset();
}

void UDebugDrawInWorldComponent::EnableSelfDestructComponent(float delay)
{
	_selfDestructType = ESelfDestructType::Component;
	_selfDestructDelay = delay;
	_selfDestructTime.Reset();
}

void UDebugDrawInWorldComponent::ForceSelfDestruct()
{
	if (_selfDestructType == ESelfDestructType::None)
	{
		// Assume we want to destroy component if no self destruct was in progress
		EnableSelfDestructComponent(1);
	}
	_selfDestructTime = 0;
}

bool UDebugDrawInWorldComponent::ShouldDraw(bool enabled, bool hasDrawn, EDrawRate drawRate) const
{
	if (enabled)
	{
		if (drawRate == EDrawRate::Once)
		{
			return !hasDrawn;
		}
		else if (drawRate == EDrawRate::EveryFrame)
		{
			return true;
		}
		else
		{
			ensureAlwaysMsgf(false, TEXT("unknown draw rate"));
			return false;
		}
	}
	else
	{
		return false;
	}
}