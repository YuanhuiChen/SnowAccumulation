#version 330 core

layout(location = 0) in vec3 vs_position;

//attribute vec3 vs_position;
//attribute vec3 vs_normal;
//attribute vec3 vs_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_projMatrix;


void main()
{
   //gl_Position = (u_frontProjMatrix * u_ModelMatrix * vec4(vs_position, 1.0));
   gl_Position = (u_projMatrix * u_modelMatrix * vec4(vs_position, 1.0));
   //fs_position = gl_Position.xyz;
  
}