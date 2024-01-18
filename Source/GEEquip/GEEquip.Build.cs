// Copyright (C) 2024 owoDra

using UnrealBuildTool;

public class GEEquip : ModuleRules
{
	public GEEquip(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                ModuleDirectory,
                ModuleDirectory + "/GEEquip",
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "CoreUObject", "Engine",

                "ModularGameplay", "GameplayTags",

                "GFCore", "GCItem",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "NetCore",
            }
        );

        SetupIrisSupport(Target);
    }
}
