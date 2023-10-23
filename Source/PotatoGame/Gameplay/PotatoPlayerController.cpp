#include "PotatoPlayerController.h"

#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Gameplay/PotatoCheatManager.h"

#include "EngineUtils.h"

APotatoPlayerController::APotatoPlayerController()
{
	CheatClass = UPotatoCheatManager::StaticClass();
}

void APotatoPlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnableCheats();
}

void APotatoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Switch", IE_Pressed, this, &APotatoPlayerController::Server_ChangeRole);
	InputComponent->BindAction("Quit", IE_Pressed, this, &APotatoPlayerController::Server_QuitGame);
}

void APotatoPlayerController::Server_QuitGame_Implementation()
{
	Authority_QuitGame();
}

void APotatoPlayerController::Authority_QuitGame()
{
	if (ensure(HasAuthority()))
	{
		UWorld* world = GetWorld();
		if (ensure(IsValid(world)))
		{
			APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
			if (ensure(IsValid(gameMode)))
			{
				gameMode->QuitGame(this);
			}
		}
	}
}

void APotatoPlayerController::Server_ChangeRole_Implementation()
{
	Authority_ChangeRole();
}

void APotatoPlayerController::Authority_ChangeRole()
{
	if(ensure(HasAuthority()))
	{
		UWorld* world = GetWorld();
		if(ensure(IsValid(world)))
		{
			APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
			if (ensure(IsValid(gameMode)))
			{
				gameMode->ChangeRole(this);
			}
		}
	}
}
