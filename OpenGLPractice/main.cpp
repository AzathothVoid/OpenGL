#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
#include "Camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double posX, double posY);
void scroll_callback(GLFWwindow* window, double posX, double posY);

float mixVal = 0.2f;

float WHeight = 600.0f;
float WWidth = 800.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WWidth / 2.0f;
float lastY = WHeight / 2.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;


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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//it declares the current window as the context of openGL
	glfwMakeContextCurrent(window);

	//loads and initializes glad and checks if it encounters any error
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed To Initialize Glad" << std::endl;
		return -1;
	}

	//sets viewport of openGL

	//--------------------------------------------------------------------
	//-------------------------------------------------------------------
								//SHADER PROGRAM//

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------


	// data about the vertices of triagle
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//VERTEX ATTRIBS//
	// specifies the vertex attributes (specified in the vertex shader) of the vertex data given in the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);



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


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//TRANSFORMATION MATRICES//

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -4.0f),
		glm::vec3(0.0f,  0.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, -6.0f),
		glm::vec3(0.0f,  0.0f, -7.0f),
		glm::vec3(0.0f,  0.0f, -8.0f),
		glm::vec3(0.0f,  0.0f, -9.0f)
	};

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// main render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		float currentframe =(float) glfwGetTime();
		deltaTime = currentframe - lastFrame;
		lastFrame = currentframe;

		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//uses the shaderprogram made
		ourShader.use();
		ourShader.setFloat("offset", 0.0);
		glBindVertexArray(VAO);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.setFloat("mixer", mixVal);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		glm::mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		
		//ourShader.setMat4("transform", trans);
		
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float xvalue = 0.0f;

			float angle = 0.0f;
			model = glm::rotate(model,  glm::radians(angle) , glm::vec3(0.5f, 1.0f, 0.0f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			for (unsigned int j = 0; j < 32; j++) {

				//creating more containgers above the originals and fake
				glm::mat4  trans = glm::mat4(1.0f);
				trans = glm::translate(trans, cubePositions[i] + glm::vec3(0.0f + xvalue, 1.0f, 0.0f));
				ourShader.setMat4("model", trans);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				xvalue += 1.0f;

				//creating more containers on the side of the original
				trans = glm::mat4(1.0f);
				trans = glm::translate(trans, cubePositions[i] + glm::vec3(0.0f + xvalue, 0.0f, 0.0f));
				ourShader.setMat4("model", trans);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				
			}
		}
	
		 
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

	const float cameraSpeed = 5.0f * deltaTime;;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FILL, GL_LINE);
	}
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
	float FposX = static_cast<float>(posX);
	float FposY = static_cast<float>(posY);
	
	if (firstMouse) {
		lastX = FposX;
		lastY = FposY;
		firstMouse = false;
	}

	float xoffset = FposX - lastX;
	float yoffset = lastY - FposY;

	lastX = FposX;
	lastY = FposY;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double posX, double posY) {
	camera.ProcessMouseScroll(posY);
}