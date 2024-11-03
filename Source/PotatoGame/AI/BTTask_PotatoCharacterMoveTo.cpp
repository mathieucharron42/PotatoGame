// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BTTask_PotatoCharacterMoveTo.h"

#include "PotatoGame/Characters/PotatoBaseCharacter.h"

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
	}	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
