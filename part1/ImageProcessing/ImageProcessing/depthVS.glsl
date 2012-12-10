#version 330 core

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec3 vs_color;
//attribute vec3 vs_position;
//attribute vec3 vs_normal;
//attribute vec3 vs_color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_frontProjMatrix;
uniform mat4 u_topProjMatrix;

out vec3 fs_color;
out vec3 fs_normal;
out vec3 fs_frontPosition;
out vec3 fs_topPosition;
void main()
{
   //fs_normal = (u_ModelMatrix * vec4(vs_normal, 0.0)).xyz;
   fs_normal = vs_normal;
   fs_color = vs_color;
   fs_frontPosition =  (u_frontProjMatrix * u_ModelMatrix * vec4(vs_position, 1.0)).xyz;
   fs_topPosition = (u_topProjMatrix * u_ModelMatrix * vec4(vs_position, 1.0)).xyz;
   gl_Position = (u_frontProjMatrix * u_ModelMatrix * vec4(vs_position, 1.0));
   //fs_position = gl_Position.xyz;
  

}