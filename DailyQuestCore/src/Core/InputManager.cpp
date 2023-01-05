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
	glfwSetCursorEnterCallback(window, InputManager::MouseEnteredCallback);
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);

	_bindingsStack.push_back(new Bindings());
}

void InputManager::Update(float deltaTime)
{
	if (_typedKeys.size())
	{
		_typedKeys.erase(_typedKeys.begin());
		_typedKeysMods.erase(_typedKeysMods.begin());
	}
}

void InputManager::LateUpdate(float deltaTime)
{
	for (auto& inputState : _inputStates)
	{
		inputState.second.Update(deltaTime);
	}
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS: UpdateType(InputType::Keyboard, key, 1.0f); break;
	case GLFW_RELEASE: UpdateType(InputType::Keyboard, key, 0.0f); break;
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

void InputManager::MouseEnteredCallback(GLFWwindow* window, int entered)
{
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS: UpdateType(InputType::Mouse, button, 1.0f); break;
	case GLFW_RELEASE: UpdateType(InputType::Mouse, button, 0.0f); break;
	}
}

void InputManager::SetBinding(InputType type, std::string newName, int key)
{
	if (_bindingsStack.size() == 0)
		PushBindings(new Bindings());
	Bindings* b = _bindingsStack[_bindingsStack.size() - 1];
	b->SetBinding(type, newName, key);
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
	return glm::max(0.0f, current - previous);
}

float InputManager::IsReleased(std::string input)
{
	float current;
	float previous;
	GetState(input, current, previous);
	return glm::max(0.0f, previous - current);
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

void InputManager::UpdateType(InputType type, int input, float activity)
{
	std::vector<std::string> bindings;
	TryGetBinding(type, input, bindings);
	for (auto binding : bindings)
		_inputStates[type].Set(binding, activity);
}

bool InputManager::TryGetBinding(InputType inputType, int input, std::vector<std::string>& name)
{
	for (int i = _bindingsStack.size() - 1; i >= 0; i--)
	{
		auto bindings = _bindingsStack[i];
		if (bindings->TryGetBinding(inputType, input, name))
			return true;
	}
	name.push_back(std::to_string(input));
	return false;
}

bool InputManager::GetState(std::string input, float& current, float& previous)
{
	current = 0.0f;
	previous = 0.0f;
	float c_current = 0.0f;
	float c_previous = 0.0f;
	bool found = false;
	for (auto inputState : _inputStates)
	{
		if (inputState.second.Get(input, c_current, c_previous))
		{
			current = c_current > current ? c_current : current;
			previous = c_previous > previous ? c_previous : previous;
			found = true;
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

void Bindings::SetBinding(InputType type, std::string newName, int key)
{
	_bindings[type][newName].push_back(key);
	_reverseBindings[type][key].push_back(newName);
}

bool Bindings::TryGetBinding(InputType type, int key, std::vector<std::string>& name)
{
	if (_reverseBindings[type].count(key))
	{
		name = _reverseBindings[type][key];
		return true;
	}
	return false;
}

bool Bindings::TryGetReverseBinding(InputType type, std::string name, std::vector<int>& key)
{
	if (_bindings[type].count(name))
	{
		key = _bindings[type][name];
		return true;
	}
	return false;
}
