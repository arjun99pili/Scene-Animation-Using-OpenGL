#include "../include/control.hpp"



using namespace glm;

void Controller::run()
{
	world->createScene();
}

void Controller::display()
{
	world->setCamera(camChoice);
	world->setLighting();
	world->updateScene();
	world->drawScene();
}

void Controller::move(int speedchoice, int direction, int object)
{
	GraphNode *temp = world->scenegraph[object];

	if(moveMethod == 0)
	{
		moveMan(speedchoice, direction, temp);
	}
	else
	{
		rotateMan(speedchoice, temp);
	}
	
}

void Controller::moveMan(int speedchoice, int direction, GraphNode *temp)
{

	if(stopObject == 0)
	{
    	if(speedchoice == 1)
    	{
    	    temp->object->speed[direction] = 1;
    	}
		else if(speedchoice == -1)
		{
    	    temp->object->speed[direction] = -1;	
		}
	}
	else 
    {
        temp->object->speed[direction] = 0;
    }
}

void Controller::rotateMan(int speedchoice, GraphNode *temp)
{
	if(stopObject == 0)
	{
    	if(speedchoice == 1)
    	{
    	    temp->object->rot[1] = 1;
    	}
		else if(speedchoice == -1)
		{
    	    temp->object->rot[1] = -1;	
		}
	}
	else 
    {
        temp->object->rot[1] = 0;
    }
}

void Controller::scroll_camera(double y)
{
	world->cameras[camChoice]->ProcessMouseScroll(y);
}

void Controller::move_camera(double x, double y)
{
	world->cameras[camChoice]->ProcessMouseMovement(x, y);
}

void Controller::translate_camera(int direction) //1 up, 2 down, 3 right, 4 left
{
	if(direction == 1)
	{
		world->cameras[camChoice]->ProcessKeyboard(FORWARD, 0.5f);
	}
	else if(direction == 2)
	{
		world->cameras[camChoice]->ProcessKeyboard(BACKWARD, 0.5f);
	}
	else if(direction == 3)
	{
		world->cameras[camChoice]->ProcessKeyboard(RIGHT, 0.5f);
	}
	else if(direction == 4)
	{
		world->cameras[camChoice]->ProcessKeyboard(LEFT, 0.5f);
	}
}