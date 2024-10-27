// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PotatoAIController.generated.h"

class UBehaviorTree;

UCLASS()
class POTATOGAME_API APotatoAIController : public AAIController
{
	GENERATED_BODY()
private:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* _behaviourTree;
};
