#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

enum class InputType
{
	Keyboard,
	Mouse
};

class Bindings
{
public:
	Bindings() {}
	~Bindings() {}

	void SetBinding(InputType type, std::string newName, int key);
	bool TryGetBinding(InputType type, int key, std::string& name);
	bool TryGetReverseBinding(InputType type, std::string name, int& key);

private:
	std::unordered_map<InputType,
		std::unordered_map<std::string, int>> _bindings;
};

class InputManager;

class Mouse
{
public:
	static void SetPosition(double x, double y);
	static void GetPosition(double& x, double& y) { x = _x; y = _y; }

	static void Hide();
	static void Show();
private:
	static void SetDataPosition(double x, double y) { _x = x; _y = y; }

	static double _x;
	static double _y;
	friend class InputManager;
};

class InputManager
{
private:
	class InputState
	{
	public:
		InputState() {}
		~InputState() {}

		void Set(std::string name, float currentActivity)
		{
			if (_currentState.count(name))
				_previousState[name] = _currentState[name];
			_currentState[name] = currentActivity;
		}
		bool Get(std::string name, float& currentlyActive, float& previouslyActive)
		{
			currentlyActive = 0.0f;
			previouslyActive = 0.0f;
			if (_currentState.count(name))
			{
				currentlyActive = _currentState[name];
				if (_previousState.count(name))
					previouslyActive = _previousState[name];
				return true;
			}
			return false;
		}
	private:
		std::unordered_map<std::string, float> _currentState;
		std::unordered_map<std::string, float> _previousState;
	};

public:
	static void Init(GLFWwindow* window);

	static void Update(float deltaTime);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharacterModCallback(GLFWwindow* window, unsigned int key, int mods);
	static void MousePositionCallback(GLFWwindow* window, double x, double y);

	static void PushBindings(Bindings* bindings);
	static Bindings* PopBindings();

	static float IsDown(std::string input);
	static float IsUp(std::string input);
	static float IsPressed(std::string input);
	static float IsReleased(std::string input);

	static bool GetChar(unsigned int& key, int& mods);

private:
	static GLFWwindow* _window;

	static std::vector<Bindings*> _bindingsStack;
	static std::unordered_map<InputType, InputState> _inputStates;

	static std::vector<unsigned int> _typedKeys;
	static std::vector<int> _typedKeysMods;

	static bool TryGetBinding(InputType inputType, int input, std::string& name);
	static bool GetState(std::string input, float& current, float& previous);

	friend class Mouse;
};

