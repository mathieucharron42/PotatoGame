// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PotatoGame/Gameplay/PotatoGameRole.h"

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "PotatoGameMode.generated.h"

class APotato;
class APotatoAIController;
class APotatoPlayerController;

UCLASS()
class POTATOGAME_API APotatoGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;

	bool ChangeRole(APotatoPlayerController* playerController);
	void QuitGame(APotatoPlayerController* playerController);

	APotato* SpawnPotato(const FTransform& transform, const FVector& velocity);
	
private:
	static bool IsPossessed(const ACharacter* character);
	static bool IsPossessedByAI(const ACharacter* character);
	static bool IsPossessedByPlayer(const ACharacter* character);

	virtual void RestartPlayer(AController* NewPlayer) override;
	
	void CheckGameEnded();

	APotatoBaseCharacter* FindSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type);
	bool IsSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type, const APotatoBaseCharacter* character);
	FPotatoGameRole GetNextRole(FPotatoGameRole current);

	TOptional<FPotatoGameRole> GetRoleFromCharacterType(APotatoBaseCharacter* character) const;

	void SpawnAIControllers();

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APotato>> _potatoTypes;

	UPROPERTY(EditAnywhere)
	TArray<FPotatoGameRole> _supportedRoles;

	UPROPERTY(EditAnywhere)
	TArray<EGameRoleType> _aiRoles;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APotatoAIController> _aiControllerClass;
};
