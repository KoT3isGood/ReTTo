
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Output.h"

#include <glad.c>
#include <GLFW/glfw3.h>

#include "..\Game\GameSettings.h"

#include "toggle.h"

#include <windows.h>
#include <shellapi.h>

#include "ImGuiBridge.cpp"

// IMGUI BRIDGE
bool toggle(bool input) {
	if (input == 1) {
		return 0;
	}
	return 1;
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
}

// CORE


namespace CoreTick {
	int onUpdate() {
		return 0;
	}
}


namespace Core {
	


	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec2 texCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   texCoord = aColor.xy;\n"
		"}\0";



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

		

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		std::string fragmentCode;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			fShaderFile.open("shader.fsh");
			std::stringstream fShaderStream;
			// read file's buffer contents into streams
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			fShaderFile.close();
			// convert stream into string
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* fShaderCode = fragmentCode.c_str();



		// build and compile our shader program
	// ------------------------------------
	// vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			print(error, "Vertex Shader Error");
			print(error, infoLog);
		}
		// fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			print(error, "Fragment Shader Error");
			print(error, infoLog);
		}


		

		// link shaders
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			print(error, "Shader Linker Error");
			print(error, infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions         // colors
			-1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,    // top left
			 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // top right
			-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom left

			1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // top right
			-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom left
		    1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.0f,   // bottom left		
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);


		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		while (!glfwWindowShouldClose(window)) {
			
			

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGuiBridge::runImGui();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// draw our first triangle
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// glBindVertexArray(0); // no need to unbind it every time 

			

			glfwSwapBuffers(window);
			glfwPollEvents();
			CoreTick::onUpdate();
		};
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glfwTerminate();
		return 0;
	};

}