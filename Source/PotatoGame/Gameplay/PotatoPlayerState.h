// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotatoGame/Gameplay/PotatoGameRole.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "PotatoPlayerState.generated.h"

UCLASS()
class POTATOGAME_API APotatoPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void SetCurrentRole(FPotatoGameRole role);
	FPotatoGameRole GetCurrentRole() const;

private:
	UPROPERTY(Transient, Replicated)
	FPotatoGameRole _currentRole;
};
