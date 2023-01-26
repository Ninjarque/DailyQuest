#pragma once

class TimeStep
{
public:
	TimeStep(float deltaTime, unsigned long frameID)
	{
		_deltaTime = deltaTime;
		_frameID = frameID;
	}
	operator float() const { return _deltaTime; }

	float GetElapsedTime() { return _deltaTime; }
	unsigned long GetCurrentFrameID() { return _frameID; }
private:
	float _deltaTime;
	unsigned long _frameID;
};