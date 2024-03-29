#include "InputManager.h"

double Mouse::_x;
double Mouse::_y;
double Mouse::_xScroll;
double Mouse::_yScroll;

GLFWwindow* InputManager::_window;
std::vector<Bindings*> InputManager::_bindingsStack;
std::unordered_map<InputType, InputManager::InputState> InputManager::_inputStates;
std::vector<unsigned int> InputManager::_typedKeys;
std::vector<int> InputManager::_typedKeysMods;

std::vector<std::string> InputManager::_droppedPaths;

void InputManager::Init(GLFWwindow* window)
{
	_window = window;
	glfwSetKeyCallback(window, InputManager::KeyCallback);
	glfwSetCursorPosCallback(window, InputManager::MousePositionCallback);
	glfwSetCharModsCallback(window, InputManager::CharacterModCallback);
	glfwSetCursorEnterCallback(window, InputManager::MouseEnteredCallback);
	glfwSetScrollCallback(window, InputManager::MouseScrollCallback);
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);

	glfwSetDropCallback(window, InputManager::DroppedPathsCallback);

	_bindingsStack.push_back(new Bindings());
}

void InputManager::Update(TimeStep timeStep)
{
	if (_typedKeys.size())
	{
		_typedKeys.erase(_typedKeys.begin());
		_typedKeysMods.erase(_typedKeysMods.begin());
	}
}

void InputManager::LateUpdate(TimeStep timeStep)
{
	for (auto& inputState : _inputStates)
	{
		inputState.second.Update(timeStep);
	}
	_droppedPaths.clear();
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
	Mouse::SetDataPosition(x, y);
}

void InputManager::MouseEnteredCallback(GLFWwindow* window, int entered)
{
}

void InputManager::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Mouse::SetDataScroll(xOffset, yOffset);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS: UpdateType(InputType::Mouse, button, 1.0f); break;
	case GLFW_RELEASE: UpdateType(InputType::Mouse, button, 0.0f); break;
	}
}

void InputManager::DroppedPathsCallback(GLFWwindow* window, int count, const char** paths)
{
	_droppedPaths.clear();
	for (int i = 0; i < count; i++)
	{
		_droppedPaths.push_back(paths[i]);
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
	ResolvePushStates(bindings);
	_bindingsStack.push_back(bindings);
}

Bindings* InputManager::PopBindings()
{
	Bindings* b = _bindingsStack.back();
	_bindingsStack.erase(_bindingsStack.end() - 1);
	ResolvePopStates(b);
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

bool InputManager::GetClipboard(std::string& string)
{
	const char* text = glfwGetClipboardString(_window);
	if (text)
	{
		string = text;
		return true;
	}
	return false;
}

void InputManager::SetClipboard(std::string string)
{
	glfwSetClipboardString(_window, string.c_str());
}

bool InputManager::GetDroppedPaths(std::vector<std::string>& paths)
{
	if (_droppedPaths.size())
	{
		paths = _droppedPaths;
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

void InputManager::ResolvePushStates(Bindings* newBindings)
{
	if (_bindingsStack.size() == 0)
		return;
	std::unordered_map<std::string, bool> alreadySeen;
	for (int i = _bindingsStack.size() - 1; i >= 0; i--)
	{
		auto bindings = _bindingsStack[i];
		std::unordered_map<InputType, std::unordered_map<std::string, std::vector<std::string>>> toTargets;
		bindings->Conversions(*newBindings, toTargets);
		for (auto type : toTargets)
		{
			for (auto target : type.second)
			{
				bool seen = false;
				for (auto currentTarget : target.second)
				{
					if (alreadySeen.count(currentTarget))
					{
						seen = true;
						break;
					}
				}
				if (seen)
					continue;
				_inputStates[type.first].Replace(target.first, target.second);
				for (auto currentTarget : target.second)
				{
					alreadySeen[currentTarget] = true;
				}
			}
		}
	}
}

void InputManager::ResolvePopStates(Bindings* newBindings)
{
	if (_bindingsStack.size() == 0)
		return;
	std::unordered_map<std::string, bool> alreadySeen;
	for (int i = _bindingsStack.size() - 1; i >= 0; i--)
	{
		auto bindings = _bindingsStack[i];
		std::unordered_map<InputType, std::unordered_map<std::string, std::vector<std::string>>> toTargets;
		newBindings->Conversions(*bindings, toTargets);
		for (auto type : toTargets)
		{
			for (auto target : type.second)
			{
				if (alreadySeen.count(target.first)) 
					continue;

				_inputStates[type.first].Replace(target.first, target.second);
				alreadySeen[target.first] = true;
			}
		}
	}
}

void Mouse::SetPosition(float x, float y)
{
	glfwSetCursorPos(InputManager::_window, (double)x, (double)y);
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

void Bindings::Conversions(Bindings& targetBindings, 
	std::unordered_map<InputType,
		std::unordered_map<std::string, std::vector<std::string>>>& toTargets)
{
	for (auto type : _bindings)
	{
		for (auto currentBindings : type.second)
		{
			for (auto input : currentBindings.second)
			{
				std::vector<std::string> targetNames;
				if (targetBindings.TryGetBinding(type.first, input, targetNames))
				{
					toTargets[type.first][currentBindings.first] = targetNames;
				}
			}
		}
	}
}
