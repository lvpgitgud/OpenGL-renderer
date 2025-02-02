#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<Header/VAO.h>
#include<Header/VBO.h>
#include<Header/EBO.h>
#include<Header/shaderClass.h>
#include<Header/Texture.h>
#include<Header/Camera.h>

//The vertex containing the position for our triangle
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};
// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};
using namespace std; 

const unsigned int width = 800;
const unsigned int height = 800;
//Function for resizing the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Constant loop to check for user's input
void processInput(GLFWwindow *window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //If the user presses the escape button
        glfwSetWindowShouldClose(window, true);
}

//Main function
int main()
{
    //Initialize GLFW
    glfwInit();


    //Specified the version of OpenGL we are using
    //We are using OpenGL 3.3, the VERSION_MAJOR specified the version number in front, and the VERSION_MINOR specified ther version number behind
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //OpenGL 3.3
    //       ^
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     //OpenGL 3.3
    //          ^
    //Choose OpenGl profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a new window
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLrenderer", NULL, NULL);;
    //The window is 800x600, name is "OpenGLrenderer"
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }
    //Make it the main window
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    //Set the viewport width and height to the default width and height of the window
    glViewport(0, 0, width, height);

    //Create new shader
    Shader shaderProgram("Resource/Shaders/default.vert", "Resource/Shaders/default.frag");

    VAO VAO1;//Create new VAO
    VAO1.Bind();//Bind the VAO to the shader program

    VBO VBO1(vertices, sizeof(vertices)); //Create new VBO
    EBO EBO1(indices, sizeof(indices));//Create new EBO

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);//Pass the coordinates from the VBO to the VAO
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));//Pass the color from the VBO to the VAO
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));//Pass the Texture coordinates from the VBO to the VAO

    VAO1.Unbind();//Unbind the VAO so we dont accidentally change it
    VBO1.Unbind();//Unbind the VBO so we dont accidentally change it
    EBO1.Unbind();//Unbind the EBO so we dont accidentally change it

    //Texture
    Texture popcat("Resource/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);//Create a new texture
    popcat.texUnit(shaderProgram, "tex0", 0);//Activate texture slot 0 of the shaderProgram


    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		// Binds texture so that is appears in rendering
		popcat.Bind();
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		glfwPollEvents();   
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popcat.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

