#include "PotatoPlayerState.h"

#include "Net/UnrealNetwork.h"

void APotatoPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APotatoPlayerState, _currentRole);
}

void APotatoPlayerState::SetCurrentRole(FPotatoGameRole role)
{
	_currentRole = role;
}

FPotatoGameRole APotatoPlayerState::GetCurrentRole() const
{
	return _currentRole;
}
