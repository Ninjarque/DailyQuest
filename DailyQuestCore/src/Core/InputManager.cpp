#include "InputManager.h"

double Mouse::_x;
double Mouse::_y;

GLFWwindow* InputManager::_window;
std::vector<Bindings*> InputManager::_bindingsStack;
std::unordered_map<InputType, InputManager::InputState> InputManager::_inputStates;
std::vector<unsigned int> InputManager::_typedKeys;
std::vector<int> InputManager::_typedKeysMods;

void InputManager::Init(GLFWwindow* window)
{
	_window = window;
	glfwSetKeyCallback(window, InputManager::KeyCallback);
	glfwSetCursorPosCallback(window, InputManager::MousePositionCallback);
	glfwSetCharModsCallback(window, InputManager::CharacterModCallback);
}

void InputManager::Update(float deltaTime)
{
	if (_typedKeys.size())
	{
		_typedKeys.erase(_typedKeys.begin());
		_typedKeysMods.erase(_typedKeysMods.begin());
	}
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::string keyBinding;
	TryGetBinding(InputType::Keyboard, key, keyBinding);
	switch (action)
	{
	case GLFW_PRESS:
		_inputStates[InputType::Keyboard].Set(keyBinding, 1.0f);
		break;
	case GLFW_RELEASE:
		_inputStates[InputType::Keyboard].Set(keyBinding, 0.0f);
		break;
	}
}

void InputManager::CharacterModCallback(GLFWwindow* window, unsigned int key, int mods)
{
	_typedKeys.push_back(key);
	_typedKeysMods.push_back(mods);
}

void InputManager::MousePositionCallback(GLFWwindow* window, double x, double y)
{
	Mouse::SetPosition(x, y);
}

void InputManager::PushBindings(Bindings* bindings)
{
	_bindingsStack.push_back(bindings);
}

Bindings* InputManager::PopBindings()
{
	Bindings* b = _bindingsStack.back();
	_bindingsStack.erase(_bindingsStack.end());
	return b;
}

float InputManager::IsDown(std::string input)
{
	float current;
	float previous;
	GetState(input, current, previous);
	return current;
}

float InputManager::IsUp(std::string input)
{
	float current;
	float previous;
	GetState(input, current, previous);
	return 1.0f - current;
}

float InputManager::IsPressed(std::string input)
{
	float current;
	float previous;
	GetState(input, current, previous);
	return current - previous;
}

float InputManager::IsReleased(std::string input)
{
	float current;
	float previous;
	GetState(input, current, previous);
	return previous - current;
}

bool InputManager::GetChar(unsigned int& key, int& mods)
{
	if (_typedKeys.size())
	{
		key = _typedKeys[0]; 
		_typedKeys.erase(_typedKeys.begin());
		mods = _typedKeysMods[0]; 
		_typedKeysMods.erase(_typedKeysMods.begin());
		return true;
	}
	return false;
}

bool InputManager::TryGetBinding(InputType inputType, int input, std::string& name)
{
	for (auto bindings : _bindingsStack)
	{
		if (bindings->TryGetBinding(inputType, input, name))
			return true;
	}
	name = std::to_string(input);
	return false;
}

bool InputManager::GetState(std::string input, float& current, float& previous)
{
	for (auto inputState : _inputStates)
	{
		if (inputState.second.Get(input, current, previous))
		{
			return true;
		}
	}
	return false;
}

void Mouse::SetPosition(double x, double y)
{
	glfwSetCursorPos(InputManager::_window, x, y);
}

void Mouse::Hide()
{
	glfwSetInputMode(InputManager::_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Mouse::Show()
{
	glfwSetInputMode(InputManager::_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
//Implement dis and test bindings and we'll be good for now
void Bindings::SetBinding(InputType type, std::string newName, int key)
{
}

bool Bindings::TryGetBinding(InputType type, int key, std::string& name)
{
	return false;
}

bool Bindings::TryGetReverseBinding(InputType type, std::string name, int& key)
{
	return false;
}
