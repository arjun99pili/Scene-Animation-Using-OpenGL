#define STB_IMAGE_IMPLEMENTATION
#define PI 3.14159265

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include<cmath>

using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

//inlude image loader
#include"../stb-master/stb_image.h"

#include "../include/model.hpp"
#include"../include/parser.hpp"

float quadSolution(float a, float b, float c);
float quadDistance(float *direction);

void Model::makeObject(const char* path)
{
    Parser parse;
    parse.loadObject(path);

    vertices = parse.getVetrtices();
    norms = parse.getNorms();
    indices = parse.getIndices();
    vertex_count = vertices.size();
    indices_count = indices.size();

    min_y = parse.getMinY();
    max_y = parse.getMaxY();

    //manage object's events
    for(int i=0; i<5; i++)
    {
        eventManager[i] = 0;
    }


    //Set up the rest of the variables in object.
    setNormals();
    //setupBuffer();
}

void Model::setInitialVals(float *values)
{
    initialTrans[0] = values[0];
    initialTrans[1] = values[1];
    initialTrans[2] = values[2];

    initialRot[0] = values[3];
    initialRot[1] = values[4];
    initialRot[2] = values[5];

    initialScale = values[6];
}

void Model::setSpeed(float *vel)
{
    speed[0] = vel[0];
    speed[1] = vel[1];
    speed[2] = vel[2];
}

void Model::setNormals()
{
    vec3 a,b;
    vec3 objnormal = vec3(0.0f);
    float magnitude;
    

    for(int i = 0; i < indices_count; i+=3)
    {
        vec3 cross;
        a = vertices[indices[i+1]] - vertices[indices[i]];
        b = vertices[indices[i+2]] - vertices[indices[i]]; 
        
        cross.x = a.y * b.z - b.y * a.z;
        cross.y = a.z * b.x - b.z * a.x;
        cross.z = a.x * b.y - b.x * a.y;
        float area = glm::length(cross);
        
        magnitude = std::sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
        //cross /= (magnitude, magnitude, magnitude);
        cross.x/magnitude;
        cross.y/magnitude;
        cross.z/magnitude;


        norms[indices[i]]+=cross;
        norms[indices[i+1]]+=cross;
        norms[indices[i+2]]+=cross;
        
        
    }

    //normals from norms
    for(int i=0; i<vertex_count; i++)
    {
        norms[i].x/(float)3;
        norms[i].y/(float)3;
        norms[i].z/(float)3;

        normals.push_back(norms[i]);
        objnormal += norms[i];
    }
    objnormal = normalize(objnormal);
    
}

void Model::setTexture(const char* path)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //loading texture data. Code borrowed from learnopengl.
    data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    normFromCenter(0);

    setupBuffer();

  /*  //set texture buffer
    glGenBuffers(1, &TBO1);
	glBindBuffer(GL_ARRAY_BUFFER, TBO1);
	glBufferData(GL_ARRAY_BUFFER, texCords.size() * sizeof(glm::vec2), &texCords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			sizeof(glm::vec2),                                // stride
			(void*)0                          // array buffer offset
		);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
*/
}

void Model::normFromCenter(int choice)
{
    float rad = -1, temprad;
    for(int i=0; i<vertex_count; i++)
    {
        temprad = glm::length(vertices[i]);
        if(temprad > rad)
        {
            rad = temprad;
        }
    }

    if(choice == 0)
    {
        glm::vec3 pointSphere;
        glm::vec2 texpoints;
        for(int i=0; i<vertex_count; i++)
        {
            pointSphere = glm::normalize(vertices[i]);
            pointSphere = rad * pointSphere;
            texpoints = intermediateSphere(pointSphere, rad);
            texCords.push_back(texpoints);
        }
    }
    else
    {
        glm::vec3 pointCylinder, tempPoint;
        glm::vec2 texpoints, radpoint;
        float factor;
        for(int i=0; i<vertex_count; i++)
        {
            radpoint.x = vertices[i].x;
            radpoint.y = vertices[i].z;

            factor = rad / glm::length(radpoint);
            pointCylinder = factor * vertices[i];
            texpoints = intermediateCylinder(pointCylinder);
            texCords.push_back(texpoints);
             
        }
    }
}

