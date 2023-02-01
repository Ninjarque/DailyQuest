#pragma once

class Viewport
{
public:
	Viewport()
	{
		_isWindow = true;
	}
	Viewport(float x, float y, float width, float height)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_needsRecalcultations = false;
		_isWindow = false;
	}

	void GetPosition(float& x, float& y) 
	{ 
		if (_isWindow) 
		{
			x = 0.0f; y = 0.0f;
			return;
		} 
		x = _x; y = _y;
	}
	void GetSize(float& width, float& height)
	{
		if (_isWindow)
		{
			width = Window::Current->GetWidth();
			height = Window::Current->GetHeight();
			return;
		}  
		width = _width; height = _height; 
	}
	float GetX() { if (_isWindow) return 0.0f; return _x; }
	float GetY() { if (_isWindow) return 0.0f; return _y; }
	float GetWidth() { if (_isWindow) return Window::Current->GetWidth(); return _width; }
	float GetHeight() { if (_isWindow) return Window::Current->GetHeight(); return _height; }

	void SetPosition(float x, float y) { _x = x; _y = y; _needsRecalcultations = true; }
	void SetSize(float width, float height) { _width = width; _height = height; _needsRecalcultations = true; }
	void SetX(float x) { _x = x; _needsRecalcultations = true; }
	void SetY(float y) { _y = y; _needsRecalcultations = true; }
	void SetWidth(float width) { _width = width; _needsRecalcultations = true; }
	void SetHeight(float height) { _height = height; _needsRecalcultations = true; }

	bool NeedsRecalculations() { return _needsRecalcultations; }
	void RecalculationsDone() { _needsRecalcultations = false; }
private:
	float _x, _y, _width, _height;
	bool _needsRecalcultations;
	bool _isWindow;
};