#version 330 core
uniform sampler2D u_image;
in float alpha;
out vec4 fragColor;
in vec3 fogPosition;
uniform bool u_fog;
void main(void)
{



	vec4 color = (texture2D(u_image, gl_PointCoord));
	float t = min(color.w, alpha);
	fragColor = vec4(color.xyz,t);
	
	//fog
	if(u_fog)
	{
	float depth = length(fogPosition);	
	float LOG2 = 1.442695;
	float density = 0.007;
	float fogFactor = exp(-density * density * depth * depth * LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	vec4 fogColor = vec4(0.2, 0.2, 0.2, 1.0);
    fragColor = mix(fogColor, fragColor, fogFactor);
	}

	fragColor.w = t;




}
