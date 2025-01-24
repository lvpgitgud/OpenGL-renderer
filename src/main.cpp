#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>


#include<Header/VAO.h>
#include<Header/VBO.h>
#include<Header/EBO.h>
#include<Header/shaderClass.h>
#include<Header/Texture.h>

//The vertex containing the position for our triangle
GLfloat vertices[] =
{ //COORDINATES              /     COLORS     //
    -0.5, -0.5f, 0.0f,       1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
    0.5f, 0.5f, 0.0f,        0.0f, 0.0f, 1.0f,      1.0f, 1.0f,  
    0.5f, -0.5f, 0.0f,       1.0f, 1.0f, 1.0f,      1.0f, 0.0f   
};

GLuint indices[] = {
    0, 2, 1,
    0, 3, 2
};
using namespace std; 


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
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
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
    GLFWwindow* window;
    //The window is 800x600, name is "OpenGLrenderer"
    window = glfwCreateWindow(800, 600, "OpenGLrenderer", NULL, NULL);
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
    glViewport(0, 0, 800, 600);

    Shader shaderProgram("Resource/Shaders/default.vert", "Resource/Shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    //Texture
    Texture popcat("Resource/Textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popcat.texUnit(shaderProgram, "tex0", 0);

    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        popcat.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

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

