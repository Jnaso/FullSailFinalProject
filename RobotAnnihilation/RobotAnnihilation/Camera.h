#pragma once
class Camera
{
	struct MousePos
	{
		float x;
		float y;
	};

	MousePos CurrentPos, PreviousPos;

public:
	void Update(float delta);
	Camera();
	~Camera();
};

