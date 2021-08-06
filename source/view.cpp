#include "../include/view.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include"../include/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>



View::View()
{
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    width = 800;
    height = 600;  
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    
    window = glfwCreateWindow( width, height, "A-1", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window\n" );
        getchar();
        glfwTerminate();
        
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        //return -1;
    }

    //Rnable state variable depth(z).
    glEnable(GL_DEPTH_TEST);
   
    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles whose normal is not towards the camera
	glEnable(GL_CULL_FACE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    manControlled = camORman = 0; 

    //setting callbacks
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callbackStatic);
    glfwSetCursorPosCallback(window, mouse_callbackStatic);
    glfwSetScrollCallback(window, scroll_callbackStatic);
    glfwSetKeyCallback(window, key_callbackStatic);
}

void View::startShow()
{
    world->window = window;
    controller->run();

    runShow();
}

void View::display()
{
    controller->display();
}

void View::controlShow()
{
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(-1, 0, manControlled);
        else
        {
            controller->translate_camera(4);
        }
        
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(1, 0, manControlled);
        else
        {
            controller->translate_camera(3);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(1, 2, manControlled);
        else
        {
            controller->translate_camera(1);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(-1, 2, manControlled);
        else
        {
            controller->translate_camera(2);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    {
        controller->stopObject = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        controller->stopObject = 0;
    }
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        controller->moveMethod = 0;
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        controller->moveMethod = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if(camORman == 0)
            manControlled = 0;
        else
        {
            controller->camChoice = 0;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if(camORman == 0)
            manControlled = 1;
        else
        {
            controller->camChoice = 1;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        camORman = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        camORman = 0;
    }

    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
    
    
}

void View::runShow()
{
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.35f, 0.297f, 0.9f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        display();
        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glfwTerminate();
}


void View::key_callbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	View* temp = static_cast<View*>(glfwGetWindowUserPointer(window));
	temp->key_callback(window, key, scancode, action, mods);
}
        
void View::framebuffer_size_callbackStatic(GLFWwindow* window, int width, int height)
{
	View* temp = static_cast<View*>(glfwGetWindowUserPointer(window));
	temp->framebuffer_size_callback(window, width, height);
}

void View::mouse_callbackStatic(GLFWwindow* window, double xpos, double ypos)
{
    View* temp = static_cast<View*>(glfwGetWindowUserPointer(window));
    temp->mouse_callback(window, xpos, ypos);
}

void View::scroll_callbackStatic(GLFWwindow* window, double xpos, double ypos)
{
    View* temp = static_cast<View*>(glfwGetWindowUserPointer(window));
    temp->scroll_callback(window, xpos, ypos);
}


void View::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(-1, 0, manControlled);
        else
        {
            controller->translate_camera(4);
        }
        
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(1, 0, manControlled);
        else
        {
            controller->translate_camera(3);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(1, 2, manControlled);
        else
        {
            controller->translate_camera(1);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if(camORman == 0)
            controller->move(-1, 2, manControlled);
        else
        {
            controller->translate_camera(2);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    {
        controller->stopObject = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        controller->stopObject = 0;
    }
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        controller->moveMethod = 0;
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        controller->moveMethod = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if(camORman == 0)
            manControlled = 0;
        else
        {
            controller->camChoice = 0;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if(camORman == 0)
            manControlled = 1;
        else
        {
            controller->camChoice = 1;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        controller->camChoice = 2;
    }

    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        controller->camChoice = 3;
    }
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        camORman = 1;
    }
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        camORman = 0;
    }

    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void View::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void View::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    controller->move_camera(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void View::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    controller->scroll_camera(yoffset);
}