#pragma once
#include "AdvancedFriendsGameInstance.h"
#include "UHuntGameInstance.generated.h"
UCLASS()
class UHuntGameInstance : public UAdvancedFriendsGameInstance {
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
};
