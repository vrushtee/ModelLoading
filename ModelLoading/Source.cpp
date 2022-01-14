//Model loading without skybox (Lambo)

//// ********** model loading related errors solved (use all dll files from ogl folder in 
////ur project folder and also paste it in computer --> windows --> system32/system64 bit --> 
////in this folder just paste it, but before that set all paths of asismps also , then done.)
//
//// +++++++++++ use this code
//
//char tmp[] = "res/models1/nanosuit.obj";
//    Model ourModel(tmp);
//in 2019 this code will  work 

//instead of this ( Model ourModel("res/models1/nanosuit.obj");  ) , it will work till 2017
//in 2019 it will not work 


// Std. Includes
#include <string>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

//2. add 
//#include "Model.h"
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

#include "Texture.h"

// Properties
const GLuint WIDTH = 1400, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SkyBox-Model", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
   // glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    //Shader shader( "res/shaders/cube.vs", "res/shaders/cube.frag" );
   Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");


    /// **************
   // char tmp[] = "res/models1/nanosuit.obj";
    char tmp[] = "res/models2/Lamborghini_Aventador.obj";

    Model ourModel(tmp);

    //Model ourModel("res/models1/nanosuit.obj");

    //proj for 
    //spaceman projection
    //glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 1000.0f);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();


        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();

        //model
        glDepthFunc(GL_LEQUAL);
        shader.Use();
        glBindVertexArray(0);

        // make sure use new view as view1 ***********************
        glm::mat4 view1 = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view1));

        // make sure use new model as model1 ***********************
        //6.4 Draw the loaded model and done
        glm::mat4 model1;
        //spaceman value trans and scale
        //model1 = glm::translate(model1, glm::vec3(-2.0f, -5.0f, -7.0f)); // Translate it down a bit so it's at the center of the scene
        //model1 = glm::scale(model1, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down

        //lambo value trans and scale
        model1 = glm::translate(model1, glm::vec3(-2.0f, -2.0f, -50.0f)); // Translate it down a bit so it's at the center of the scene
        model1 = glm::scale(model1, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down

        glBindVertexArray(0);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
        ourModel.Draw(shader);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        cout << "w key pressed\n";
        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        cout << "s key pressed\n";
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        cout << "a key pressed\n";
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        cout << "d key pressed\n";
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
