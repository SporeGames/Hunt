#pragma once
#include "UHuntNetworkedObject.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UHuntNetworkedObject : public UObject {
	GENERATED_BODY()
public:
	virtual bool IsNameStableForNetworking() const override;
	virtual bool IsSupportedForNetworking() const override;
};
