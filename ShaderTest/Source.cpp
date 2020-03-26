#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <mat4x4.hpp>
#include <gtc\matrix_transform.hpp>

#include "ShaderParser.h"

void cycle2(GLFWwindow* wind, unsigned int prog) {
	/*float vertices[] = {
				-0.5f, 0.4f, 0.0f,
				-0.1f, -0.4f, 0.0f,
				0.4f, 0.7f, 0.0f
	};*/

	/*float vertices[] = {
				-0.5f, 0.4f, 0.0f, 1.0f,
				-0.1f, -0.4f, 0.0f, 1.0f,
				0.4f, 0.7f, 0.0f, 1.0f
	};*/

	float vertices[] = {
				90.0f, 160.0f, 0.0f, 1.0f,
				220.0f, 135.0f, 0.0f, 1.0f,
				150.0f, 70.0f, 0.0f, 1.0f
	};

	float colors[] = {
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
	};

	unsigned int buffer[2];

	glGenBuffers(2, buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, colors, GL_STATIC_DRAW);

	unsigned int vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindVertexArray(vertexArray);

	unsigned int unif1Move = glGetUniformLocation(prog, "move");
	unsigned int unif2Projection = glGetUniformLocation(prog, "proj");

	//TRANSFORM MATRIX
	glm::mat4 transf = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//PROJECTION MATRIX
	glm::mat4 proj = glm::ortho(0.0f, 300.0f, 0.0f, 200.0f, -1.0f, 1.0f);

	if (unif1Move >= 0) {
		glUniformMatrix4fv(unif1Move, 1, GL_FALSE, &proj[0][0]);
	}

	if (unif2Projection >= 0) {
		glUniformMatrix4fv(unif2Projection, 1, GL_FALSE, &transf[0][0]);
	}

	while (!glfwWindowShouldClose(wind)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (unif1Move >= 0) {
			glUniformMatrix4fv(unif1Move, 1, GL_FALSE, &proj[0][0]);
		}

		if (unif2Projection >= 0) {
			glUniformMatrix4fv(unif2Projection, 1, GL_FALSE, &transf[0][0]);
		}

		/*if (glfwGetKey(wind, GLFW_KEY_A)) {
			transf[0][3] += 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_D)) {
			transf[0][3] -= 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_W)) {
			transf[1][3] -= 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_S)) {
			transf[1][3] += 0.05f;
		}*/

		if (glfwGetKey(wind, GLFW_KEY_A)) {
			transf[3][0] -= 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_D)) {
			transf[3][0] += 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_W)) {
			transf[3][1] += 0.05f;
		}
		if (glfwGetKey(wind, GLFW_KEY_S)) {
			transf[3][1] -= 0.05f;
		}


		//std::cout << proj[0][3] << "\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		glfwPollEvents();
		glfwSwapBuffers(wind);
	}
}

int main() {
	const char* vpath = "shaders\\vshader.shader";
	const char* fpath = "shaders\\fshader.shader";

	ShaderReader vsr(vpath);

	ShaderReader fsr(fpath);

	if (vsr.error || fsr.error) {
		std::cout << "no " << (vsr.error ? (fsr.error ? "vertex and fragment" : "vertex") : "fragment") << " shader had been found\n\n" << 
			"they should be in \"*filepath*\\shaders\"\n\n" << 
			"close console to continue...\n\n";
		while(true){}
	}

	glfwInit();

	GLFWwindow* wind = glfwCreateWindow(300, 200, "pes", 0, 0);
	glfwMakeContextCurrent(wind);

	glewInit();

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

			cycle2(wind, prog);
		}
	}
	glfwTerminate();
	return 0;
}