void Model::normFromObj(int choice)
{
    float rad = -1, temprad;
    for(int i=0; i<vertex_count; i++)
    {
        temprad = glm::length(vertices[i]);
        if(temprad > rad)
        {
            rad = temprad;
        }
    }

    if(choice == 0)
    {
        glm::vec3 pointSphere, tempvec;
        glm::vec2 texpoints;
        float a, b, c, posValue, factor;
        for(int i=0; i<vertex_count; i++)
        { 
            a = 1.0;

            tempvec = vertices[i];
            posValue = glm::length(tempvec);
            b = 2 * posValue * glm::dot(glm::normalize(tempvec),normals[i]);

            c = (posValue - rad) * (posValue - rad); // a^2 -b^2 formula.

            factor = quadSolution(a, b, c);
            pointSphere = tempvec + (factor * normals[i]);

            texpoints = intermediateSphere(pointSphere, rad);
            texCords.push_back(texpoints);

        }
    }
    else
    {
        glm::vec3 pointCylinder;
        glm::vec2 texpoints, temppos, tempnorm;
        float a, b, c, posValue, factor;
        for(int i=0; i<vertex_count; i++)
        { 
            a = 1.0;

            temppos.x = vertices[i].x;
            temppos.y = vertices[i].z;
            posValue = glm::length(temppos);

            tempnorm.x = normals[i].x;
            tempnorm.y = normals[i].z;

            glm::normalize(temppos);
            glm::normalize(tempnorm);

            b = 2 * posValue * glm::dot(temppos, tempnorm);

            c = (posValue - rad) * (posValue - rad); // a^2 -b^2 formula.

            factor = quadSolution(a, b, c);
            pointCylinder = vertices[i] + (factor * normals[i]);

            texpoints = intermediateCylinder(pointCylinder);
            texCords.push_back(texpoints);

        }
    }
    
}

glm::vec2 Model::intermediateSphere(glm::vec3 pointOnSphere, float radius)
{
    glm::vec2 texp;

    texp.y = (asin(pointOnSphere.y / radius) + PI/2) / PI;
        
        if(pointOnSphere.x >= 0)       
        {
            texp.x = acos(pointOnSphere.z / (radius * sin(PI * texp.y))) / (2*PI);
        }
        else
        {
            texp.x = (2*PI - acos(pointOnSphere.z / (radius * sin(PI * texp.y)))) / (2*PI);
        }
    //cout<<texp.x<<" "<<texp.y<<endl;
    return texp;
}

glm::vec2 Model::intermediateCylinder(glm::vec3 pointOnCylinder)
{
    glm::vec2 texp;
    texp.x = acos(pointOnCylinder.x);
    texp.y = (pointOnCylinder.y - min_y) / (max_y - min_y);
    return texp;
}

float quadSolution(float a, float b, float c)
{
    float x1, x2, discriminant, realPart, imaginaryPart;
    discriminant = b*b - 4*a*c;
    
    if (discriminant > 0) {
        x1 = (-b + sqrt(discriminant)) / (2*a);
        x2 = (-b - sqrt(discriminant)) / (2*a);
        if(x1 > x2)
        {
            return x1;
        }
        else
        {
            return x2;
        }
        
    }
    
    else if (discriminant == 0) {
        x1 = (-b + sqrt(discriminant)) / (2*a);
        return x1;
    }

    else {
        realPart = -b/(2*a);
        return realPart;
    }
}

