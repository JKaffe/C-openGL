/*  Copyright 2020 Karmjit Mahil.
    This file is part of C-openGL.

    C-openGL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    C-openGL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with C-openGL. If not, see <https://www.gnu.org/licenses/>
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow * getGLFWwindow(){
	/* Set minimum openGL version to 3.0 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	/* Create GLFW window. */
	return glfwCreateWindow(800, 600, "LearnOpenGl", NULL, NULL);
}

unsigned int createVAO(float vertices[], size_t length){
	/* Create a vertex array object with id of VAO.			*/
	/* It will contain:						*/
	/* 	- Setting of (enabled/disabled) vertex attribute.	*/
	/*	- Associated VBO and configuration.			*/
	/*	- Associated EBO.					*/
	/* Note: 1 specifies a size, not an id.				*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/* Create a vertex buffer object with id of VBO. */
	/* Its type is: GL_ARRAY_BUFFER */
	/* OpenGL allows only one type of buffer to be assigned at one time. */
	/* Note: There are many types of buffer objects. */
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	/* Bind VBO to VAO. */
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STATIC_DRAW);

	return VAO;
}

int createShader(int type, const char* source){
	int shader;
	shader = glCreateShader(type);

	/* Attach shader with shader object and compile. */
	/* 1 for how many strings the source is composed of. */
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	/* Checking if the shader compiled. */
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stdout, "ERROR:  Shader compilation.\n%s", infoLog);
		return -1;
	}

	return shader;
}

int main(){
	/* Initialize GLFW */
	glfwInit();

	GLFWwindow * window = getGLFWwindow();
	if(window == NULL){
		glfwTerminate();
		perror("Failed to create GLFW window.\n");
		return -1;
	}

	glfwMakeContextCurrent(window);

	/* Load openGL functions with GLAD. */
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		perror( "Could not load openGL functions.\n"
			"Failed to initialize GLAD.\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	float leftTriangle[] = {
		-0.75f,	-0.5f,	0.0f,
		-0.25f,	-0.5f,	0.0f,
		-0.5f ,	 0.0f,	0.0f
	};

	unsigned int VAO1 = createVAO(leftTriangle, sizeof(leftTriangle));
	glVertexAttribPointer(	0,	/* Vertex attribute index */
				3,
				GL_FLOAT,
				GL_FALSE,
				3 * sizeof(float),
				(void*) 0);

	glEnableVertexAttribArray(0);

	float rightTriangle[] = {
		0.75f,	0.0f,	0.0f,
		0.25f,	0.0f,	0.0f,
		0.5f,	-0.5f,	0.0f
	};

	unsigned int VAO2 = createVAO(rightTriangle, sizeof(rightTriangle));
	glVertexAttribPointer(	0,	/* Vertex attribute index */
				3,
				GL_FLOAT,
				GL_FALSE,
				3 * sizeof(float),
				(void*) 0);

	glEnableVertexAttribArray(0);


	/* ------------------------------SHADERS----------------------------- */

	const char *vertexShaderSource = 
		 "#version 130\n"
		 "in vec3 aPos;\n"
		 "out vec4 colour;\n"
		 "void main()\n"
		 "{\n"
		 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		 	"colour = gl_Position;\n"
		 "}\0";
	int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	if(vertexShader == -1)
		return -1;

	/* RGBA colour values are used. */
	const char *fragmentShaderSource =
		"#version 130\n"
		"in vec4 colour;\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"	FragColor = abs(colour.xxxx);\n"
		"}\0";
	int fragmentShader;
	fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if(fragmentShader == -1)
		return -1;

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/* Binding location as "location = 0" is only supported from GLSL 1.4.*/
	glBindAttribLocation(shaderProgram, 0, "vertexPosition_modelspace");

	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "ERROR: Linking shaders.\n%s", infoLog);
		return -1;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* -----------------------------RENDERING--------------------------- */

	while(!glfwWindowShouldClose(window)){

		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO1);
		/* 0	- vertex attribute to use.	*/
		/* 3	- How many vertices to draw.	*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffer in double buffering. */
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	//TODO: delete VBO
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}
