#pragma once

#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Gameplay/PotatoGameRole.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PotatoPlayerController.generated.h"

UCLASS()
class POTATOGAME_API APotatoPlayerController : public APlayerController
{
	GENERATED_BODY()

	APotatoPlayerController();

public:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_ChangeRole();

	void Authority_ChangeRole();

protected:
	virtual void SetupInputComponent() override;

private:
	UFUNCTION(Server, Reliable)
	void Server_QuitGame();

	void Authority_QuitGame();
};
