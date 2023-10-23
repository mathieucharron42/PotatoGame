#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PotatoManagerSubsystem.generated.h"

class APotato;

UCLASS()
class POTATOGAME_API UPotatoManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterPotato(APotato* potato);
	void UnregisterPotato(APotato* potato);

	const TArray<APotato*> GetPotatoes() const;

private:
	UPROPERTY()
	TArray<APotato*> _potatoes;
};
