#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"

#include "PotatoCheatManager.generated.h"

enum class EGameRoleType;

UCLASS()
class POTATOGAME_API UPotatoCheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec)
	void Potato_SpawnPotatoes(int32 amount);

	UFUNCTION(Exec)
	void Potato_ClearPotatoes();

	UFUNCTION(Exec)
	void Potato_RainPotatoes(int32 amount, float frequency);

	UFUNCTION(Exec)
	void Potato_ScalePotatoes(float scale);

	UFUNCTION(Exec)
	void Potato_UseTheForce();

	UFUNCTION(Exec)
	void Potato_IgnoreForceField();

	UFUNCTION(Exec)
	void Potato_RemoveForceField();

	UFUNCTION(Exec)
	void Potato_Unpossess();

	UFUNCTION(Exec)
	void Potato_Repossess();

	UFUNCTION(Exec)
	void Potato_ActivateAIAllCharacter();

	UFUNCTION(Exec)
	void Potato_ActivateAIPlanter();

	UFUNCTION(Exec)
	void Potato_ActivateAIGatherer();

	UFUNCTION(Exec)
	void Potato_ActivateAIEater();

	UFUNCTION(Exec)
	void Potato_DeactivateAIAllCharacter();

	UFUNCTION(Exec)
	void Potato_DeactivateAIPlanter();

	UFUNCTION(Exec)
	void Potato_DeactivateAIGatherer();

	UFUNCTION(Exec)
	void Potato_DeactivateAIEater();

private:
	void RainPotatoLoop(int32 remaining, float frequency);

	void ActivateAI(const TArray<EGameRoleType>& roles);
	void DeactivateAI(const TArray<EGameRoleType>& roles);
};
