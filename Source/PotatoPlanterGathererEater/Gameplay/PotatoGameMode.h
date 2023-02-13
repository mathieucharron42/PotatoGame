// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotatoPlanterGathererEater/Gameplay/PotatoGameRole.h"

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "PotatoGameMode.generated.h"

class APotato;
class APotatoPlayerController;

UCLASS()
class POTATOPLANTERGATHEREREATER_API APotatoGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void Tick(float dt) override;

	bool ChangeRole(APotatoPlayerController* playerController);
	void QuitGame(APotatoPlayerController* playerController);

	APotato* SpawnPotato(const FTransform& transform, const FVector& velocity);
	
private:
	virtual void RestartPlayer(AController* NewPlayer) override;
	
	void CheckGameEnded();
	void CheckGameEmpty();

	APotatoBaseCharacter* FindSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type);
	bool IsSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type, const APotatoBaseCharacter* character);
	FPotatoGameRole GetNextRole(FPotatoGameRole current);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APotato>> _potatoTypes;

	UPROPERTY(EditAnywhere)
	TArray<FPotatoGameRole> _roles;
};
