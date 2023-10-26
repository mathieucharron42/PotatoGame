#include "PotatoEaterCharacter.h"

#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"

APotatoEaterCharacter::APotatoEaterCharacter()
{

}

void APotatoEaterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APotatoEaterCharacter, _currentScale);
}

void APotatoEaterCharacter::BeginPlay()
{
	Super::BeginPlay();
	_springArmComponent = FindComponentByClass<USpringArmComponent>();
	_initialSpringArmLenght = _springArmComponent->TargetArmLength;
}

void APotatoEaterCharacter::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);
}
