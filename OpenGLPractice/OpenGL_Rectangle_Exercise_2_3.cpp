#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// Vertex Shader source code, written in OpenGL Shading Language(GLSL)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment Shader source code, written in OpenGL Shading Language(GLSL)
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


// Callback function that resizes the viewport of OpenGL context if window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//callback function that deals with input, such as keystrokes etc.
void processInput(GLFWwindow* window) {
	//shuts down window if escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

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

	// creates a vertex shader that is to compiled and runs on the GPU as a shader program. This is the unique id
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates a shader and stores unique ID in vertexShader that defines it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attaches vertex shader code to vertexShader variable
	glCompileShader(vertexShader); // compiles the vertex shader just created

	//checks if the vertex shader encounters an error in compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// creates a fragment shader that is to compiled and runs on the GPU as a shader program. This is the unique id
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);// creates a shader and stores unique ID in fragmentShader that defines it
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);// attaches fragment shader code to fragmentShader variable
	glCompileShader(fragmentShader1); // compiles the fragment shader just created

	unsigned fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	//checks if the framgnet shader encounters an error in compilation
	int success2;
	char infoLog2[512];
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success2);
	if (!success2) {
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog2);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
	}

	// creates a shader program that is to be run on the GPU that actually runs the shaders
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram(); // creates a shader program and stores its unique ID in shaderProgram
	glAttachShader(shaderProgram1, vertexShader); // attaches vertex shader to the shader program
	glAttachShader(shaderProgram1, fragmentShader1);// attaches fragment shader to the shader program
	glLinkProgram(shaderProgram1); // links the attached shaders together

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	//deletes shaders after they have been linked.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------


	// data about the vertices of triagle
	float vertices1[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,  // top left 

	};

	float vertices2[] = {
		// second triangle
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
										//VAO//
	// create a vertex array object that stores vertex attrib pointers and vertex buffer objects associated with it
	unsigned int VAOs[2];

	glGenVertexArrays(2, VAOs);// generates a vertex array and stores unique ID in VAO that defines it
	glBindVertexArray(VAOs[0]); // binds the VAO to the state variable in openGL

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
										//VBO//

	// creates a vertex Buffer Object that stores vertex data in itself in the GPU. this is the unique ID
	unsigned int VBOs[2];
	glGenBuffers(2, VBOs); // Generates a buffer and stores unique ID in VBO that defines it
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // Binds the VBO to the ARRAY BUFFER state variable in openGL

	// Stores data in the VBO from the vertices array defined above
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//VERTEX ATTRIBS//
	// specifies the vertex attributes (specified in the vertex shader) of the vertex data given in the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//setting up VAO2 
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
											//UNBINDING VAO AND VBO//
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding this does not effect the program because VAO remembers the VBO associated with it



	// main render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//uses the shaderprogram made
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAOs[1]);
		glUseProgram(shaderProgram2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//deletes shader program and buffers after they have been linked.
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}
;