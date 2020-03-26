#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor;

out vec3 Color;

uniform mat4 proj;
uniform mat4 move;

void main(){	
	Color = inColor;

	gl_Position = proj * move * vec4(position, 1.0f);
}