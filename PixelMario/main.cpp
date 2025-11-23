#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//fragment shader source
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";

//vertex shader source

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//now we need to compile this shader dynamically at runtime
//let's create a shader object first

unsigned int vertexShader;
unsigned int fragmentShader;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
int main() {
	/*
	* Vertex data is stored in gpu memory
	* We can manage this memory with VBO
	* VBO can store large number of vertices in gpu memory
	* Once the vertices data is in gpu memory the vertex shader has instant access
	* to those vertices making it faster
	*/
	//define Vertex buffer object
	unsigned int VBO;
	//define Vertex array object
	unsigned int VAO;
	//triangle coordinates on NDC plane
	float vertices[] = {
		//x,y,z
		//NDC coordinates
		//first triangle
		-0.5, 0.5, 0.0,
		-0.5,-0.5, 0.0,
		0.5,-0.5,0.0,

		//second triangle
		0.5,0.5,0.0,
		-0.5,0.5,0.0,
		0.5,-0.5,0.0
	};
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//generate buffer id for VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);




	//generate buffer id for vbo
	glGenBuffers(1, &VBO);

	
	//vbo buffer type is GL_ARRAY_BUFFER
	//bind vbo to GL_ARRAY_BUFFER using glBindBuffer
	glBindBuffer(

		GL_ARRAY_BUFFER, //the current active slot for vertex data
		VBO //vertex buffer object
	);

	//remember currently bound buffer is VBO
	//copy vertex data into buffer memory using glBufferData
	/*
		GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		GL_STATIC_DRAW: the data is set only once and used many times.
		GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/
	//after glBufferData the gpu now has it's own fast copy of vertices
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(vertices), 
		vertices, 
		GL_STATIC_DRAW
	);

	//we successfully stored vertex data on gpu memory!
	//now let's build the vertex and fragment shader
#pragma region VertexShader
	//we are creating vertex shader first so lets pass GL_VERTEX_SHADER to glCreateShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attaching shader source to shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//compiling shader
	glCompileShader(vertexShader);

	//we need to check if the compilation was successfull or not
	int success;
	char infoLog[512];
	//we can check it with glGetShaderiv
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//if compilation fails retieve the error message
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}
	std::cout << "SUCCESS::SHADER::VERTEX::COMPILATION_COMPLETED\n" << std::endl;
#pragma endregion

#pragma region FragmentShader
	//now let's build the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}
	std::cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION_COMPLETED\n" << std::endl;
#pragma endregion


#pragma region ShaderProgram
	//now let's create a shader program to link shaders
	/*
		When linking the shaders into a program it links the outputs of each shader to the inputs of the next shader.
		This is also where you'll get linking errors if your outputs and inputs do not match.

	*/
	unsigned int shaderProgram;
	//The glCreateProgram function creates a program and returns the ID reference to the newly created program object.
	shaderProgram = glCreateProgram();

	//now we need to attach the previously compiled shaders to the program object
	//and then link them with glLinkProgram
	//first let's attach each one with glAttachShader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//now we link the program
	glLinkProgram(shaderProgram);
	//ofc now we need to check if the linking was success or not
	//we'll use glGetProgramiv
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION::LINKING_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	std::cout << "SUCCESS::SHADER::COMPILATION::LINKING_COMPLETED\n" << std::endl;
#pragma endregion
	//if the linking is successful now we can use the linked program
	//glUseProgram is the one for the job
	glUseProgram(shaderProgram);
	/*
	* Every shader and rendering call after glUseProgram will now use this program object 
	* (and thus the shaders).
	* another thing now we linked the shaders to program object we can safetly delete them
	* with glDeleteShader
	*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	/*
	* now let's tell the opengl how it should interpret the vertex data in memory
	* and how it should connect the vertex data to the vertex shader attributes
	* let me warn you this is alien level shit and confusing as fook
	* please keep writing comments even the smallist detail
	*/


	/*
	* So our vertex buffer data is formatted as follow
	* The position data is stored as 32bit (4 byte)
	* Each position is composed of 3 of those values
	* There is no space or values between each set of 3 values(x,y,z).
	* The values are tightly packed in the array.
	* The first value in the data is at beginning of the buffer
	*/

	//we can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer
	glVertexAttribPointer(
		0, //The data goes to vertex attribute location 0, remeber the layout(location=0)
		3, //Each vertex has 3 float
		GL_FLOAT, //The datatype is float
		GL_FALSE, //Don't normalize them
		3 * sizeof(float), //The stride is of 12 bytes, next vertex starts right after these 3 floats
		(void*)0 //Start reading from the beginning of the buffer
	);
	/*
	* now opengl understands like this
	* each stride is 12 bytes remember
	* bytes 0-11  -> vertex 1 position -> send to location 0 -> becomes aPos 
	* bytes 12-23 -> vertex 2 position -> send to location 0 -> becomes aPos
	* bytes 24-35 -> vertex 3 position -> send to location 0 -> becomes aPos
	*/
	//it's just here for some unknown reason 
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		//idhar input wala bhaijan
		processInput(window);

		//rendering wala yaha karna hai
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//isko ase he chalne do ye adha samjh aya hai abhi
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}