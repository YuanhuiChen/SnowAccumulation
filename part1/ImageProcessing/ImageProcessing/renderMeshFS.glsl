#version 330 core
in vec3 fs_color;
in vec3 fs_normal;
in vec3 lightDirect;

//layout(location = 0) out float fragDepth;
 uniform sampler2D u_frontDepth;

 uniform sampler2D u_topDepth;

out vec4 fragColor;
void main()
{

	vec3 ambientColor = 0.5 * fs_color;
	vec3 lightColor = vec3(1.0, 1.0,1.0);
	float diffuseC = clamp(dot(normalize(fs_normal), normalize(lightDirect)), 0.0, 1.0);
	fragColor = vec4(fs_color * diffuseC * lightColor + ambientColor, 1.0);	
}

