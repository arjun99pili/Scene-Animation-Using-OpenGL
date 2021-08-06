#ifndef View_H
#define View_H
#pragma once

// Include GLEW
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include"shader_s.hpp"
#include"control.hpp"
#include"model.hpp"

using namespace glm;

class View
{
    public:
        GLFWwindow* window;
        World *world;
        Controller *controller;
        int manControlled, manspeed[2], camORman;  // 0->A, 1->B. camORman decides whether you want to control the man or the camera.
        void startShow();
        void runShow();
        void controlShow();
        void display();
        int width, height, time;

        float lastX;
        float lastY;
        bool firstMouse;
        View();

        //callback functions
        static void key_callbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods);
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 

        static void framebuffer_size_callbackStatic(GLFWwindow* window, int width, int height);
        void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        
        static void mouse_callbackStatic(GLFWwindow* window, double xpos, double ypos);
        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        
        static void scroll_callbackStatic(GLFWwindow* window, double xoffset, double yoffset);
        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


};


#endif