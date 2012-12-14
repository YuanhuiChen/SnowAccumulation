#version 330 core

in vec2 v_Texcoords;

uniform sampler2D u_image;

layout(location = 0) out vec4 fragColor;
uniform bool u_fog;
void main(void)
{

	fragColor = texture2D(u_image, v_Texcoords);

	if(u_fog)
	{
	float depth = length(gl_FragCoord.xyz);	
	float LOG2 = 1.442695;
	float density = 0.0007;
	float fogFactor = exp2(-density * density * depth * depth * LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 fogColor = vec4(0.8, 0.9, 0.8, 1.0);

	fragColor = mix(fogColor, fragColor, fogFactor);
	}
	
	
	
}
