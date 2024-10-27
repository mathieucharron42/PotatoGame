#include "PotatoAIController.h"

#include "BehaviorTree/BehaviorTree.h"

void APotatoAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ensure(IsValid(_behaviourTree)))
	{
		RunBehaviorTree(_behaviourTree);
	}
}
