#define STB_IMAGE_IMPLEMENTATION


#include<Header/Mesh.h>
#include<Header/Texture.h>
#include<Header/Model.h>

//The vertex containing the position for our triangle

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES        /        COLORS         /    TexCoord       /       NORMALS      //
	-10.0f, 0.0f,  10.0f,		0.0f, 0.0f, 0.0f,		0.0f,   0.0f,		0.0f, 1.0f, 0.0f,
	-10.0f, 0.0f, -10.0f,		0.0f, 0.0f, 0.0f,		0.0f,  10.0f,		0.0f, 1.0f, 0.0f,
	 10.0f, 0.0f, -10.0f,		0.0f, 0.0f, 0.0f,		10.0f, 10.0f,		0.0f, 1.0f, 0.0f,
	 10.0f, 0.0f,  10.0f,		0.0f, 0.0f, 0.0f,		10.0f,  0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};


GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};
using namespace std; 

const unsigned int width = 800;
const unsigned int height = 800;

unsigned int samples = 8;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};


float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};
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
float randf()
{
	return -1.0f + (rand() / (RAND_MAX / 2.0f));
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
	//glfwWindowHint(GLFW_SAMPLES, samples);
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
    Shader shaderProgram
	(
		"Resource/Shaders/default.vert",
		"Resource/Shaders/default.frag"
	);

	Shader skyboxShader
	(
		"Resource/Shaders/skybox.vert",
		"Resource/Shaders/skybox.frag"
	);

	Shader asteroidShader
	(
		"Resource/Shaders/asteroid.vert",
		"Resource/Shaders/default.frag"

	);

	Shader framebufferProgram
	(
		"Resource/Shaders/framebuffer.vert", 
		"Resource/Shaders/framebuffer.frag"
	);


	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Shader for light cube
	Shader lightShader("Resource/Shaders/light.vert", "Resource/Shaders/light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();






    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// skyboxShader.Activate();
	// glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
	// asteroidShader.Activate();
	// glUniform4f(glGetUniformLocation(asteroidShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	// glUniform3f(glGetUniformLocation(asteroidShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// framebufferProgram.Activate();
	// glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);

	Texture planksTex("Resource/Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
	Texture planksSpec("Resource/Textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "tex1", 1);




    // Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_MULTISAMPLE);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glFrontFace(GL_CCW);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


   // Model jupiter("Resource/Models/jupiter/scene.gltf");

	// unsigned int rectVAO, rectVBO;
	// glGenVertexArrays(1, &rectVAO);
	// glGenBuffers(1, &rectVBO);
	// glBindVertexArray(rectVAO);
	// glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	


    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;

    unsigned int counter =0;

	// // Create Frame Buffer Object
	// unsigned int FBO;
	// glGenFramebuffers(1, &FBO);
	// glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// // Create Framebuffer Texture
	// unsigned int framebufferTexture;
	// glGenTextures(1, &framebufferTexture);
	// glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	// glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// // Create Render Buffer Object
	// unsigned int RBO;
	// glGenRenderbuffers(1, &RBO);
	// glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// // Error checking framebuffer
	// auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	// if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	// 	std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// // Create Frame Buffer Object
	// unsigned int postProcessingFBO;
	// glGenFramebuffers(1, &postProcessingFBO);
	// glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// // Create Framebuffer Texture
	// unsigned int postProcessingTexture;
	// glGenTextures(1, &postProcessingTexture);
	// glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// // Error checking framebuffer
	// fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	// if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	// 	std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;


	// unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	// glGenVertexArrays(1, &skyboxVAO);
	// glGenBuffers(1, &skyboxVBO);
	// glGenBuffers(1, &skyboxEBO);
	// glBindVertexArray(skyboxVAO);
	// glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// std::string facesCubemap[6] =
	// {
	// 	"Resource/Skyboxes/skybox/right.png",
	// 	"Resource/Skyboxes/skybox/left.png",
	// 	"Resource/Skyboxes/skybox/top.png",
	// 	"Resource/Skyboxes/skybox/bottom.png",
	// 	"Resource/Skyboxes/skybox/front.png",
	// 	"Resource/Skyboxes/skybox/back.png"
	// };

	// unsigned int cubemapTexture;
	// glGenTextures(1, &cubemapTexture);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// // These are very important to prevent seams
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// for (unsigned int i = 0; i < 6; i++)
	// {
	// 	int width, height, nrChannels;
	// 	unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
	// 	if (data)
	// 	{
	// 		stbi_set_flip_vertically_on_load(false);
	// 		glTexImage2D
	// 		(
	// 			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	// 			0,
	// 			GL_RGB,
	// 			width,
	// 			height,
	// 			0,
	// 			GL_RGB,
	// 			GL_UNSIGNED_BYTE,
	// 			data
	// 		);
	// 		stbi_image_free(data);
	// 	}
	// 	else
	// 	{
	// 		std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
	// 		stbi_image_free(data);
	// 	}
	// }

	// // The number of asteroids to be created
	// const unsigned int number = 5000;
	// // Radius of circle around which asteroids orbit
	// float radius = 100.0f;
	// // How much ateroids deviate from the radius
	// float radiusDeviation = 25.0f;

	// // Holds all transformations for the asteroids
	// std::vector <glm::mat4> instanceMatrix;

	// for (unsigned int i = 0; i < number; i++)
	// {
	// 	// Generates x and y for the function x^2 + y^2 = radius^2 which is a circle
	// 	float x = randf();
	// 	float finalRadius = radius + randf() * radiusDeviation;
	// 	float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);

	// 	glm::vec3 tempTranslation;
	// 	glm::quat tempRotation;
	// 	glm::vec3 tempScale;

	// 	// Makes the random distribution more even
	// 	if (randf() > 0.5f)
	// 	{
	// 		// Generates a translation near a circle of radius "radius"
	// 		tempTranslation = glm::vec3(y * finalRadius, randf(), x * finalRadius);
	// 	}
	// 	else
	// 	{
	// 		// Generates a translation near a circle of radius "radius"
	// 		tempTranslation = glm::vec3(x * finalRadius, randf(), y * finalRadius);
	// 	}

	// 	// Generates random rotations
	// 	tempRotation = glm::quat(1.0f, randf(), randf(), randf());
	// 	// Generates random scales
	// 	tempScale = 0.1f * glm::vec3(randf(), randf(), randf());

	// 			// Initialize matrices
	// 			glm::mat4 trans = glm::mat4(1.0f);
	// 			glm::mat4 rot = glm::mat4(1.0f);
	// 			glm::mat4 sca = glm::mat4(1.0f);
		
	// 			// Transform the matrices to their correct form
	// 			trans = glm::translate(trans, tempTranslation);
	// 			rot = glm::mat4_cast(tempRotation);
	// 			sca = glm::scale(sca, tempScale);
		
	// 			// Push matrix transformation
	// 			instanceMatrix.push_back(trans * rot * sca);
	// }


	//Model asteroid("Resource/Models/asteroid/scene.gltf",number, instanceMatrix);




    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        if (timeDiff >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "OpemGLRenderer - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());

            prevTime = crntTime;
            counter = 0;
        }

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);



		// Handles camera inputs (delete this if you have disabled VSync)
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		planksTex.Bind();
		planksSpec.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// // Draw the normal model
		// jupiter.Draw(shaderProgram, camera);
		// // Draw the asteroids
		
		
		// asteroid.Draw(asteroidShader, camera);
		// // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		// glDepthFunc(GL_LEQUAL);

		// skyboxShader.Activate();
		// glm::mat4 view = glm::mat4(1.0f);
		// glm::mat4 projection = glm::mat4(1.0f);
		// // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// // The last row and column affect the translation of the skybox (which we don't want to affect)
		// view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		// projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		// glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		// // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// // where an object is present (a depth of 1.0f will always fail against any object's depth value)
		// glBindVertexArray(skyboxVAO);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);
		
		// // Switch back to the normal depth function
		// glDepthFunc(GL_LESS);

		// glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		// glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		// // Conclude the multisampling and copy it to the post-processing FBO
		// glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// // Bind the default framebuffer
		// glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// // Draw the framebuffer rectangle
		// framebufferProgram.Activate();
		// glBindVertexArray(rectVAO);
		// glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		// glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		// glDrawArrays(GL_TRIANGLES, 0, 6);

		



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents(); 
    }
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	planksTex.Delete();
	planksSpec.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	//skyboxShader.Delete();
	// framebufferProgram.Delete();


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

