#version 330 core
in vec3 fs_color;
in vec3 fs_normal;
in vec3 fs_frontPosition;
in vec3 fs_topPosition;

//layout(location = 0) out float fragDepth;
layout(location = 0) out vec3 frontFragColor;
layout(location = 1) out vec3 topFragColor;
void main()
{

	float depth = fs_frontPosition.z;
	depth = gl_FragCoord.z;
	//fragColor = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
	frontFragColor = vec3(depth, depth, depth);
	//depth = fs_topPosition.z;
	//topFragColor = vec3(depth, depth, depth);
	//fragColor = lightColor;
	//fragDepth = gl_FragCoord.z ; 
	
}

