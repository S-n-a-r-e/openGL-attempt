#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "ShaderParser.h"

void cycle2(GLFWwindow* wind) {
	float vertices[] = {
				-0.5f, 0.4f, 0.0f,
				-0.1f, -0.4f, 0.0f,
				0.4f, 0.7f, 0.0f,
	};
	float colors[] = {
				1.0f, 0.0f, 0.6f,
				0.8f, 1.0f, 0.0f,
				0.6f, 0.2f, 1.0f
	};

	unsigned int buffer[2];

	glGenBuffers(2, buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, colors, GL_STATIC_DRAW);

	unsigned int vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindVertexArray(vertexArray);

	while (!glfwWindowShouldClose(wind)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(wind);
	}
}

int main() {
	glfwInit();

	GLFWwindow* wind = glfwCreateWindow(300, 200, "pes", 0, 0);
	glfwMakeContextCurrent(wind);

	glewInit();

	const char* path = "C:\\Users\\User\\Desktop\\vshader.shader";
	const char* fpath = "C:\\Users\\User\\Desktop\\fshader.shader";

	ShaderReader vsr(path);

	ShaderReader fsr(fpath);

	unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, &vsr.source.word, NULL);
	glCompileShader(vshader);

	unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, &fsr.source.word, NULL);
	glCompileShader(fshader);
	
	int res;
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &res);
	int res2;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &res2);

	if ((res != 1) || (res2 != 1)) {
		int length;
		glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &length);
		char* ch = new char[length];
		glGetShaderInfoLog(fshader, length, &length, ch);
		std::cout << ch << "\n";
		delete[] ch;

		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &length);
		ch = new char[length];
		glGetShaderInfoLog(vshader, length, &length, ch);
		std::cout << ch << "\n";
		delete[] ch;
	}
	else {
		std::cout << "compile ok\n\n";

		unsigned int prog = glCreateProgram();
		glAttachShader(prog, vshader);
		glAttachShader(prog, fshader);

		glLinkProgram(prog);

		int lres;
		glGetProgramiv(prog, GL_LINK_STATUS, &lres);
		if (lres != 1) {
			int length;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
			char* ch = new char[length];
			glGetProgramInfoLog(prog, length, &length, ch);
			std::cout << ch << "\n";
			delete[] ch;
		}
		else {
			std::cout << "link ok\n\n";
			glUseProgram(prog);

			cycle2(wind);
		}
	}

	return 0;
}