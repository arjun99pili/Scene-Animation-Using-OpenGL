#include"../include/camera.hpp"


void Camera::setCamera(Shader ourShader, Shader lightShader)
{
    // Camera matrix
	projection = glm::perspective(glm::radians(Zoom), (float)width / (float)height, 0.1f, 100.0f);

	ourShader.use();
	ourShader.setMat4("view", GetViewMatrix());
	ourShader.setMat4("projection", projection);


	lightShader.use();
	lightShader.setMat4("view", GetViewMatrix());
	lightShader.setMat4("projection", projection);

}