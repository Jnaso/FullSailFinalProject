#pragma once
#include <vector>

class GameManager
{
	// Replace int with GameObjects when implemented
	std::vector<int> GameObjects;
public:
	GameManager();
	~GameManager();
	void Update(float delta);
};

