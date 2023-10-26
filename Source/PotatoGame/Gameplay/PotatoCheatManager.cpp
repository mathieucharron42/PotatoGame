#include "PotatoCheatManager.h"

#include "PotatoGame/Blockers/ForceField.h"
#include "PotatoGame/Characters/PotatoBaseCharacter.h"
//#include "PotatoGame/Crops/Potato.h"
//#include "PotatoGame/Crops/PotatoManagerSubsystem.h"
#include "PotatoGame/Gameplay/PotatoPlayerController.h"
#include "PotatoGame/Gameplay/PotatoGameMode.h"

#include "EngineUtils.h"

void UPotatoCheatManager::Potato_SpawnPotatoes(int32 amount)
{
	//UWorld* world = GetWorld();
	//APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	//if (ensure(IsValid(controller)))
	//{
	//	if (controller->HasAuthority())
	//	{
	//		const APotatoBaseCharacter* character = controller->GetPawn<APotatoBaseCharacter>();
	//		if (ensure(IsValid(character)))
	//		{
	//			APotatoGameMode* gameMode = world->GetAuthGameMode<APotatoGameMode>();
	//			if (ensure(IsValid(gameMode)))
	//			{
	//				for (int32 i = 0; i < amount; ++i)
	//				{
	//					FTransform potatoTransform = character->GetTransform();
	//					float angle = ((2 * PI) / amount) * i;
	//					FVector offset = FVector(FMath::Cos(angle), FMath::Sin(angle), 0);
	//					const float radius = 150;
	//					potatoTransform.AddToTranslation(offset * radius);
	//					FVector velocity = FVector::ZeroVector;
	//					gameMode->SpawnPotato(potatoTransform, velocity);
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		controller->ServerExec(FString::Printf(TEXT("Potato_SpawnPotatoes %d"), amount));
	//	}
	//}
}

void UPotatoCheatManager::Potato_ClearPotatoes()
{
	//UWorld* world = GetWorld();
	//if (ensure(IsValid(world)))
	//{
	//	APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	//	if (ensure(IsValid(controller)))
	//	{
	//		if (controller->HasAuthority())
	//		{
	//			UGameInstance* gameInstance = world->GetGameInstance();
	//			if (ensure(IsValid(gameInstance)))
	//			{
	//				UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
	//				for (APotato* potato : potatoSubsystem->GetPotatoes())
	//				{
	//					potato->Destroy();
	//				}
	//			}
	//		}
	//		else
	//		{
	//			controller->ServerExec(TEXT("Potato_ClearPotatoes"));
	//		}
	//	}
	//}
}

void UPotatoCheatManager::Potato_ScalePotatoes(float scale)
{
	//UWorld* world = GetWorld();
	//if (ensure(IsValid(world)))
	//{
	//	UGameInstance* gameInstance = world->GetGameInstance();
	//	if (ensure(IsValid(gameInstance)))
	//	{
	//		UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
	//		for (APotato* potato : potatoSubsystem->GetPotatoes())
	//		{
	//			if (ensure(IsValid(potato)))
	//			{
	//				potato->Cheat_Scale(scale);
	//			}
	//		}
	//	}
	//}
}

void UPotatoCheatManager::Potato_UseTheForce()
{
	//UWorld* world = GetWorld();
	//if (ensure(IsValid(world)))
	//{
	//	UGameInstance* gameInstance = world->GetGameInstance();
	//	if (ensure(IsValid(gameInstance)))
	//	{
	//		APotatoPlayerController* controller = Cast<APotatoPlayerController>(GetOuterAPlayerController());
	//		if (ensure(IsValid(controller)))
	//		{
	//			if(controller->HasAuthority())
	//			{
	//				UPotatoManagerSubsystem* potatoSubsystem = gameInstance->GetSubsystem<UPotatoManagerSubsystem>();
	//				const TArray<APotato*>& potatoes = potatoSubsystem->GetPotatoes();
	//				if (potatoes.Num() > 0)
	//				{
	//					APotato* potato = potatoes[0];
	//					if (IsValid(potato))
	//					{
	//						APotatoBaseCharacter* currentCharacter = controller->GetPawn<APotatoBaseCharacter>();
	//						if (ensure(IsValid(currentCharacter)))
	//						{
	//							// Drop potato if held by any other character
	//							for (TActorIterator<APotatoBaseCharacter> actorItr(world); actorItr; ++actorItr)
	//							{	
	//								APotatoBaseCharacter* character = *actorItr;
	//								if (character != currentCharacter)
	//								{
	//									UPotatoPickUpComponent* pickupComponent = character->FindComponentByClass<UPotatoPickUpComponent>();
	//									if (pickupComponent->IsHoldingPotato(potato))
	//									{
	//										pickupComponent->Authority_DropPotato();
	//									}
	//								}
	//							}
	//						
	//							// Grab potato by current character
	//							UPotatoPickUpComponent* pickupComponent = currentCharacter->FindComponentByClass<UPotatoPickUpComponent>();
	//							if (ensure(IsValid(pickupComponent)))
	//							{
	//								pickupComponent->Authority_PickupPotato(potato);
	//							}
	//						}
	//					}
	//				}
	//			}
	//			else
	//			{
	//				controller->ServerExec(TEXT("Potato_UseTheForce"));
	//			}
	//		}
	//	}
	//}
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