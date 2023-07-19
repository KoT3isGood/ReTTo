
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include <iostream>
#include "Output.h"
#include <string>

#include <glad.c>
#include <GLFW/glfw3.h>

#include "..\Game\GameSettings.h"

#include "toggle.h"

#include <windows.h>
#include <shellapi.h>



bool toggle(bool input) {
	if (input == 1) {
		return 0;
	}
	return 1;
}


namespace CoreTick {
	int onUpdate() {
		return 0;
	}
}


namespace Core {
	namespace ImGuiBridge {
		void openInfo() {

		}

		bool infoOpened = false;
		char url[1000] = "https://www.google.com";

		void runImGui() {
			
			ImGui::BeginMainMenuBar();

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create")) {
					print<std::string>(info, "user has pressed Create");
				}
				if (ImGui::MenuItem("Open")) {
					print<std::string>(info, "user has pressed Open");
				}
				if (ImGui::MenuItem("Info")) {
					print<std::string>(info, "user has pressed Info");

					infoOpened = toggle(infoOpened);
					

					
				}

				ImGui::EndMenu();
			}


			ImGui::EndMainMenuBar();

				if (infoOpened) {
					ImGui::Begin("Hi", &infoOpened);

					ImGui::Text("Hi there, I'm KoT3, developer of this game engine");
					if (ImGui::Button("Documentation")) {
						ShellExecute(0, 0, L"https://github.com/KoT3isGood/ReTTo/wiki", 0, 0, SW_SHOW);
					};
					if (ImGui::Button("Patreon")) {
						ShellExecute(0, 0, L"https://www.patreon.com/kotofyt/membership", 0, 0, SW_SHOW);
					};
					ImGui::End();

				}

				



		}

	}


	int runWindow() {
		GLFWwindow* window;

		if (!glfwInit()) {
			print<std::string>(error, "GLFW Wasn't Inited");
			return -1;
		}

		window = glfwCreateWindow(GameSettings::resolution[0], GameSettings::resolution[1], GameSettings::title, NULL, NULL);
		if (!window) {
			glfwTerminate();
			print<std::string>(error, "Window wasn't created");
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			print<std::string>(error, "GLAD wasn't inited");
		}

		glClearColor(1, 0, 0, 0);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGuiBridge::runImGui();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
			CoreTick::onUpdate();
		};
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		return 0;
	};

}