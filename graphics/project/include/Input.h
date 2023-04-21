#pragma once

#include "InputListener.h"

#include <imgui/imgui.h>

#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <vector>

class Input
{
private:
	// default bindings
	static std::map <int, InputAction> inputmap;

	// raw state machine
	static std::pair<bool, bool> pressed[400];

	static std::map <InputAction, InputState> inputStates;

	static void setInputState(InputAction e, InputState s) {
		Input::inputStates[e] = s;	//bad

		// do listener function on press/release
		if (s == InputState::Press || s == InputState::Release) {
			InputEvent r = InputEvent(e, s);
			Broadcast(r);
		}
	}


	// observer/listener pattern
	static std::vector<InputListener*> listeners;

	static void Broadcast(const InputEvent msg) {
		for (InputListener* li : Input::listeners) {
			li->OnEvent(msg);
		}
	}

public:
	static void init() {
		// populate states map
		for (int i = 0; i <= 11; i++)
		{
			InputAction e = static_cast<InputAction>(i);

			Input::inputStates.insert(std::pair<InputAction, InputState>(e, InputState::None));
		}
	}

	static InputState GetInputState(InputAction e) {
		return Input::inputStates[e];
	}

	static void AddListener(InputListener* p) {

		Input::listeners.push_back(p);
	}
	static void Clear() {
		Input::listeners.clear();
	}



	static void handle(bool gui) {
		if(gui) ImGui::Begin("input");
		
		for (const auto &e : inputmap) {
			std::string v = "nothing";
			const std::pair<bool, bool>* istate = &Input::pressed[e.first];

			if (istate->first && istate->second) {	// held
				setInputState(e.second, InputState::Hold);
				v = "held";
			}
			else if (istate->first) {				// press
				setInputState(e.second, InputState::Press);
				v = "press";
			}
			else if (istate->second) {				// release
				setInputState(e.second, InputState::Release);
				v = "released";
			}
			else {
				setInputState(e.second, InputState::None);
				continue;
			}

			if (gui) ImGui::Text((std::string("") + std::to_string(e.first) + v).c_str());

			// progress state:
			Input::pressed[e.first].second = Input::pressed[e.first].first;
		}

		if (gui) ImGui::End();
	}

	static void keyListener(GLFWwindow* win, int key, int scancode, int action, int mods) {

		if (action == GLFW_PRESS) {
			Input::pressed[key].first = true;
		}
		else if (action == GLFW_RELEASE) {
			Input::pressed[key].first = false;
		}


		// edge cases
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(win, GL_TRUE);
		}
	}

};