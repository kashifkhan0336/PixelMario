#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//fragment shader source


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
	//define Vertex buffer object
	unsigned int VBO;
	float vertices[] = {
		//x,y,z
		//NDC coordinates
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
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
	//generate buffer id for vbo
	glGenBuffers(1, &VBO);

	//vbo buffer type is GL_ARRAY_BUFFER
	//bind vbo to GL_ARRAY_BUFFER using glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//remember currently bound buffer is VBO
	//copy vertex data into buffer memory using glBufferData
	/*
		GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		GL_STATIC_DRAW: the data is set only once and used many times.
		GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//we successfully stored vertex data on gpu memory!
	//now let's build the vertex and fragment shader

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

	//now let's build the fragment shader


	while (!glfwWindowShouldClose(window))
	{
		//idhar input wala bhaijan
		processInput(window);

		//rendering wala yaha karna hai
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//isko ase he chalne do ye adha samjh aya hai abhi
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}