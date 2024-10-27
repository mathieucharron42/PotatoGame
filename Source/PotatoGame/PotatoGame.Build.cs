// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PotatoGame : ModuleRules
{
	public PotatoGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags", "InputCore", "HeadMountedDisplay" });

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }
    }
}
