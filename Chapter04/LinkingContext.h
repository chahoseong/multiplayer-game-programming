#pragma once

#include <cstdint>
#include <unordered_map>

class GameObject;

class LinkingContext
{
public:
	uint32_t GetNetworkId(GameObject* inGameObject);

	GameObject* GetGameObject(uint32_t inNetworkId);

private:
	std::unordered_map<uint32_t, GameObject*> networkIdToGameObject;
	std::unordered_map<GameObject*, uint32_t> gameObjectToNetworkId;
};