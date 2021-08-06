#include<bits/stdc++.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"../include/shader_s.hpp"
#include"../include/model.hpp"
#include"../include/control.hpp"
#include"../include/view.hpp"
#include"../include/camera.hpp"
#include"../include/light.hpp"


#include "glm/ext.hpp" 


int main()
{

    View view;
    World world;
    Controller controller;
    controller.world = &world;
    view.controller = &controller;
    view.world = &world;
    view.startShow();
   
    return 0;
}







