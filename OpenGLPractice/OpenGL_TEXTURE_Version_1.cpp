#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* windo);

float mixVal = 0.2;

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
		 0.5f,  0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,	0.0f, 1.0f
	};
	int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3  //second traingle
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
										//EBO//
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//VERTEX ATTRIBS//
	// specifies the vertex attributes (specified in the vertex shader) of the vertex data given in the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);



	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//UNBINDING VAO AND VBO//
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding this does not effect the program because VAO remembers the VBO associated with it

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//TEXTURE MAPPING//

	//TEXTURE 1
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrchannels;
	unsigned char* data = stbi_load("images\\container.jpg", &width, &height, &nrchannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);

	//TEXTURE 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("images\\awesomeface.png", &width, &height, &nrchannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use();

	ourShader.setInt("ourTexture1", 0);
	ourShader.setInt("ourTexture2", 1);

	// main render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//uses the shaderprogram made
		ourShader.use();
		ourShader.setFloat("offset", 0.0);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.setFloat("mixer", mixVal);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixVal += 0.001f;
		if (mixVal >= 1.0f) {
			mixVal = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixVal -= 0.001f;
		if (mixVal <= 0.0f) {
			mixVal = 0.0f;
		}

	}

}