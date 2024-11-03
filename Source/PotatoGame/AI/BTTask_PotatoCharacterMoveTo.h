// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "BTTask_PotatoCharacterMoveTo.generated.h"


UCLASS(config = Game, MinimalAPI)
class UBTTask_PotatoCharacterMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
private:
	using TargetVariant = TVariant<std::monostate, FVector, AActor*>;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	TargetVariant GetTarget(const UBehaviorTreeComponent& behaviourTreeComponent) const;
	void SetDebugTarget(const UBehaviorTreeComponent& behaviourTreeComponent);
	
	UPROPERTY(Category = Node, EditAnywhere)
	bool _showDebugTarget = false;

	UPROPERTY(Category = Node, EditAnywhere)
	float _debugTargetRadius = 50;

	UPROPERTY(Category = Node, EditAnywhere)
	float _debugTargetSegments = 20;

	UPROPERTY(Category = Node, EditAnywhere)
	FColor _debugTargetColor = FColor::White;

	UPROPERTY(Category = Node, EditAnywhere)
	float _debugTargetLifetime = 5.f;
};
