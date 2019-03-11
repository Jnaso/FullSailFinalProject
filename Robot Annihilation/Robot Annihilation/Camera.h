#pragma once
class Camera
{
	struct MousePos
	{
		float x;
		float y;
	};

	MousePos CurrentPos, PreviousPos;
	
	GW::MATH::GMATRIXF View;

public:
	void Update(float delta);
	Camera();
	~Camera();
};

