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

void process_key(GLFWwindow *window, int key, int scancode, int action, int mods){
	if(action == GLFW_RELEASE){
		switch(key){
			case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE);
						break;
			case GLFW_KEY_ENTER:
				{static unsigned int view_state = 1;
				switch(view_state){
					case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						view_state++; break;
					case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
						view_state=0; break;
					default:glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						view_state++; break;
				}break;}
		}
	}
}


int main()
{
	/* Initialize GLFW */
	glfwInit();

	/* Set minimum openGL version to 3.0 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	/* Create GLFW window. */
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGl", NULL, NULL);
	if(window == NULL){
		glfwTerminate();
		perror("Failed to create GLFW window.\n");
		return -1;
	}
	/* Make the current thread's main context the same ad the window's one. */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, process_key);

	/* Load openGL functions with GLAD. */
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		perror("Could not load openGL functions.\n Failed to initialize GLAD.\n");
		return -1;
	}

	/* Specify the rendering window to openGL. */
	/* (0, 0) at top left edge of the window. */
	glViewport(0, 0, 800, 600);

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	/* Used to index the vertices. */
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	/* Create a vertex array object with id of VAO.			*/
	/* It will contain:						*/
	/* 	- Setting of (enabled/disabled) vertex attribute.	*/
	/*	- Associated VBO and configuration.			*/
	/*	- Associated EBO.					*/
	/* Note: 1 specifies a size, not an id. Ids are assigned by openGL not the user. */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/* Create a vertex buffer object with id of VBO. */
	/* Its type is: GL_ARRAY_BUFFER */
	/* OpenGL allows only one type of buffer to be assigned at one time. */
	/* Note: There are many types of buffer objects. */
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	/* Create an EBO object. */
	/* Since this is a different type of buffer we can it assigned together with a VBO.*/
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	/* Bind VBO and EBO to VAO. */
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	/* since VAO was currently bound all this was connected to it. */


	/* We need to dynamically compile shaders because hardware implementations differ. */

	/* ----------------------------------VERTEX SHADER-------------------------------- */

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/* gl_Postition is a predefined vec4. */
	/* Binding location as "location = 0" is only supported from GLSL 1.4. */
	const char *vertexShaderSource = 
		 "#version 130\n"
		 "in vec3 aPos;\n"
		 "void main()\n"
		 "{\n"
		 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		 "}\0";

	/* Attach shader with shader object and compile. */
	/* 1 for how many strings the source is composed of. */
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/* Checking if the shader compiled. */
	/* Note: the source is in vertexShaderSource, vertexShader is an id. */
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stdout, "ERROR: Vertex Shader compilation.\n%s", infoLog);
		return -1;
	}

	/* --------------------------------FRAGMENT SHADER------------------------------- */

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	/* RGBA colour values are used. */
	const char *fragmentShaderSource =
		"#version 130\n"
		 "out vec4 FragColor;\n"
		 "void main(){\n"
		 "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		 "}\0";

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fprintf(stderr, "ERROR: Fragment Shader compilation.\n%s", infoLog);
		return -1;
	}

	/* ---------------------------------LINKING SHADERS------------------------------ */
	/*  Creating a shader program by linking the shaders.				  */

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/* Binding location as "location = 0" is only supported from GLSL 1.4.*/
	glBindAttribLocation(shaderProgram, 0, "vertexPosition_modelspace");

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "ERROR: Linking shaders.\n%s", infoLog);
		return -1;
	}

	/* Activating the program. */
	/* Every shader and rendering call will use now use this new shaderProgram. */
	glUseProgram(shaderProgram);

	/* No need to keep them. */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	
	/* ----------------------------------RENDERING----------------------------------- */

	glBindVertexArray(VAO);

	printf("Press enter and see!\n");

	/* Render loop. */
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* 0	- vertex attribute to use.	*/
		/* 3	- How many vertices to draw.	*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffer in double buffering. */
		glfwSwapBuffers(window);
		/* Checks for any event and call appropriate callback. */
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
