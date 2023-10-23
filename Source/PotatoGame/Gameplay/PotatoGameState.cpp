#include "PotatoGameState.h"

#include "Net/UnrealNetwork.h"

void APotatoGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APotatoGameState, _gameEnded);
}

bool APotatoGameState::IsGameEnded()
{
	return _gameEnded;
}

void APotatoGameState::SetGameEnded(bool completed)
{
	_gameEnded = completed;
}