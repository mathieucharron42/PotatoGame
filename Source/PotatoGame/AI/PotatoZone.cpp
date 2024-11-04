// Fill out your copyright notice in the Description page of Project Settings.


#include "PotatoZone.h"

#include "PotatoGame/Gameplay/GameplayTagComponent.h"

APotatoZone::APotatoZone()
{
	GameplayTag = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTag"));
}

void APotatoZone::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	GameplayTag->GetOwnedGameplayTags(TagContainer);
}