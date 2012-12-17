#version 330 core

layout(location = 0) in vec3 vs_position;
layout(location = 1) in float pointSize;
layout(location = 2) in float fallSpeed;

uniform mat4 u_projMatrix;
uniform mat4 u_modelViewMatrix;

uniform float globalTime;
out vec3 fogPosition;
out float alpha;
void main(void)
{

	vec4 pos = u_modelViewMatrix * vec4(vs_position, 1.0);

	pos.x += sin(globalTime *0.001 * 3.0) ;
	pos.z += cos(globalTime * 3.0 * 0.001);


	pos.y =20.0 - mod(globalTime * fallSpeed * 0.05 , 40.0);
	fogPosition = pos.xyz;
	alpha = 1.0 - clamp(-pos.z / 60.0, 0.0, 1.0);
	gl_Position = u_projMatrix * pos; ;
	gl_PointSize = pointSize;
}