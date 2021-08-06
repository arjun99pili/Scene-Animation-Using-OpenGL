#ifndef Control_H_
#define Control_H_

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

// Include GLEW
#include <GL/glew.h>

// Include GLM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>


#include <GLFW/glfw3.h>
#include <iostream>


#include"model.hpp"
#include"utils.hpp"
#include"camera.hpp"
#include"light.hpp"

using namespace glm;

class Controller
{
    public :

        World *world;
        int stopObject, moveMethod;
        int camChoice; // 0 - A, 1 - B, 2 - Bird, 3 - scene.

        void run();
        void display();


        Controller(){
            stopObject = 0;
            moveMethod = 0;
            camChoice = 3;
        };

        void move(int speedchoice, int direction, int object);
        void moveMan(int speedchoice, int direction, GraphNode *temp);
        void rotateMan(int speedchoice, GraphNode *temp);
        void scroll_camera(double s);
        void move_camera(double x, double y);
        void translate_camera(int direction);


};



#endif