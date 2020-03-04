#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor;

out vec3 Color;

void main(){
	vec3 addColor = inColor;

	if(addColor.r > 0.5f){
		addColor.r = addColor.r - 0.5f;
	}
	
	Color = addColor;

	gl_Position = vec4(position, 1.0f);
}