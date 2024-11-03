// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BTTask_PotatoCharacterMoveTo.h"

#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Utils/Visualizations/DebugDrawInWorldComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIModule/Classes/AIController.h"

EBTNodeResult::Type UBTTask_PotatoCharacterMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (IsValid(AIController))
	{
		APotatoBaseCharacter* potatoCharacter = Cast<APotatoBaseCharacter>(AIController->GetPawn());

		if (IsValid(potatoCharacter))
		{
			FilterClass = potatoCharacter->GetNavigationQueryFilter();			
		}

		SetDebugTarget(OwnerComp);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_PotatoCharacterMoveTo::SetDebugTarget(const UBehaviorTreeComponent& behaviourTreeComponent)
{
	if (_showDebugTarget)
	{
		TargetVariant target = GetTarget(behaviourTreeComponent);

		if (!target.IsType<std::monostate>())
		{
			AActor* targetActor = nullptr;
			bool dummyActorCreated = false;
			FVector location;
			if (target.IsType<FVector>())
			{
				location = target.Get<FVector>();

				targetActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

				UActorComponent* actorComponent = targetActor->AddComponentByClass(UDebugDrawInWorldComponent::StaticClass(), false, FTransform(), false);
				targetActor->SetActorLocation(location);

				dummyActorCreated = true;
			}
			else if (target.IsType<AActor*>())
			{
				targetActor = target.Get<AActor*>();
				location = targetActor->GetActorLocation();
			}

			UDebugDrawInWorldComponent* targetDebugInWorldComponent = nullptr;
			if (IsValid(targetActor))
			{
				if (targetActor->GetComponentByClass<UDebugDrawInWorldComponent>())
				{
					targetDebugInWorldComponent = targetActor->GetComponentByClass<UDebugDrawInWorldComponent>();
				}
				else
				{
					targetDebugInWorldComponent = Cast<UDebugDrawInWorldComponent>(
						targetActor->AddComponentByClass(
							UDebugDrawInWorldComponent::StaticClass(),
							false,
							FTransform(),
							false
						)
					);
				}
			}

			if (IsValid(targetDebugInWorldComponent))
			{
				FDrawInWorldSphereProperties properties;
				properties.Radius = _debugTargetRadius;
				properties.Color = _debugTargetColor;
				properties.NbSegments = _debugTargetSegments;
				properties.Lifetime = _debugTargetLifetime;
				const bool drawOnce = true;
				targetDebugInWorldComponent->EnableDrawSphere(properties);

				if (dummyActorCreated)
				{
					targetDebugInWorldComponent->EnableSelfDestructActor(_debugTargetLifetime);
				}
				else
				{
					targetDebugInWorldComponent->EnableSelfDestructComponent(_debugTargetLifetime);
				}
			}
		}
	}
}

UBTTask_PotatoCharacterMoveTo::TargetVariant UBTTask_PotatoCharacterMoveTo::GetTarget(const UBehaviorTreeComponent& behaviourTreeComponent) const
{
	TargetVariant target;

	const UBlackboardComponent* blackboard = behaviourTreeComponent.GetBlackboardComponent();
	if (ensure(IsValid(blackboard)))
	{
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* targetObject = blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			if (IsValid(targetObject))
			{
				AActor* targetActor = Cast<AActor>(targetObject);
				if (IsValid(targetActor))
				{
					target.Set<AActor*>(targetActor);
				}
			}
		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			target.Set<FVector>(blackboard->GetValueAsVector(BlackboardKey.SelectedKeyName));
		}
	}

	return target;
}
