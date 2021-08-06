#ifndef Model_H
#define Model_H
#pragma once

#include<GLFW/glfw3.h>
#include<vector>

#include <GL/glew.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shader_s.hpp"
#include"light.hpp"
#include"camera.hpp"

using namespace glm;
using namespace std;

class Model
{
	public:
	    
        std::vector<glm::vec3>  vertices;
	    std::vector<glm::vec3>  normals;
	    std::vector<glm::vec3>  norms;
		std::vector<glm::vec2>  texCords;

        std::vector<unsigned int> indices;
	
        // glm::mat4 modelInitial;
        // glm::mat4 scaleMatrix;
        // glm::mat4 translateMatrix;
        // glm::mat4 rotateMatrix; REPLACED BY

		float initialTrans[3], initialRot[3], initialScale;

	    GLuint VAO1, VBO1, NBO1, TBO1, EBO1;

		GLuint texture;
		int width, height, nrChannels; float temp;
		unsigned char* data;

		int vertex_count, indices_count;
		bool is_picked;

		//float trans_x, trans_y, trans_z;
		//FOR TEXTURE MAPPING.
		float min_y, max_y;

		//Scene manager
		int eventManager[5] = {0};
		float speed[3], rot[3];
		float direction[3];
		int time;
		//int initialValues[7];

		//The shader specific for the model
		Shader* ourShader;

	    Model(){
			time = 0;
			speed[0] = speed[1] = speed[2] = 0;
		};
		//getters and setters

        void makeObject(const char* path);
        void setNormals();
		void setTexture(const char* path);
	    void setupBuffer();

		void setInitialVals(float *values);
		void setSpeed(float *);
		glm::vec2 intermediateSphere(vec3 point, float rad);
		glm::vec2 intermediateCylinder(glm::vec3 pointOnCylinder);
		void normFromCenter(int choice);
		void normFromObj(int choice);
		//void translate_object(float delX, float delY, float delZ);
		//void scale_object(int choice);
		virtual void nextPosition(float *transm, float *rotm, float *scale);
		virtual void render(); //Might have to pass shader.
		void setShader(Shader *objshader);
	    void Draw(Shader ourShader);

		void deleteObject();
	    ~Model(){}; //delete vertex array and vertex buffers.

};

class PersonHead : public Model
{
	public :
		PersonHead(char*);
		virtual void nextPosition(float *transm, float *rotm, float *scale);
		void render();
};

class PersonBody : public Model
{
	public :
		PersonBody(char*);
		void nextPosition(float *transm, float *rotm, float *scale);
		void render();

};

class PersonHeadC : public PersonHead
{
	public :
		PersonHeadC(char*);
		void nextPosition(float *transm, float *rotm, float *scale);
};


class Bird: public Model
{
	public :
		Bird(char*);
		void nextPosition(float *transm, float *rotm, float *scale);
		void render();
};

class HotairBalloon: public Model
{
	public :
		HotairBalloon(char*);
		void nextPosition(float *transm, float *rotm, float *scale);
		void render();
};

class Basket: public Model
{
	public :
		Basket(char*);
		int eventTime, deltaTime;
		void nextPosition(float *transm, float *rotm, float *scale);
		void render();
};

class Animal: public Model
{
	public :
		Animal(char*);
		void nextPosition(float *transm, float *rotm, float *scale);
		void render();
};

class GraphNode
{
	public :

		Model *object;
		std::vector <GraphNode*> children;
		float transMat[3], rotMat[3], scale;
		glm::mat4 scaleMatrix, initialScale;
        glm::mat4 translateMatrix, initialTrans;
        glm::mat4 rotateMatrix, initialrot;
        glm::mat4 modelMatrix, initialPos;
		Shader *ourShader;

		GraphNode(Model*, float*, Shader* objshader);
		GraphNode* getNode(int i);
		void addChild(GraphNode *);
		void drawNodeTree();
		void getPosition(float *);
		void drawModel();
		void setMatrices();
		void startEvents();

};

class World
{
	public :

		std::vector <Light *> lights;
		std::vector <GraphNode *> scenegraph;
		std::vector <Camera *> cameras;

		int worldtime; // Keep adding variables when necessary.
		Shader objectShader, lightShader;
		GLFWwindow *window;
		World(){
			worldtime = 0;
		};
		void setShader();
		void addNode(GraphNode*);
		void getNodePosition(int node, float *);
		void render();
		void createScene();
		void drawScene();
		void updateScene();
		void setCamera(int );
		void setLighting();

};
#endif