void Model::setupBuffer()
{

    glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glEnableVertexAttribArray(0);
   
    glGenBuffers(1, &NBO1);
	glBindBuffer(GL_ARRAY_BUFFER, NBO1);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    
		glVertexAttribPointer(
			1,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			sizeof(glm::vec3),                                // stride
			(void*)0                          // array buffer offset
		);
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &TBO1);
	glBindBuffer(GL_ARRAY_BUFFER, TBO1);
	glBufferData(GL_ARRAY_BUFFER, texCords.size() * sizeof(glm::vec2), &texCords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			sizeof(glm::vec2),                                // stride
			(void*)0                          // array buffer offset
		);
    glEnableVertexAttribArray(2);
    
        
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

}

void Model::setShader(Shader *objshader)
{
    ourShader = objshader;
}

void Model::nextPosition(float *transm, float *rotm, float *scale)
{}

void Model::render()
{
    ourShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);
    
    glBindVertexArray(0);
}

void Model::Draw(Shader ourShader)
{

    ourShader.use();

    //load texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);
    
    glBindVertexArray(0);
}

void Model::deleteObject()
{
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &NBO1);
    glDeleteBuffers(1, &TBO1);
    glDeleteBuffers(1, &EBO1);
}

//Describing each Person update and render.

PersonHead::PersonHead(char *path)
{
    makeObject(path);
}

void PersonHead::nextPosition(float *transm, float *rotm, float *scale)
{
    if(time == 0)
    {
        for(int i=0; i<3; i++)
        {
            transm[i] += speed[i];
        }
        if(rot[1] == 1)
        {
            rotm[1] = (int(rotm[1] + 2.0) % 360);
        }    
        if(rot[1] == -1)
        {
            rotm[1] = (int(rotm[1] - 2.0) % 360);
        }
    }
    time++;
    if(time>10)
    {
        time = 0;
    }
}

void PersonHead::render()
{
    Model::render();
}

PersonBody::PersonBody(char *path)
{
    makeObject(path);
}

void PersonBody::nextPosition(float *transm, float *rotm, float *scale)
{
    time++;
};


void PersonBody::render()
{
    Model::render();
}
PersonHeadC::PersonHeadC(char *path) : PersonHead(path){}

void PersonHeadC::nextPosition(float *transm, float *rotm, float *scale)
{
    time++;
    if(speed[0] == 1)
    {
        transm[0] += 1;
    }
    if(speed[1] == 1)
    {
        transm[1] += 1;
    }
    if(rot[1] == 1)
    {
        rotm[1] = (int(rotm[1] + 2.0) % 360);
    }    
    if(rot[1] == -1)
    {
        rotm[1] = (int(rotm[1] - 2.0) % 360);
    }

    if(eventManager[1] == 1)
    {
        rotm[1] = (int(rotm[1] + 30.0) % 360);
        eventManager[1] = 0;
    }
}

//Describing the bird's update and render.

Bird::Bird(char *path)
{
    makeObject(path);
    direction[0] = direction[1] = direction[2] = 0;
}

void Bird::nextPosition(float *transm, float *rotm, float *scale)
{
    if(eventManager[0] == 1)
    {
        if(quadDistance(direction) < 4)
        {
            eventManager[1] = 1;
        }
        else if(quadDistance(direction) > 7)
        {
            eventManager[1] = 0;
        }
        
        if(eventManager[1] == 0)
        {
            transm[0] += (direction[0]) * 0.02;
            transm[1] += (direction[1]) * 0.02;
            transm[2] += (direction[2]) * 0.02;
        }
        else if(eventManager[1] == 1)
        {
            transm[0] -= direction[0] * 0.02;
            transm[1] -= direction[1] * 0.02;
            transm[2] -= direction[2] * 0.02;
        }
    }
}

float quadDistance(float *direction)
{
    return sqrt(abs(direction[0]) + abs(direction[1]) + abs(direction[2]));
}

void Bird::render()
{
    Model::render();
}

//Describe Balloon and Basket
HotairBalloon::HotairBalloon(char *path)
{
    makeObject(path);
}

