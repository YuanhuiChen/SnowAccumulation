#version 330 core




//there's no rules about the prefixes but they do help
//keep track of things
uniform mat4 u_modelMatrix;
uniform mat4 u_persMatrix;
uniform mat4 u_viewMatrix;
uniform vec4 u_lightPos;
uniform mat4 u_toMapMatrix;

uniform sampler2D u_growMap;

layout (location = 1)in vec3 vs_normal;
layout (location = 0)in vec3 vs_position;
layout (location = 2)in vec4 vs_uv;


out vec3 fs_normal;
out vec3 fs_light_vector;
out vec2 fs_uv;

out vec4 mapCoord;
out vec4 fogPosition;
out vec3 diffuseLightDir;
void main() { 
  
	diffuseLightDir = (vec4(-10.0, 10.0, 10.0, 1.0) - u_modelMatrix * vec4(vs_position, 1.0)).xyz;
    // Set the color and normal as just the input from the VBOs,
	// they only get interesting after they've been interpolated by the rasterizer
    fs_uv = vs_uv.xy;
    fs_normal = (u_modelMatrix * vec4(vs_normal,0)).xyz;
	mapCoord = u_toMapMatrix * u_modelMatrix * vec4(vs_position, 1.0); 

	fs_light_vector = ( u_lightPos).xyz;

	fogPosition = u_viewMatrix * u_modelMatrix * vec4(vs_position, 1.0);
	gl_Position = u_persMatrix * u_viewMatrix *u_modelMatrix * vec4(vs_position, 1.0);
	
	
}