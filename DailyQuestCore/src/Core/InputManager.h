#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#include "TimeStep.h"

enum class InputType
{
	Keyboard,
	Mouse
};

class InputManager;

class Bindings
{
public:
	Bindings() {}
	~Bindings() {}

	void SetBinding(InputType type, std::string newName, int key);
	bool TryGetBinding(InputType type, int key, std::vector<std::string>& name);
	bool TryGetReverseBinding(InputType type, std::string name, std::vector<int>& key);

	void Conversions(Bindings& targetBindings,
		std::unordered_map<InputType, 
		std::unordered_map<std::string, std::vector<std::string>>>& toTargets);
private:
	std::unordered_map<InputType,
		std::unordered_map<std::string, std::vector<int>>> _bindings;
	std::unordered_map<InputType,
		std::unordered_map<int, std::vector<std::string>>> _reverseBindings;
};


class Mouse
{
public:
	static void SetPosition(float x, float y);
	static void GetPosition(float& x, float& y) { x = (float)_x; y = (float)_y; }
	static void GetScroll(double& xOffset, double& yOffset) 
	{ xOffset = _xScroll; yOffset = _yScroll; }

	static void Hide();
	static void Show();
private:
	static void SetDataPosition(double x, double y) { _x = x; _y = y; }
	static void SetDataScroll(double xOffset, double yOffset) { _xScroll = xOffset; _yScroll = yOffset; }

	static double _x;
	static double _y;
	static double _xScroll;
	static double _yScroll;
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

		void Update(TimeStep timestep)
		{
			for (auto state : _currentState)
			{
				_previousState[state.first] = state.second;
			}
		}

		void Set(std::string name, float currentActivity)
		{
			//if (_currentState.count(name))
			//	_previousState[name] = _currentState[name];
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
		void Replace(std::string currentName, std::vector<std::string> newNames)
		{
			float current = 0.0f;
			float previous = 0.0f;
			if (_currentState.count(currentName))
			{
				current = _currentState[currentName];
				_currentState.erase(currentName);
				if (_previousState.count(currentName))
				{
					previous = _previousState[currentName];
					_previousState.erase(currentName);
				}
			}
			for (auto newName : newNames)
			{
				_currentState[newName] = current;
				_previousState[newName] = previous;
			}
		}
	private:
		std::unordered_map<std::string, float> _currentState;
		std::unordered_map<std::string, float> _previousState;
	};

public:
	static void Init(GLFWwindow* window);

	static void Update(TimeStep timestep);
	static void LateUpdate(TimeStep timestep);

	static void SetBinding(InputType type, std::string newName, int key);

	static void PushBindings(Bindings* bindings);
	static Bindings* PopBindings();

	static float IsDown(std::string input);
	static float IsUp(std::string input);
	static float IsPressed(std::string input);
	static float IsReleased(std::string input);

	static bool GetChar(unsigned int& key, int& mods);

	static bool GetClipboard(std::string& string);
	static void SetClipboard(std::string string);

	static bool GetDroppedPaths(std::vector<std::string>& paths);

private:
	static GLFWwindow* _window;

	static std::vector<Bindings*> _bindingsStack;
	static std::unordered_map<InputType, InputState> _inputStates;

	static std::vector<unsigned int> _typedKeys;
	static std::vector<int> _typedKeysMods;

	static std::vector<std::string> _droppedPaths;

	static void UpdateType(InputType type, int input, float activity);

	static bool TryGetBinding(InputType inputType, int input, std::vector<std::string>& name);
	static bool GetState(std::string input, float& current, float& previous);
	static void ResolvePushStates(Bindings* newBindings);
	static void ResolvePopStates(Bindings* newBindings);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharacterModCallback(GLFWwindow* window, unsigned int key, int mods);
	static void MousePositionCallback(GLFWwindow* window, double x, double y);
	static void MouseEnteredCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void DroppedPathsCallback(GLFWwindow* window, int count, const char** paths);


	friend class Mouse;
};