void HotairBalloon::nextPosition(float *transm, float *rotm, float *scale)
{
    if(eventManager[0] == 1)
    {
        time ++;

        transm[0] += speed[0] ;
        transm[1] += speed[1] ;
        transm[2] += speed[2] ;
        //cout<<"Ball "<<transm[0]<<" "<<transm[1]<<" "<<transm[2]<<endl;
        *scale +=  0.005;
        //cout<<*scale<<endl;
        if(time == 500)//*scale >= 2*initialScale) can remove the comment and try, it works but scaling effects basket and ballonorientaion.
        {
            eventManager[0] = 2;
          //  eventManager[1] = 1;
        }
    }
}

void HotairBalloon::render()
{
    Model::render();
}

Basket::Basket(char *path)
{
    makeObject(path);
    eventTime = 0;
}

void Basket::nextPosition(float *transm, float *rotm, float *scale)
{
    if(eventManager[0] == 1)
    {
        *scale = initialScale/2;
        time++;
       // cout<<"ask "<<transm[0]<<" "<<transm[1]<<endl;

    }
    if(eventManager[1] == 1)
    {
        for(int i=0; i<3; i++)
        {
            speed[i] = (transm[i] - initialTrans[i])/time; 
        }
        eventManager[1] = 2;
        eventTime = time;
    }
    else if(eventManager[1] == 2)
    {
        deltaTime = (time - eventTime)/5;
        transm[0] += speed[0];
        transm[1] += speed[1]*deltaTime - 0.2*deltaTime*deltaTime;

        if(transm[1] < 5)
        {
            transm[1] = 0;
            eventManager[1] = 0;
            eventManager[0] = 0;
        }
    } 
}

void Basket::render()
{
    Model::render();
}

//Describe GraphNode 
GraphNode::GraphNode(Model *obj, float* iniValues, Shader* objshader)
{
    object = obj;
    object->setInitialVals(iniValues);
    object->setShader(objshader);

    transMat[0] = transMat[1] = transMat[2] = 0;
    rotMat[0] = rotMat[1] = rotMat[2] = 0;
    scale = 1;
    modelMatrix = translateMatrix = scaleMatrix = rotateMatrix = mat4(1.0f);

    //To keep initial scene settings
    initialTrans = translate(mat4(1.0f), vec3(iniValues[0], iniValues[1], iniValues[2]));
    initialrot = glm::rotate(mat4(1.0f), iniValues[3], vec3(1, 0, 0));
    initialrot = glm::rotate(initialrot, iniValues[4], vec3(0, 1, 0));
    initialrot = glm::rotate(initialrot, iniValues[5], vec3(0, 0, 1));
    initialScale = glm::scale(mat4(1.0f), vec3(iniValues[6], iniValues[6], iniValues[6]));
    initialPos = initialTrans * initialScale * initialrot;
}

GraphNode* GraphNode::getNode(int i)
{
    return children[i];
}

void GraphNode::addChild(GraphNode* child)
{
    children.push_back(child);
}

void GraphNode::getPosition(float *pos)
{
    pos[0] = object->initialTrans[0] + transMat[0];
	pos[1] = object->initialTrans[1] + transMat[1];
	pos[2] = object->initialTrans[2] + transMat[2];
}

void GraphNode::setMatrices()
{
    translateMatrix = translate(mat4(1.0f), vec3(transMat[0], transMat[1], transMat[2]));
    rotateMatrix = glm::rotate(mat4(1.0f), rotMat[0], vec3(1, 0, 0));
    rotateMatrix = glm::rotate(rotateMatrix, rotMat[1], vec3(0, 1, 0));
    rotateMatrix = glm::rotate(rotateMatrix, rotMat[2], vec3(0, 0, 1));
    scaleMatrix = glm::scale(mat4(1.0f), vec3(scale, scale, scale));
    modelMatrix = translateMatrix * rotateMatrix * scaleMatrix * initialPos;
}

void GraphNode::drawModel()
{
    object->ourShader->use();
    object->ourShader->setMat4("model", modelMatrix);
    object->render();
}

