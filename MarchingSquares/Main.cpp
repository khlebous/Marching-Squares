#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include <iostream>
#include <imgui\imgui.h>
#include "imgui_impl_glfw.h"

#include <stdio.h>
#include <algorithm>
#include "GraphicsLibrary.h"
#include "MarchingSquares.h"
#include "Point.h"
#include "Square.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void LoadNewTexture(unsigned char **data, const char* src, unsigned int* texture, int* nrChannels);

using namespace std;

char new_image_path[30] = "3.png";

int SCR_WIDTH = 512;
int SCR_HEIGHT = 512;
int STEP = 20;
int THRESHHOLD = 120;

GLFWwindow* window;
void error_callback(int error, const char* description);


int main(int, char**)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	glfwInit();
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Marching Cubes", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1); // Enable vsync
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	ImGui_ImplGlfwGL2_Init(window, true);
	//=========================================
	float vertices[] = {
		// positions          // texture coords
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//=========================================
	unsigned int texture;
	int nrChannels;
	unsigned char* data = nullptr;
	LoadNewTexture(&data, new_image_path, &texture, &nrChannels);
	//=========================================
	Shader ourShader("texture.vs", "texture.fs");
	vector<Line> lines;
	//=========================================
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		ourShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(0);

		GL::Draw(lines);
		ImGui_ImplGlfwGL2_NewFrame();
		{
			ImGui::InputText("Path", new_image_path, IM_ARRAYSIZE(new_image_path));
			if (ImGui::Button("Load new image"))
			{
				stbi_image_free(data);
				lines.clear();
				LoadNewTexture(&data, new_image_path, &texture, &nrChannels);
			}
			ImGui::Text("---------------");
			if (ImGui::SliderInt("step", &STEP, 5, min(SCR_WIDTH, SCR_HEIGHT)))
				lines = MS::MarchingSquares(data);
			ImGui::Text("---------------");
			if (ImGui::SliderInt("threshhold", &THRESHHOLD, 0, 255))
				lines = MS::MarchingSquares(data);
			ImGui::Text("---------------");
			if (ImGui::Button("Marching Squares"))
			{
				lines = MS::MarchingSquares(data);
			}

		}
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		processInput(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	stbi_image_free(data);
	ImGui_ImplGlfwGL2_Shutdown();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}


void LoadNewTexture(unsigned char **data, const char* src, unsigned int* texture, int* nrChannels)
{
	glBindTexture(GL_TEXTURE_2D, *texture);

	stbi_set_flip_vertically_on_load(true);
	*data = stbi_load(src, &SCR_WIDTH, &SCR_HEIGHT, nrChannels, 0);
	glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
	if (*data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, *data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

}



