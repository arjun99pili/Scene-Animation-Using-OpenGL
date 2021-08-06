#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

#include"../include/parser.hpp"

using namespace std;
using namespace glm;

Parser::Parser()
{
    max_x = max_y = max_z = -1;
    min_x = min_y = min_z = 1e6;
};

float Parser::max_float(float a, float b)
{
    if(a > b){
        return a;
    }
    return b;
}

float Parser::min_float(float a, float b){
    if(a > b)
    {
        return b;
    }
    return a;
}

void Parser::normalize_vertices()
{
    float max = -1;
    max = max_float(max, max_x);
    max = max_float(max, max_y);
    max = max_float(max, max_z);
    max = max_float(max, abs(min_x));
    max = max_float(max, abs(min_y));
    max = max_float(max, abs(min_z));

    min_x = min_x/(max*1.0f);
    min_y = min_y/(max*1.0f);
    min_z = min_z/(max*1.0f);
    max_x = max_x/(max*1.0f);
    max_y = max_y/(max*1.0f);
    max_z = max_z/(max*1.0f);

    float mid_x = (min_x + max_x) / 2;
    float mid_y = (min_y + max_y) / 2;
    float mid_z = (min_z + max_z) / 2;
    
    cout<<mid_x<<" "<<mid_y<<" "<<mid_z<<endl;
    for(int i = 0; i < noOfVertices; i++)
    {
        
        vertices[i].x /= (max*1.0f);
        vertices[i].y /= (max*1.0f);
        vertices[i].z /= (max*1.0f);

        vertices[i].x -= mid_x;
        vertices[i].y -= mid_y;
        vertices[i].z -= mid_z;
    }
}

bool Parser::loadObject(const char* path)
{
	printf("Loading PLY file : %s...\n", path);

	//std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	//std::vector<glm::vec3> temp_vertices; 
	//std::vector<glm::vec3> temp_normals;
	
	ifstream file(path);
   bool past_header = false;
    bool flag = false;
    string line,temp1,temp2;
    unsigned long long size;
    int count = 0;
	float dummy;
    float temp_num;
    istringstream ss;

    if ( !file.is_open() ){
        cout << "file could not be opened" << endl;
        exit(1);
    }

    while (true)
	{
		getline(file, line);
		istringstream liness (line);
		liness >> temp1;
		
		if(temp1.compare(string("end_header"))==0)
			break;
		
		if (temp1.compare(string("element")) == 0)
		{
			liness >> temp1;
			liness >> size;
			
			if (temp1.compare(string("vertex")) == 0)
			{
				noOfVertices = size;
				flag = true;
			}
			else if (temp1.compare(string("face")) == 0)
			{
				noOfFaces = size;
				flag = false;
			}	
		}
		else if (temp1.compare(string("property")) == 0 && flag)
			count++;			
	}

    float tempx, tempy, tempz;
	for(int i = 0; i < noOfVertices; i++){
        file>>tempx>>tempy>>tempz;

        glm::vec3 vertex = vec3(tempx, tempy, tempz);

        max_x = max_float(vertex.x, max_x);
        max_y = max_float(vertex.y, max_y);
        max_z = max_float(vertex.z, max_z);

        min_x = min_float(vertex.x, min_x);
        min_y = min_float(vertex.y, min_y);
        min_z = min_float(vertex.z, min_z);
        
        vertices.push_back(vertex);
        norms.push_back(vec3(0.0f,0.0f,0.0f));

        //cerr << line << endl;
    }
    normalize_vertices();
	//cerr << "\nvertices read = " << temp_vertices.size() << endl;
    unsigned int vertexIndex[3], jib;

    for(int i = 0; i < noOfFaces; i++ )
    {
        file >> jib >> vertexIndex[0] >> vertexIndex[1] >> vertexIndex[2] ;
        
        indices.push_back(vertexIndex[0]);
        indices.push_back(vertexIndex[1]);
        indices.push_back(vertexIndex[2]);
    }


    return true;
}