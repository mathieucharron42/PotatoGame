// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BTTask_PotatoCharacterMoveTo.h"

#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Utils/Visualizations/DebugDrawInWorldComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"

uint16 UBTTask_PotatoCharacterMoveTo::GetInstanceMemorySize() const
{
	return sizeof(FBTPotatoCharacterMoveToTaskMemory);
}

void UBTTask_PotatoCharacterMoveTo::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTPotatoCharacterMoveToTaskMemory>(NodeMemory, InitType);
}

void UBTTask_PotatoCharacterMoveTo::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTPotatoCharacterMoveToTaskMemory>(NodeMemory, CleanupType);
}

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

		SetDebugTarget(OwnerComp, NodeMemory);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_PotatoCharacterMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FBTPotatoCharacterMoveToTaskMemory* myMemory = CastInstanceNodeMemory<FBTPotatoCharacterMoveToTaskMemory>(NodeMemory);
	if (myMemory->TargetDebugInWorldComponent.IsValid())
	{
		myMemory->TargetDebugInWorldComponent->ForceSelfDestruct();
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_PotatoCharacterMoveTo::SetDebugTarget(const UBehaviorTreeComponent& behaviourTreeComponent, uint8* nodeMemory)
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
				properties.Lifetime = -1;
				properties.DrawRate = EDrawRate::EveryFrame;
				
				targetDebugInWorldComponent->EnableDrawSphere(properties);

				if (dummyActorCreated)
				{
					targetDebugInWorldComponent->EnableSelfDestructActor(_debugTargetLifetime);
				}
				else
				{
					targetDebugInWorldComponent->EnableSelfDestructComponent(_debugTargetLifetime);
				}

				FBTPotatoCharacterMoveToTaskMemory* myMemory = CastInstanceNodeMemory<FBTPotatoCharacterMoveToTaskMemory>(nodeMemory);
				myMemory->TargetDebugInWorldComponent = targetDebugInWorldComponent;
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
