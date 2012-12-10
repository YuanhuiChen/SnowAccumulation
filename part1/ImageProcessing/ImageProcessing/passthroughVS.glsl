#version 330 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 Texcoords;

//attribute vec4 Position;
//attribute vec2 Texcoords;


out vec2 v_Texcoords;

void main(void)
{
	v_Texcoords = Texcoords;
	gl_Position = Position;
}