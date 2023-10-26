#include "PotatoGameMode.h"

#include "PotatoGame/Gameplay/PotatoGameRole.h"
#include "PotatoGame/Gameplay/PotatoGameState.h"
#include "PotatoGame/Gameplay/PotatoPlayerController.h"
#include "PotatoGame/Gameplay/PotatoPlayerState.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Algo/AllOf.h"
#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#endif
#include "EngineUtils.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

void APotatoGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer->IsA<APotatoPlayerController>())
	{
		APotatoPlayerController* potatoPlayerController = Cast<APotatoPlayerController>(NewPlayer);
		ensure(ChangeRole(potatoPlayerController));
	}
	else
	{
		Super::RestartPlayer(NewPlayer);
	}
}

void APotatoGameMode::CheckGameEnded()
{
	/*UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		TArray<UPotatoEatingComponent*> eatingComponents;
		for (TActorIterator<APotatoBaseCharacter> it(world); it; ++it)
		{
			AActor* actor = *it;
			if (IsValid(actor))
			{
				UPotatoEatingComponent* component = it->FindComponentByClass<UPotatoEatingComponent>();
				if (IsValid(component))
				{
					eatingComponents.Add(component);
				}
			}
		}

		const bool arePotatoEatersWellFeed = Algo::AllOf(eatingComponents, [](UPotatoEatingComponent* eatingComponent)
		{
			return !eatingComponent->IsHungry();
		});

		if (eatingComponents.Num() > 0 && arePotatoEatersWellFeed)
		{
			APotatoGameState* gameState = GetGameState<APotatoGameState>();
			if (ensure(IsValid(gameState)))
			{
				gameState->SetGameEnded(true);
			}
		}
	}*/
}

APotatoBaseCharacter* APotatoGameMode::FindSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::FindSuitableCharacter"))
	APotatoBaseCharacter* suitableCharacter = nullptr;
	UWorld* world = GetWorld();
	if(ensure(IsValid(world)))
	{
		for (TActorIterator<APotatoBaseCharacter> it(world); it && !IsValid(suitableCharacter); ++it)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::FindSuitableCharacter Loop"))
			APotatoBaseCharacter* character = Cast<APotatoBaseCharacter>(*it);
			if (IsSuitableCharacter(type, character))
			{
				suitableCharacter = character;
			}
		}
	}
	return suitableCharacter;
}

bool APotatoGameMode::IsSuitableCharacter(const TSubclassOf<APotatoBaseCharacter>& type, const APotatoBaseCharacter* character)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::IsSuitableCharacter"))
	bool suitable = false;
	if (ensure(IsValid(character)))
	{
		if (character->IsA(type))
		{
			const APotatoPlayerController* possessingController = character->GetController<APotatoPlayerController>();
			const bool isCharacterPossessed = IsValid(possessingController);
			suitable = !isCharacterPossessed;
		}
	}
	PotatoUtilities::DoSomething(1.1f);
	return suitable;
}

FPotatoGameRole APotatoGameMode::GetNextRole(FPotatoGameRole current)
{
	FPotatoGameRole nextRole;
	if (ensure(_roles.Num() > 0))
	{
		const int32 currentIndex = _roles.Find(current);
		const int32 nextIndex = (currentIndex + 1) % +_roles.Num();
		nextRole = _roles[nextIndex];
	}
	return nextRole;
}

void APotatoGameMode::Tick(float dt)
{
	Super::Tick(dt);

	CheckGameEnded();
}

bool APotatoGameMode::ChangeRole(APotatoPlayerController* playerController)
{
	TRACE_BOOKMARK(TEXT("ChangeRole"));
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::ChangeRole"));
	bool found = false;
	TArray<FPotatoGameRole> consideredRoles;

	APotatoPlayerState* playerState = playerController->GetPlayerState<APotatoPlayerState>();
	FPotatoGameRole initialRole = playerState->GetCurrentRole();

	for (FPotatoGameRole role = GetNextRole(initialRole); initialRole != role; role = GetNextRole(role))
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::ChangeRole Loop"))

		APotatoBaseCharacter* character = FindSuitableCharacter(role.GetCharacterType());
		if (IsValid(character))
		{
			playerController->Possess(character);
			playerState->SetCurrentRole(role);
			found = true;
			break;
		}
	}

	return found;
}

void APotatoGameMode::QuitGame(APotatoPlayerController* playerController)
{
	// Disconnect player
	playerController->Destroy(false);

	// Check if server should shutdown
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		bool isServerLeaving = playerController->IsLocalController();

		bool allPlayersLeft = true;
		for (TActorIterator<APotatoPlayerController> it(world); it; ++it)
		{
			if (IsValid(*it) && *it != playerController)
			{
				allPlayersLeft = false;
			}
		}

		if (allPlayersLeft || isServerLeaving)
		{
			// Have player quit which might shutdown game if server
			UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Quit, false);
#if WITH_EDITOR
			// If dedicated server, shutdown explicitly
			if (world->GetNetMode() == ENetMode::NM_DedicatedServer)
			{
				UEditorEngine* engine = Cast<UEditorEngine>(GEngine);
				if (IsValid(engine))
				{
					engine->RequestEndPlayMap();
				}
			}
#endif
		}
	}
}