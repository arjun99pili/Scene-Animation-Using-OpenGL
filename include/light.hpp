#ifndef Light_H
#define Light_H

#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader_s.hpp"


class Light
{
    public:

        glm::vec3 position;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 direction;
        void setDirection(float *pos)
        {
            direction = glm::vec3(pos[0], pos[1], pos[2]);
        }
        

        //to move the light around.
        glm::mat4 modelMatrix;
        glm::mat4 modelInitial;
        glm::mat4 scaleMatrix;
        glm::mat4 translateMatrix;
        glm::mat4 rotateMatrix;

        float trans_x, trans_y, trans_z;

        //methods.
        Light()
        {
            modelMatrix = scaleMatrix = translateMatrix = rotateMatrix = glm::mat4(1.0f);
			trans_x = trans_y = trans_z = 0;
        };
        
        virtual void makeObject(Shader ourShader, glm::vec3 initialpos); //give it a figure.
        //objects to draw
        void drawLight();
        uint lightVAO, VBO; 
        Shader *lampShader;
        void setShader(Shader *lightShader)
        {
            lampShader = lightShader;
        }
        //------------------------------
        
        virtual void setBuffers(Shader ourShader); 
		void translate_object(float delX, float delY, float delZ);

	    virtual void Draw(Shader ourShader, Shader lightShader); //set onlny position. Buffer sets up the light.
        ~Light(){};


};


class spotLight : public Light
{
    public:
    
    spotLight(){};


    float cutOff;
    float outerCutOff;

    void makeObject(Shader ourshader, glm::vec3 initialpos);
    void setBuffers(Shader oursh);
    void Draw (Shader ourShader, Shader lightShader); 
};

#endif