// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameplayTagAssetInterface.h"

#include "PotatoZone.generated.h"

UCLASS()
class POTATOGAME_API APotatoZone : public ATriggerBox, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGameplayTagComponent* GameplayTag;

public:	
	// Sets default values for this actor's properties
	APotatoZone();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
