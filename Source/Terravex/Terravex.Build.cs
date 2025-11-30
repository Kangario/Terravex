using UnrealBuildTool;

public class Terravex : ModuleRules
{
	public Terravex(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"Http", 
			"Json", 
			"UMG",
			"JsonUtilities", "OnlineSubsystem", 
            "AndroidPermission" 
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem", 
			"OnlineSubsystemUtils"
		});
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			AdditionalPropertiesForReceipt.Add("GradleDependencies", 
				"implementation \"com.google.android.gms:play-services-auth:20.5.0\"");
		}
	}
}
