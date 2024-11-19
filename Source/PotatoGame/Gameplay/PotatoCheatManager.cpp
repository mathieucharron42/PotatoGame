#include "PotatoCheatManager.h"

#include "PotatoGame/Blockers/ForceField.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
#include "PotatoGame/Characters/Components/PotatoPickupComponent.h"
#include "PotatoGame/Crops/Potato.h"
#include "PotatoGame/Crops/PotatoManagerSubsystem.h"
#include "PotatoGame/Gameplay/PotatoPlayerController.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"
#include "PotatoGame/Utils/PotatoUtilities.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"
#include <Kismet/KismetMathLibrary.h>

void UPotatoCheatManager::Potato_SpawnPotatoes(int32 amount)
{
	UWorld* world = GetWorld();
	APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	if (ensure(IsValid(controller)))
	{
		if (controller->HasAuthority())
		{
			const APotatoBaseCharacter* character = controller->GetPawn<APotatoBaseCharacter>();
			if (ensure(IsValid(character)))
			{
				APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
				if (ensure(IsValid(gameMode)))
				{
					for (int32 i = 0; i < amount; ++i)
					{
						FTransform potatoTransform = character->GetTransform();
						float angle = ((2 * PI) / amount) * i;
						FVector offset = FVector(FMath::Cos(angle), FMath::Sin(angle), 0);
						const float radius = 150;
						potatoTransform.AddToTranslation(offset * radius);
						FVector velocity = FVector::ZeroVector;
						gameMode->SpawnPotato(potatoTransform, velocity);
					}
				}
			}
		}
		else
		{
			controller->ServerExec(FString::Printf(TEXT("Potato_SpawnPotatoes %d"), amount));
		}
	}
}

void UPotatoCheatManager::Potato_ClearPotatoes()
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
		if (ensure(IsValid(controller)))
		{
			if (controller->HasAuthority())
			{
				UGameInstance* gameInstance = world->GetGameInstance();
				if (ensure(IsValid(gameInstance)))
				{
					UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
					for (APotato* potato : potatoSubsystem->GetPotatoes())
					{
						potato->Destroy();
					}
				}
			}
			else
			{
				controller->ServerExec(TEXT("Potato_ClearPotatoes"));
			}
		}
	}
}

void UPotatoCheatManager::Potato_RainPotatoes(int32 amount, float frequency)
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (ensure(IsValid(gameInstance)))
		{
			RainPotatoLoop(amount, frequency);
		}
	}
}

void UPotatoCheatManager::RainPotatoLoop(int32 remaining, float frequency)
{
	UE_LOG(LogPotato, Log, TEXT("Raining potato (remaining %d at every %f sec)"), remaining, frequency);
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (ensure(IsValid(gameInstance)))
		{
			APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
			if (ensure(IsValid(gameMode)))
			{
				if (remaining > 0)
				{
					TArray<AActor*> clouds;
					UGameplayStatics::GetAllActorsWithTag(world, FName("PotatoCloud"), clouds);

					if (clouds.Num() > 0)
					{
						const ATriggerBox* cloud = Cast<ATriggerBox>(clouds[FMath::RandRange(0, clouds.Num() - 1)]);
						if (IsValid(cloud))
						{
							const FBox box = cloud->GetComponentsBoundingBox();

							const FVector location = UKismetMathLibrary::RandomPointInBoundingBox_Box(box);

							FTransform potatoTransform;
							potatoTransform.SetLocation(location);
							const FVector velocity = FVector::ZeroVector;
							gameMode->SpawnPotato(potatoTransform, velocity);

							const auto callback = FTimerDelegate::CreateUObject(this, &UPotatoCheatManager::RainPotatoLoop, remaining - 1, frequency);
							const float delay = FMath::Max(frequency, 0.0000000001f);
							const bool loop = false;

							FTimerHandle handle;
							gameInstance->GetTimerManager().SetTimer(handle, callback, delay, loop);
						}
					}
				}
			}
		}
	}
}

void UPotatoCheatManager::Potato_ScalePotatoes(float scale)
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (ensure(IsValid(gameInstance)))
		{
			UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
			for (APotato* potato : potatoSubsystem->GetPotatoes())
			{
				if (ensure(IsValid(potato)))
				{
					potato->Cheat_Scale(scale);
				}
			}
		}
	}
}

