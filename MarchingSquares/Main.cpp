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
#include "Point.h"
#include "Square.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

using namespace std;
void LoadNewTexture(unsigned char *data, const char* src);
vector<Line> MarchingSquares(unsigned char * data);

unsigned int texture;
char new_image_path[30] = "Lenna.png";

int SCR_WIDTH = 512;
int SCR_HEIGHT = 512;
GLFWwindow* window;
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int nrChannels;
int step = 20;
int threshhold = 120;
vector<Line> lines;

int main(int, char**)
{

#pragma region SetupWindow
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
#pragma endregion

	//=========================================

	Shader ourShader("texture.vs", "texture.fs");
#pragma region Initialize buffors
	float vertices[] = {
		// positions          // texture coords
		1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
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


	// load and create a texture 
	// -------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load("3.png", &SCR_WIDTH, &SCR_HEIGHT, &nrChannels, 0);
	glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

#pragma endregion

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//=========================================

	// Main loop
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
				LoadNewTexture(data, new_image_path);
			}
			ImGui::Text("---------------");
			if (ImGui::SliderInt("step", &step, 5, min(SCR_WIDTH, SCR_HEIGHT)))
				lines = MarchingSquares(data);
			ImGui::Text("---------------");
			if (ImGui::SliderInt("threshhold", &threshhold, 0, 255))
				lines = MarchingSquares(data);
			ImGui::Text("---------------");
			if (ImGui::Button("Marching Squares"))
			{
				lines = MarchingSquares(data);
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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void LoadNewTexture(unsigned char *data, const char* src)
{

	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(src, &SCR_WIDTH, &SCR_HEIGHT, &nrChannels, 0);
	glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

}


pair<Point, bool>** GetSamples(unsigned char* data)
{
	//if (step < 1 || threshhold < 0 || threshhold > 255)
	//	throw new InvalidOperationException();
	int h = (SCR_HEIGHT - 1) / step + 1;

	pair<Point, bool>**  result = new pair<Point, bool>*[h];
	for (size_t i = 0; i < h; i++)
		result[i] = new pair<Point, bool>[(SCR_WIDTH - 1) / step + 1];

	for (int i = 0; i * step < SCR_HEIGHT; i++)
	{
		for (int j = 0; j * step < SCR_WIDTH; j++)
		{
			//R==G==B
			int colour = (data[i * step * 3 * SCR_WIDTH + j * step * 3] +
				data[i * step * 3 * SCR_WIDTH + j * step * 3 + 1] +
				data[i * step * 3 * SCR_WIDTH + j * step * 3 + 2]) / 3;
			bool value = colour <= threshhold;
			//bool value = false;
			auto point = make_pair(Point(i * step, j * step), value);
			result[i][j] = point;
		}
	}

	return result;
}

Square** GetSquares(pair<Point, bool>** samples)
{
	// if (samples.GetLength(0) < 2 || samples.GetLength(1) < 2)
	//	 throw new InvalidOperationException();


	int w = (SCR_WIDTH - 1) / step + 1;
	int h = (SCR_HEIGHT - 1) / step + 1;

	Square** squares = new Square*[h];
	for (size_t i = 0; i < h; i++)
		squares[i] = new Square[w];

	for (int i = 0; i < h - 1; i++)
		for (int j = 0; j < w - 1; j++)
		{
			int value = 0;
			if (samples[i][j].second) value += 8;
			if (samples[i + 1][j].second) value += 4;
			if (samples[i + 1][j + 1].second) value += 2;
			if (samples[i][j + 1].second) value += 1;

			squares[i][j] = Square(value, samples[i][j].first, samples[i + 1][j + 1].first);
		}

	/*for (size_t i = 0; i < h; i++)
		delete[] samples[i];
	delete[] samples;*/

	return squares;
}

vector<Line> GetLines(Square**  squares)
{
	vector<Line> lines = vector<Line>();

	int w = (SCR_WIDTH - 1) / step + 1;
	int h = (SCR_HEIGHT - 1) / step + 1;

	for (int i = 0; i < h - 1; i++)
	{
		for (size_t j = 0; j < w - 1; j++)
		{
			Square* square = &squares[i][j];
			for (Line &line : (*square).GetLines())
			{
				int lineIndex = distance(lines.begin(), find(lines.begin(), lines.end(), line));
				if (lineIndex >= lines.size())
					lines.push_back(line);
				else
					lines.erase(lines.begin() + lineIndex);
			}
		}
	}

	/*for (size_t i = 0; i < h; i++)
		delete[] squares[i];
	delete[] squares;*/
	return lines;
}

vector<Line> MarchingSquares(unsigned char * data)
{
	auto samples = GetSamples(data);
	auto squares = GetSquares(samples);
	auto lines = GetLines(squares);
	//return vector<Line>();
	return lines;
}
