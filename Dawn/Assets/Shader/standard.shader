#version 330 core

#pragma vert_begin
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 uv0;
layout(location = 4) in vec2 uv1;

out vec2 TexCoord0;
out vec2 TexCoord1; 

out vec4 FragPos;  
out vec3 Normal;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;  

void main()
{ 
	vec4 worldPos =view *  model * vec4(position, 1.0);
	FragPos =  model * vec4(position, 1.0);  
	TexCoord0 = uv0;    
	TexCoord1 = uv1;
	
    Normal = normalize(normal); //normalize(mat3(transpose(inverse(view * model))) * normal); 
	gl_Position = proj * worldPos;
}

#pragma frag_begin
#pragma include "shader_globals.include"
#pragma include "shader_gbuffer.include"


in vec2 TexCoord0;
in vec2 TexCoord1; 
in vec4 FragPos;  
in vec3 Normal;   

uniform Material material;

void main()
{
	FillGBuffer(material, FragPos, Normal); 
}