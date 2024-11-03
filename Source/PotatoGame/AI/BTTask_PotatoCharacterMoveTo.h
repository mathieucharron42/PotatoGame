// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "BTTask_PotatoCharacterMoveTo.generated.h"


UCLASS(config = Game, MinimalAPI)
class UBTTask_PotatoCharacterMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
