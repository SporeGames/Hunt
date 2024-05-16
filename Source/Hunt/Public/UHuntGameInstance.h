#pragma once
#include "UHuntGameInstance.generated.h"
UCLASS()
class UHuntGameInstance : public UGameInstance {
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
};
