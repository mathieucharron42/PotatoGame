#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PotatoGameInstance.generated.h"

UCLASS()
class POTATOGAME_API UPotatoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
};
