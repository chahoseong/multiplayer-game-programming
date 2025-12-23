#include "LinkingContext.h"

uint32_t LinkingContext::GetNetworkId(GameObject* inGameObject)
{
	auto it = gameObjectToNetworkId.find(inGameObject);
	if (it != gameObjectToNetworkId.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

GameObject* LinkingContext::GetGameObject(uint32_t inNetworkId)
{
	auto it = networkIdToGameObject.find(inNetworkId);
	if (it != networkIdToGameObject.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