void GraphNode::drawNodeTree()
{
    for(int i=0; i<children.size(); i++)
    {
        children[i]->transMat[0] = transMat[0];
        children[i]->transMat[1] = transMat[1];
        children[i]->transMat[2] = transMat[2];

        children[i]->rotMat[0] = rotMat[0];
        children[i]->rotMat[0] = rotMat[0];
        children[i]->rotMat[0] = rotMat[0];
        
        children[i]->scale = scale;
        
        children[i]->drawNodeTree();
    }
    object->nextPosition(transMat, rotMat, &scale);
    setMatrices();
    drawModel();
}

void GraphNode::startEvents()
{
    for(int i=0; i<children.size(); i++)
    {
        children[i]->startEvents();
    }
    object->eventManager[0] = 1;
}


//Describe World

void World::addNode(GraphNode *node)
{
    scenegraph.push_back(node);
}

void World::getNodePosition(int node, float *pos)
{
    scenegraph[node]->getPosition(pos);
}


void World::setShader()
{
    objectShader.use();
    // lighting
    glm::vec3 lightPos(10.0f, -100.0f, 2.0f);
    
    objectShader.setVec3("objectColour", 1.0f, 0.0f, 0.31f);
    objectShader.setVec3("lightColour", 2.0f, 0.0f, 1.0f);
    //ourShader.setVec3("lightPos", lightPos);


    float FoV = 45.0f;
    mat4 projectionMatrix;
    projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);

    objectShader.setMat4("projection", projectionMatrix);

    //lamp
    lightShader.use();
    lightShader.setMat4("projection", projectionMatrix);
}

void World::updateScene()
{
    float pos[3];
   if(worldtime == 10)
   {
       float velocity[3] = {0.1,0.1,0};
       scenegraph[4]->object->setSpeed(velocity);
       for(int i=0; i<scenegraph.size(); i++)
       {
           scenegraph[i]->startEvents();
       } 
   }

   if(scenegraph[4]->object->eventManager[0] == 1) //1 is ball, 0 is bird
   {
        getNodePosition(4, scenegraph[3]->object->direction);  
        
        getNodePosition(3, pos); 
       
        scenegraph[3]->object->direction[0] -= pos[0];
        scenegraph[3]->object->direction[1] -= pos[1];
        scenegraph[3]->object->direction[2] -= pos[2];

   }
   else if(scenegraph[4]->object->eventManager[0] == 2)
   {
        scenegraph[4]->object->eventManager[0] = 0;
        scenegraph[4]->children[0]->object->eventManager[1] = 1;
        scenegraph[4] = scenegraph[4]->children[0];

        scenegraph[3]->object->eventManager[0] = 0;
        
   }

   //update camera positions
   
       scenegraph[0]->getPosition(pos); //A
       cameras[0]->setPosition(pos);

       scenegraph[1]->getPosition(pos);
       cameras[1]->setPosition(pos);

       scenegraph[3]->getPosition(pos);
       cameras[2]->setPosition(pos);

    //update direction of spot light. pos holds birds position
    lights[1]->setDirection(pos);
   
   
   worldtime++;
}

