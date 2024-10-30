#include "PotatoGameMode.h"

#include "PotatoGame/Characters/PotatoEaterCharacter.h"
#include "PotatoGame/Characters/PotatoEatingComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Gameplay/PotatoGameRole.h"
#include "PotatoGame/Gameplay/PotatoGameState.h"
#include "PotatoGame/Gameplay/PotatoPlayerController.h"
#include "PotatoGame/Gameplay/PotatoAIController.h"
#include "PotatoGame/Gameplay/PotatoPlayerState.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "Algo/AllOf.h"
#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#endif
#include "EngineUtils.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

bool APotatoGameMode::IsPossessed(const ACharacter* character)
{
	return IsPossessedByPlayer(character) || IsPossessedByAI(character);
}

bool APotatoGameMode::IsPossessedByPlayer(const ACharacter* character)
{
	return IsValid(character->GetController<APotatoPlayerController>());
}

bool APotatoGameMode::IsPossessedByAI(const ACharacter* character)
{
	return IsValid(character->GetController<APotatoAIController>());
}

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
	UWorld* world = GetWorld();
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
	}
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
		if (character->IsA(type) && !IsPossessedByPlayer(character))
		{
			suitable = true;
		}
	}
	PotatoUtilities::DoSomething(1.1f);
	return suitable;
}

FPotatoGameRole APotatoGameMode::GetNextRole(FPotatoGameRole current)
{
	FPotatoGameRole nextRole;
	if (ensure(_supportedRoles.Num() > 0))
	{
		const int32 currentIndex = _supportedRoles.Find(current);
		const int32 nextIndex = (currentIndex + 1) % +_supportedRoles.Num();
		nextRole = _supportedRoles[nextIndex];
	}
	return nextRole;
}

TOptional<FPotatoGameRole> APotatoGameMode::GetRoleFromCharacterType(APotatoBaseCharacter* character) const
{
	TOptional<FPotatoGameRole> role;

	for (const FPotatoGameRole& gameRole : _supportedRoles)
	{
		if (character->IsA(gameRole.GetCharacterType()))
		{
			role = gameRole;
			break;
		}
	}
	
	return role;
}

void APotatoGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnAIControllers(_aiRoles);
}

void APotatoGameMode::SpawnAIControllers(const TArray<EGameRoleType>& roles)
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		for (TActorIterator<APotatoBaseCharacter> it(world); it; ++it)
		{
			APotatoBaseCharacter* character = *it;
			if (!IsPossessed(character))
			{
				TOptional<FPotatoGameRole> roleType = GetRoleFromCharacterType(character);
				if (roleType && roles.Contains(roleType->GetRoleType()))
				{
					if (ensure(IsValid(_aiControllerClass)))
					{
						APotatoAIController* aiController = world->SpawnActor<APotatoAIController>(_aiControllerClass);
						aiController->Possess(character);
					}
				}
			}
		}
	}
}

void APotatoGameMode::UnspawnAIControllers(const TArray<EGameRoleType>& roles)
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		for (TActorIterator<APotatoBaseCharacter> it(world); it; ++it)
		{
			APotatoBaseCharacter* character = *it;
			if (IsPossessed(character))
			{
				TOptional<FPotatoGameRole> roleType = GetRoleFromCharacterType(character);
				if (roleType && roles.Contains(roleType->GetRoleType()))
				{
					APotatoAIController* aiController = character->GetController<APotatoAIController>();
					if (IsValid(aiController))
					{
						aiController->UnPossess();
						aiController->Destroy();
					}
				}
			}
		}
	}
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

	for (FPotatoGameRole role = GetNextRole(initialRole); initialRole != role && !found; role = GetNextRole(role))
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("APotatoGameMode::ChangeRole Loop"))

		APotatoBaseCharacter* character = FindSuitableCharacter(role.GetCharacterType());
		if (IsValid(character))
		{
			playerController->Possess(character);
			playerState->SetCurrentRole(role);
			found = true;
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

APotato* APotatoGameMode::SpawnPotato(const FTransform& transform, const FVector& velocity)
{
	APotato* newPotato = nullptr;

	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		APotatoGameMode* potatoGameMode = world->GetAuthGameMode<APotatoGameMode>();
		if (ensure(IsValid(potatoGameMode)))
		{
			if(ensure(_potatoTypes.Num() > 0))
			{
				const TSubclassOf<APotato>& potatoType = _potatoTypes[FMath::RandRange(0, _potatoTypes.Num() - 1)];

				newPotato = world->SpawnActor<APotato>(potatoType, transform);
				UPrimitiveComponent* potatoPrimitiveComponent = Cast<UPrimitiveComponent>(newPotato->GetRootComponent());
				potatoPrimitiveComponent->SetPhysicsLinearVelocity(velocity);
			}
		}
	}

	return newPotato;
}