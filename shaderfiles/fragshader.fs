#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColour;


//light
uniform vec3 lightPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;


void main()
{
    
    //ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // specular
    //float specularStrength = 0.5;
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColour;  

    vec3 result = (ambient) * Normal;
    FragColour = vec4(result, 1.0);
}