void World::createScene()
{
    objectShader.shader_init("./shaderfiles/lightshader.vs", "./shaderfiles/lightshader.fs");
    lightShader.shader_init("./shaderfiles/lamp.vs", "./shaderfiles/lamp.fs");
    setShader();

    //Person A head and body
    float initialProps[7] = {-10, 0, 0, 0, 0, 0, 1.1};
    PersonHead *personhead_A = new PersonHead("./assets/ply_files/sphere.ply"); 
    personhead_A->setTexture("./assets/textures/face.jpg");
    GraphNode *headA = new GraphNode(personhead_A, initialProps, &objectShader);

    initialProps[1] = -3; 
    PersonBody *personbody_A = new PersonBody("./assets/ply_files/cube.ply");
    personbody_A->setTexture("./assets/textures/pattern.jpeg");
    GraphNode *bodyA = new GraphNode(personbody_A, initialProps, &objectShader);
    
    //Person B head and body    
    initialProps[1] = 0; initialProps[0] = -17;
    PersonHead *personhead_B = new PersonHead("./assets/ply_files/sphere.ply"); 
    personhead_B->setTexture("./assets/textures/face.jpg");
    GraphNode *headB = new GraphNode(personhead_B, initialProps, &objectShader);

    initialProps[1] = -3;
    PersonBody *personbody_B = new PersonBody("./assets/ply_files/cube.ply");
    personbody_B->setTexture("./assets/textures/pattern.jpeg");
    GraphNode *bodyB = new GraphNode(personbody_B, initialProps, &objectShader);

    //Person C head and body
    initialProps[0] = +9; initialProps[1] = 0; initialProps[2] = 3;
    PersonHeadC *personhead_C = new PersonHeadC("./assets/ply_files/sphere.ply"); 
    personhead_C->setTexture("./assets/textures/face.jpg");
    GraphNode *headC = new GraphNode(personhead_C, initialProps, &objectShader);

    initialProps[1] = -3;
    PersonBody *personbody_C = new PersonBody("./assets/ply_files/cube.ply");
    personbody_C->setTexture("./assets/textures/pattern.jpeg");
    GraphNode *bodyC = new GraphNode(personbody_C, initialProps, &objectShader);

    //Bird
    initialProps[0] = -5; initialProps[1] = 10; initialProps[2] = -20;
    Bird *ourbird = new Bird("./assets/ply_files/beethoven.ply");
    ourbird->setTexture("./assets/textures/brickwall.jpg");
    GraphNode *bird = new GraphNode(ourbird, initialProps, &objectShader);

    //Balloon and Basket
    initialProps[0] = 5; initialProps[1] = 7; initialProps[2] = -20; initialProps[6] = 2;
    HotairBalloon *balloon = new HotairBalloon("./assets/ply_files/sphere.ply");
    balloon->setTexture("./assets/textures/red.jpg");
    GraphNode *hotBalloon = new GraphNode(balloon, initialProps, &objectShader);

    initialProps[1] = 2;
    Basket *hbasket = new Basket("./assets/ply_files/cube.ply");
    hbasket->setTexture("./assets/textures/brickwall.jpg");
    GraphNode *ourBasket = new GraphNode(hbasket, initialProps, &objectShader);


    headA->addChild(bodyA);
    headB->addChild(bodyB);
    headC->addChild(bodyC);
    hotBalloon->addChild(ourBasket);        
    
    //make the graph.
    addNode(headA);
    addNode(headB);
    addNode(headC);
    addNode(bird);
    addNode(hotBalloon);

    //Lights
    Light *light1 = new Light();
    light1->makeObject(objectShader, vec3(0, 20, 0));
    lights.push_back(light1);
    
    spotLight *light2 = new spotLight();
    light2->makeObject(objectShader, vec3(-10, 10, 0));
    lights.push_back(light2);
    
    //Cameras
    float pos[3];
    
    scenegraph[0]->getPosition(pos); //A
    Camera *cam1 = new Camera(glm::vec3(pos[0], pos[1], pos[2]));
    
    scenegraph[1]->getPosition(pos); //B
    Camera *cam2 = new Camera(glm::vec3(pos[0], pos[1], pos[2]));
    
    scenegraph[3]->getPosition(pos); //Bird
    Camera *cam3 = new Camera(glm::vec3(pos[0], pos[1], pos[2]));
    
    Camera *cam4 = new Camera(glm::vec3(0,0,50));

    cameras.push_back(cam1); //A
    cameras.push_back(cam2); //B
    cameras.push_back(cam3); //Bird
    cameras.push_back(cam4); //general purpose

}

void World::drawScene()
{
    for(int i=0; i<scenegraph.size(); i++)
    {
        scenegraph[i]->drawNodeTree();
    }
}

void World::setLighting()
{
    for(int i=0; i<lights.size(); i++)
    {
        lights[i]->Draw(objectShader, lightShader);
    }
}

void World::setCamera(int camChoice)
{
    cameras[camChoice]->setCamera(objectShader, lightShader);    
}

