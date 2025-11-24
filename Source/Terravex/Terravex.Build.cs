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
			"JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
	}
}
