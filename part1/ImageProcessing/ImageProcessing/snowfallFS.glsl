#version 330 core
uniform sampler2D u_image;
in float alpha;
out vec4 fragColor;
void main(void)
{
	vec4 color = (texture2D(u_image, gl_PointCoord));
	float t = min(color.w, alpha);
	fragColor = vec4(color.xyz,t);
	//fragColor = color;
	//if(gl_FragCoord.z > 2.0)
		//fragColor = mix(fragColor, vec4(0.5, 0.5, 0.5, 1.0),1.0 / (gl_FragCoord.z+1.0));



}
