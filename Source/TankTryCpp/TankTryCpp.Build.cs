// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TankTryCpp : ModuleRules
{
    public TankTryCpp(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] {"TankTryCpp/DonAINavigation/Private", "TankTryCpp/DonAINavigation/Private/Multithreading", /*... add other private include paths required here ...*/ });
        PublicIncludePaths.AddRange(new string[] { "TankTryCpp/DonAINavigation/Classes", "TankTryCpp/DonAINavigation/Classes/Multithreading" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "BehaviorTreeEditor", "GameplayTasks", "UMG", "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }
}
