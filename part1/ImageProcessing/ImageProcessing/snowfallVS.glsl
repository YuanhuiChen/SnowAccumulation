#version 330 core

layout(location = 0) in vec4 vs_position;
layout(location = 1) in float pointSize;
layout(location = 2) in float fallSpeed;


uniform float globalTime;

out float alpha;
void main(void)
{
	//fallSpeed = 0.2;
	vec4 pos = vs_position;
	pos.x += sin(globalTime *0.001 * 3.0) * 0.3;
	pos.z += cos(globalTime * 3.0 * 0.001) *0.3;


	pos.y =1.0 - mod(globalTime * fallSpeed * 0.01, 2.0);
	//pos.y = mod(pos.y, 1.0);
	alpha = clamp((pos.z + 15.0) / 20.0, 0.0, 1.0);
	gl_Position = pos;
	//vec4 speed = vec4(0,1, 0, 0);
	//gl_Position = vs_position - globalTime * speed;
	//gl_Position = vs_position ;
	gl_PointSize = pointSize;
}