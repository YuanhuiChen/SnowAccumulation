#version 330 core

in vec2 v_Texcoords;

uniform sampler2D u_image;

layout(location = 0) out vec4 fragColor;
void main(void)
{
	//gl_FragColor = texture2D(u_image, v_Texcoords);
	fragColor = texture2D(u_image, v_Texcoords);
}
