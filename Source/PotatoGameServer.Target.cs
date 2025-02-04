// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PotatoGameServerTarget : TargetRules
{
	public PotatoGameServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        bOverrideBuildEnvironment = true;
        ExtraModuleNames.Add("PotatoGame");
    }
}
