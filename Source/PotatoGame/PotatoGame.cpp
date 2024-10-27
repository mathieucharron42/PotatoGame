#include "PotatoGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, PotatoGame, "PotatoGame" );
 
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoPickupCapabale, "Character.Behaviour.PotatoPickupCapabale", "Able to pick up potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoPlantingCapabale, "Character.Behaviour.PotatoPlantingCapabale", "Able to plant potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoGatheringCapabale, "Character.Behaviour.PotatoGatheringCapabale", "Able to gather potato");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Behaviour_PotatoEatingCapabale, "Character.Behaviour.PotatoEatingCapabale", "Able to eat potato");