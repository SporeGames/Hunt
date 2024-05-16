#include "UHuntGameInstance.h"
#include "MoviePlayer.h"

void UHuntGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UHuntGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UHuntGameInstance::EndLoadingScreen);
}

void UHuntGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
 
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UHuntGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}