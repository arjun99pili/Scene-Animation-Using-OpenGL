#ifndef Parser_H
#define Parser_H
#pragma once

#include<GLFW/glfw3.h>
#include<vector>
#include<glm/glm.hpp>

class Parser
{
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indices;
    int noOfVertices, noOfFaces;

    //to normalise
	float max_x, max_y, max_z, min_x, min_y, min_z;
    float max_float(float a, float b);
	float min_float(float a, float b);
    void normalize_vertices();


public:
    Parser();
    bool loadObject(const char* path);
    //getters
    int getVertexCount()
    {
        return noOfVertices;
    }
    int getIndexCount()
    {
        return noOfFaces;
    }
    std::vector<glm::vec3> getVetrtices()
    {
        return vertices;
    }
    std::vector<unsigned int> getIndices()
    {
        return indices;
    }
    std::vector<glm::vec3> getNorms()
    {
        return norms;
    }
    float getMinY()
    {
        return min_y;
    }
    float getMaxY()
    {
        return max_y;
    }

    ~Parser(){};
};


#endif