void UPotatoCheatManager::Potato_UseTheForce()
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (ensure(IsValid(gameInstance)))
		{
			APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
			if (ensure(IsValid(controller)))
			{
				if(controller->HasAuthority())
				{
					UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
					const TArray<APotato*>& potatoes = potatoSubsystem->GetPotatoes();
					if (potatoes.Num() > 0)
					{
						APotato* potato = potatoes[0];
						if (IsValid(potato))
						{
							APotatoBaseCharacter* currentCharacter = controller->GetPawn<APotatoBaseCharacter>();
							if (ensure(IsValid(currentCharacter)))
							{
								// Drop potato if held by any other character
								for (TActorIterator<APotatoBaseCharacter> actorItr(world); actorItr; ++actorItr)
								{	
									APotatoBaseCharacter* character = *actorItr;
									if (character != currentCharacter)
									{
										UPotatoPickUpComponent* pickupComponent = character->FindComponentByClass<UPotatoPickUpComponent>();
										if (pickupComponent->IsHoldingPotato(potato))
										{
											pickupComponent->Authority_DropPotato();
										}
									}
								}
							
								// Grab potato by current character
								UPotatoPickUpComponent* pickupComponent = currentCharacter->FindComponentByClass<UPotatoPickUpComponent>();
								if (ensure(IsValid(pickupComponent)))
								{
									pickupComponent->Authority_PickupPotato(potato);
								}
							}
						}
					}
				}
				else
				{
					controller->ServerExec(TEXT("Potato_UseTheForce"));
				}
			}
		}
	}
}

void UPotatoCheatManager::Potato_IgnoreForceField()
{
	UWorld* world = GetWorld();
	APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	if (ensure(IsValid(controller)))
	{
		if (controller->HasAuthority())
		{
			const APotatoBaseCharacter* character = controller->GetPawn<APotatoBaseCharacter>();
			if (ensure(IsValid(character)))
			{
				UPrimitiveComponent* primativeComponent = Cast<UPrimitiveComponent>(character->GetRootComponent());
				if (ensure(IsValid(primativeComponent)))
				{
					const ECollisionChannel forceFieldChannel = ECC_GameTraceChannel1;
					primativeComponent->SetCollisionResponseToChannel(forceFieldChannel, ECR_Ignore);
				}
			}
		}
		else
		{
			controller->ServerExec(TEXT("Potato_IgnoreForceField"));
		}
	}
}

void UPotatoCheatManager::Potato_RemoveForceField()
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world)))
	{
		for (TActorIterator<AForceField> actorItr(world); actorItr; ++actorItr)
		{
			AForceField* forceField = *actorItr;
			forceField->Destroy();
		}
	}
}

void UPotatoCheatManager::Potato_ActivateAIAllCharacter()
{
	ActivateAI({ EGameRoleType::Eater, EGameRoleType::Planter, EGameRoleType::Gatherer });
}

void UPotatoCheatManager::Potato_ActivateAIPlanter()
{
	ActivateAI({ EGameRoleType::Planter });
}

void UPotatoCheatManager::Potato_ActivateAIGatherer()
{
	ActivateAI({ EGameRoleType::Gatherer });
}

void UPotatoCheatManager::Potato_ActivateAIEater()
{
	ActivateAI({ EGameRoleType::Eater});
}

void UPotatoCheatManager::ActivateAI(const TArray<EGameRoleType>& roles)
{
	UWorld* world = GetWorld();
	APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	if (ensure(IsValid(controller)))
	{
		APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
		if (ensure(IsValid(gameMode)))
		{
			TArray<EGameRoleType> newRoles = gameMode->GetAIRoles();
			for (EGameRoleType newRole : roles)
			{
				newRoles.AddUnique(newRole);
			}
			gameMode->SetAIRoles(newRoles);
			gameMode->RefreshAIControllers();
		}
	}
}

void UPotatoCheatManager::Potato_DeactivateAIAllCharacter()
{
	DeactivateAI({ EGameRoleType::Eater, EGameRoleType::Gatherer, EGameRoleType::Planter });
}

void UPotatoCheatManager::Potato_DeactivateAIPlanter()
{
	DeactivateAI({ EGameRoleType::Planter });
}

void UPotatoCheatManager::Potato_DeactivateAIGatherer()
{
	DeactivateAI({ EGameRoleType::Gatherer });
}

void UPotatoCheatManager::Potato_DeactivateAIEater()
{
	DeactivateAI({ EGameRoleType::Eater });
}

void UPotatoCheatManager::DeactivateAI(const TArray<EGameRoleType>& roles)
{
	UWorld* world = GetWorld();
	APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	if (ensure(IsValid(controller)))
	{
		APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
		if (ensure(IsValid(gameMode)))
		{
			TArray<EGameRoleType> newRoles = gameMode->GetAIRoles();
			for (EGameRoleType deleteRole : roles)
			{
				newRoles.Remove(deleteRole);
			}
			gameMode->SetAIRoles(newRoles);
			gameMode->RefreshAIControllers();
		}
	}
}
