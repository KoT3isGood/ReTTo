
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


int maxBounces = 8;
int raysPerPixel = 1;
float CameraYaw = 180;
float Emmision = 0;
float CameraPitch = 0;
float CameraPos[3] = {3,0,0};

bool isInteractable = true;

int triangleOffset = 0;


float SpherePosition[] = {
	-10.0,0.0,0.0,
	0,0,0.0,
	0,0.0,11.0,
};

float SphereColor[] = {
	1.0,1.0,1.0,
	0.0,1.0,0.0,
	0.0,0.0,1.0,
};
float SphereSize[] = {
	7.5,
	1,
	10,
};
float SphereEmmision[] = {
	1.0,
	0.0,
	0.0,
};



namespace Core {
	namespace ImGuiBridge {
		


		bool infoOpened = false;
		bool triangleEditorOpened = false;
		bool rayTracerOptions = false;


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

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Sphere Editor")) {


					triangleEditorOpened = toggle(triangleEditorOpened);
				}

				if (ImGui::MenuItem("Ray Tracing Options")) {


					rayTracerOptions = toggle(rayTracerOptions);
				}
				ImGui::EndMenu();

			}


			ImGui::EndMainMenuBar();

			if (infoOpened) {
				ImGui::Begin("ReTTo 1.0.0", &infoOpened);

				ImGui::Text("Hi there, I'm KoT3, developer of this game engine");
				if (ImGui::Button("Documentation")) {
					ShellExecute(0, 0, L"https://github.com/KoT3isGood/ReTTo/wiki", 0, 0, SW_SHOW);
				};
				if (ImGui::Button("Patreon")) {
					ShellExecute(0, 0, L"https://www.patreon.com/kotofyt/membership", 0, 0, SW_SHOW);
				};
				ImGui::Text("ReTTo - 1.0.0");
				ImGui::End();

			}
			/*if (triangleEditorOpened) {
				ImGui::Begin("Triangle Editor", &triangleEditorOpened);

				ImGui::DragInt("TrianglePicker", &triangleOffset);
				ImGui::Text("Color");
				ImGui::DragFloat("R", &data_color1[0 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("G", &data_color1[1 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("B", &data_color1[2 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::Text("Vertex1");
				ImGui::DragFloat("1X", &data_pos1[0+triangleOffset*9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("1Y", &data_pos1[1 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("1Z", &data_pos1[2 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::Text("Vertex2");
				ImGui::DragFloat("2X", &data_pos1[3 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("2Y", &data_pos1[4 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("2Z", &data_pos1[5 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::Text("Vertex3");
				ImGui::DragFloat("3X", &data_pos1[6 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("3Y", &data_pos1[7 + triangleOffset*9], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("3Z", &data_pos1[8 + triangleOffset * 9], 0.01f, -100.0f, 100.0f);

				if(ImGui::Button("PrintArray")) {

					std::cout << data_pos1[0 + triangleOffset * 9] << ", " << data_pos1[1 + triangleOffset * 9] << ", " << data_pos1[2 + triangleOffset * 9] << "," << "\n";
					std::cout << data_pos1[3 + triangleOffset * 9] << ", " << data_pos1[4 + triangleOffset * 9] << ", " << data_pos1[5 + triangleOffset * 9] << "," << "\n";
					std::cout << data_pos1[6 + triangleOffset * 9] << ", " << data_pos1[7 + triangleOffset * 9] << ", " << data_pos1[8 + triangleOffset * 9] << "," << "\n";
				}

				ImGui::End();
			}*/

			if (triangleEditorOpened) {
				ImGui::Begin("Sphere Editor", &triangleEditorOpened);

				ImGui::DragInt("Picked Sphere", &triangleOffset);


				ImGui::Text("Material");
				ImGui::DragFloat("R", &SphereColor[0 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("G", &SphereColor[1 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("B", &SphereColor[2 + triangleOffset * 3], 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Emmision", &SphereEmmision[triangleOffset], 0.001f, 0.0f, 1.0f);

				ImGui::Text("Position");
				ImGui::DragFloat("X", &SpherePosition[0 + triangleOffset * 3], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("Y", &SpherePosition[1 + triangleOffset * 3], 0.01f, -100.0f, 100.0f);
				ImGui::DragFloat("Z", &SpherePosition[2 + triangleOffset * 3], 0.01f, -100.0f, 100.0f);

				ImGui::Text("Size");
				ImGui::DragFloat("Size", &SphereSize[triangleOffset],0.01f);

				ImGui::End();
			}

			if (rayTracerOptions) {
				ImGui::Begin("Ray Tracing Options", &rayTracerOptions);
				ImGui::DragFloat("Camera Yaw", &CameraYaw,0.05f);
				ImGui::DragFloat("Camera Pitch", &CameraPitch, 0.005f, -1.5707963f, 1.5707963f);
				ImGui::DragFloat3("Camera Pos", CameraPos, 0.05f);


				ImGui::DragInt("Max Bounces", &maxBounces,1.0f,1,30);
				ImGui::DragInt("Amount Of Rays Per Pixel", &raysPerPixel, 1.0f, 1, 30);
				ImGui::End();
			}

		}

	}
}

// CORE

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		isInteractable = toggle(isInteractable);
		print(info, isInteractable);
	}

}


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


		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPos(window, GameSettings::resolution[0] / 2, GameSettings::resolution[1] / 2);
		// render loop
		while (!glfwWindowShouldClose(window)) {
			
			



			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			double xpos, ypos;

			glfwGetCursorPos(window, &xpos, &ypos);
			xpos = xpos / GameSettings::resolution[0] * 2 - 1;
			ypos = ypos / GameSettings::resolution[1] * 2 - 1;


			if (isInteractable) {
				glfwSetCursorPos(window, GameSettings::resolution[0] / 2, GameSettings::resolution[1] / 2);
				CameraPitch += -float(ypos);
				CameraYaw += float(xpos);
			}
			

			glUniform3fv(glGetUniformLocation(shaderProgram, "spherePosition"), sizeof(SpherePosition) / 4, SpherePosition);
			glUniform3fv(glGetUniformLocation(shaderProgram, "sphereColor"), sizeof(SphereColor) / 4, SphereColor);
			glUniform1fv(glGetUniformLocation(shaderProgram, "sphereSize"), sizeof(SphereSize) / 4, SphereSize);
			glUniform1fv(glGetUniformLocation(shaderProgram, "sphereEmmision"), sizeof(SphereEmmision) / 4, SphereEmmision);


			glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), GameSettings::resolution[0], GameSettings::resolution[1]);

			glUniform1i(glGetUniformLocation(shaderProgram, "u_maxBounces"), maxBounces);
			glUniform1i(glGetUniformLocation(shaderProgram, "u_raysPerPixel"), raysPerPixel);
			glUniform1i(glGetUniformLocation(shaderProgram, "u_time"), glfwGetTime());

			glUniform1f(glGetUniformLocation(shaderProgram, "u_cameraYaw"), CameraYaw);
			glUniform1f(glGetUniformLocation(shaderProgram, "u_emmision"), Emmision);
			glUniform1f(glGetUniformLocation(shaderProgram, "u_cameraPitch"), CameraPitch);
			glUniform3f(glGetUniformLocation(shaderProgram, "u_cameraPos"), CameraPos[0], CameraPos[1], CameraPos[2]);

			//glUniform1i(glGetUniformLocation(shaderProgram, "u_totalTriangles"), sizeof(data_pos1) / 36);

			glUniform1i(glGetUniformLocation(shaderProgram, "u_currentTriangle"), triangleOffset);
			


			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// draw our first triangle
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// glBindVertexArray(0); // no need to unbind it every time 

			if (!isInteractable) {
				ImGuiBridge::runImGui();
			}
			

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			

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
