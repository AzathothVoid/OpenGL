#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	//initializes glfw
	glfwInit();
	//sets major version, minor version, and profile type of OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creates a window handle 
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	//checks if window creation has an error or not
	if (window == NULL) {
		std::cout << "Failed To Create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//it declares the current window as the context of openGL
	glfwMakeContextCurrent(window);

	//loads and initializes glad and checks if it encounters any error
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed To Initialize Glad" << std::endl;
		return -1;
	}

	//sets viewport of openGL
	glViewport(0, 0, 800, 600);

	//--------------------------------------------------------------------
	//-------------------------------------------------------------------
								//SHADER PROGRAM//

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------


	// data about the vertices of triagle
	float vertices[] = {
		 0.0f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
										//VAO//
	// create a vertex array object that stores vertex attrib pointers and vertex buffer objects associated with it
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // generates a vertex array and stores unique ID in VAO that defines it
	glBindVertexArray(VAO); // binds the VAO to the state variable in openGL

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
										//VBO//

	// creates a vertex Buffer Object that stores vertex data in itself in the GPU. this is the unique ID
	unsigned int VBO;
	glGenBuffers(1, &VBO); // Generates a buffer and stores unique ID in VBO that defines it
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds the VBO to the ARRAY BUFFER state variable in openGL

	// Stores data in the VBO from the vertices array defined above
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//VERTEX ATTRIBS//
	// specifies the vertex attributes (specified in the vertex shader) of the vertex data given in the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//UNBINDING VAO AND VBO//
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding this does not effect the program because VAO remembers the VBO associated with it

	//wirefram polygon mode
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

	// main render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//uses the shaderprogram made
		ourShader.use();
		ourShader.setFloat("offset", -0.3);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//deletes shader program and buffers after they have been linked.
	ourShader.discard();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}


// Callback function that resizes the viewport of OpenGL context if window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//callback function that deals with input, such as keystrokes etc.
void processInput(GLFWwindow* window) {
	//shuts down window if escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}