#include "main.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <fstream>

#include "Shader.h"
namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int VertexShader(const char* vertexShaderSource);
unsigned int FragmentShader(const char* fragmentShaderSource);
unsigned int ShaderProgram(unsigned vertexShader, unsigned fragmentShader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void InitGLFW()
{
    //Initializing GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
}
int main()
{
    InitGLFW();

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	
    // build and compile our shader program
    // ------------------------------------

    fs::path localPath = fs::current_path();
    std::cout << "Current path is " << fs::current_path() << '\n';
    Shader ourShader("Shaders//VertexShader.vs",
        "Shaders//FragmentShader.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::ifstream inn;
    inn.open("c:/data/VertexData.txt");
    struct Vertex
    {
	    std::vector<float> vertexData;
    };
	std::vector<Vertex> vertices;
	std::vector<int> indices; //reference to the vertices vector
    int  infoCount;
    inn >> infoCount;
    float vertexData[512]{0}; // arbitrary number
	float maxY{INT16_MIN}, minY{INT16_MAX}, maxX{INT16_MIN}, minX{INT16_MAX};
    for (int i = 0; i < infoCount; i++){
    	inn >> vertexData[i];
	    
        
    }
	float minx, miny, maxx, maxy;
	minx = miny = maxx = maxy = 0;

	for (int i = 0; i < infoCount; i++) {
		if ((i+3)%3 == 0 && (i+3)%6 != 0) // is X
			{
			if (vertexData[i] > maxx)
				maxx = vertexData[i];
			if (vertexData[i] < minx)
				minx = vertexData[i];
			}
		if ((i+2)%3 == 0 && (i+2)%6 != 0) // is Y
		{
			std::cout << "y = " << vertexData[i];
			if (vertexData[i] > maxy)
				maxy = vertexData[i];
			if (vertexData[i] < miny)
				miny = vertexData[i];
		}
	}

	float xscale, yscale;
	xscale = abs(minx) > abs(maxx) ? abs(minx) : abs(maxx);
	yscale = abs(miny) > abs(maxy) ? abs(miny) : abs(maxy);

	for (int i = 0; i < infoCount; i++)
	{
		if ((i+3)%3 == 0 && (i+3)%6 != 0) // is X
			vertexData[i] /= xscale;
		if ((i+2)%3 == 0 && (i+2)%6 != 0) // is Y
			vertexData[i] /= yscale;
	}
	int vertexIndex = 0;
	Vertex curVertex;
	// got to get lowest x & y, and highest x & y and scale
    for (int i = 0; i < infoCount; ++i)
    {
    	curVertex.vertexData.emplace_back(vertexData[i]);
	    std::cout <<curVertex.vertexData[curVertex.vertexData.size()-1] << " ";
    	if ((i+1)%3 == 0)
    	{
    		std::cout << ", ";
    	}
	    if ((i+1)%6 == 0)
	    {
	    	vertices.emplace_back(curVertex);
	    	curVertex.vertexData.clear();
		    std::cout << std::endl;
	    }
    }
	std::cout << vertices.size();
	//std::vector<>
    for (int i = 0; i < vertices.size(); ++i)
    {
    	if (i-1 >= 0)
			indices.emplace_back(i-1);
    	indices.emplace_back(i);
	    std::cout << i-1 << ", " << i << std::endl;
    }

	// Create VBO and VAO
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// our index array in a element buffer for OpenGL to use
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
	//glDrawElements(GL_LINES, sizeof(indices), GL_UNSIGNED_INT, 0);
	
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO and copy vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    	
        ourShader.use();
    	
    	glBindVertexArray(VAO);
    	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    	//glDrawElements(GL_LINE, vertices.size(), GL_UNSIGNED_INT, 0);
       

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	
    glBindVertexArray(0); // no need to unbind it every time 
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    glDeleteProgram(ourShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


unsigned int VertexShader(const char* vertexShaderSource)
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
    // check for shader compile errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
    return vertexShader;
}

unsigned int FragmentShader(const char* fragmentShaderSource)
{
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
    // check for shader compile errors
    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragmentShader;
}

unsigned int ShaderProgram(unsigned vertexShader, unsigned fragmentShader)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
    // check for linking errors
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
    return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

