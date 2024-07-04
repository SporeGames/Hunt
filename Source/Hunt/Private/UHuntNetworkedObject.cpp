#include "UHuntNetworkedObject.h"

bool UHuntNetworkedObject::IsNameStableForNetworking() const
{
	return UObject::IsNameStableForNetworking();
}

bool UHuntNetworkedObject::IsSupportedForNetworking() const
{
	return true;
}
