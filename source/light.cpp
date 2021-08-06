#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include"../include/light.hpp"

using namespace std;
using namespace glm;

void Light::makeObject(Shader ourShader, vec3 initialpos)
{
    //get and set vertices when you draw.

    //set up the rest of the variables.
    position = initialpos;
    ambient = vec3(0.2f, 0.2f, 0.2f);
    diffuse = vec3(0.5f, 0.5f, 0.5f);
    specular = vec3(1.0f, 1.0f, 1.0f);
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;

    setBuffers(ourShader);
}

void Light::setBuffers(Shader ourShader)
{
    ourShader.use();
    //setup initial position
    modelInitial = translate(mat4(1.0f), position);

    //constant throughout the program
    //for the light
    ourShader.setVec3("light.ambient", ambient);
    ourShader.setVec3("light.diffuse", diffuse);
    ourShader.setVec3("light.specular", specular);
    ourShader.setFloat("light.constant", constant);
    ourShader.setFloat("light.linear", linear);
    ourShader.setFloat("light.quadratic", quadratic);
    drawLight();
    
    //for texture
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 0);
    ourShader.setFloat("material.shininess", 32.0f);
}

void Light::drawLight()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

   // unsigned int lightVAO;

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Light::translate_object(float delX, float delY, float delZ)
{
    trans_x +=delX;
    trans_y +=delY;
    trans_z +=delZ;

    translateMatrix = translate(mat4(1.0f), vec3(trans_x,trans_y,trans_z));
}

void Light::Draw(Shader ourShader, Shader lightShader)
{
    modelMatrix = translateMatrix * rotateMatrix * scaleMatrix * modelInitial;
    position = vec3(modelMatrix * vec4(1.0f));
   // cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;

    ourShader.use();
    ourShader.setVec3("light.position", position); //as of now no object identifies with it.

    //Draw cube
    lightShader.use();
    lightShader.setMat4("model", modelMatrix);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void spotLight::makeObject(Shader shade, vec3 initialpos)
{
    //set up the rest of the variables.
    position = initialpos;
    direction = vec3(2, 0, 0);
    ambient = vec3(0.2f, 0.2f, 0.2f);
    diffuse = vec3(0.5f, 0.5f, 0.5f);
    specular = vec3(1.0f, 1.0f, 1.0f);
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;

    cutOff = glm::cos(glm::radians(12.5f));
    outerCutOff = glm::cos(glm::radians(45.0f));

    setBuffers(shade);
}

void spotLight::setBuffers(Shader ourShader)
{
    ourShader.use();
   
    //setup initial position
    modelInitial = translate(mat4(1.0f), position);

    ourShader.setVec3("spot_light.position", position);
    ourShader.setVec3("spot_light.direction", direction);
    ourShader.setVec3("spot_light.ambient", ambient);
    ourShader.setVec3("spot_light.diffuse", diffuse);
    ourShader.setVec3("spot_light.specular", specular);
    ourShader.setFloat("spot_light.constant", constant);
    ourShader.setFloat("spot_light.linear", linear);
    ourShader.setFloat("spot_light.quadratic", quadratic);
    ourShader.setFloat("spot_light.cutOff", cutOff);
    ourShader.setFloat("spot_light.outerCutOff", outerCutOff);

    drawLight();
    
    //for texture
     ourShader.setInt("material.diffuse", 0);
     ourShader.setInt("material.specular", 0);
     ourShader.setFloat("material.shininess", 32.0f);
}

void spotLight::Draw(Shader ourShader, Shader lightShader)
{
    modelMatrix = translateMatrix * rotateMatrix * scaleMatrix * modelInitial;
    position = vec3(modelMatrix * vec4(1.0f));
   // cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;

    ourShader.use();
    ourShader.setVec3("spot_light.position", position); 
    ourShader.setVec3("spot_light,direction", direction);

    //Draw cube
    lightShader.use();
    lightShader.setMat4("model", modelMatrix);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}