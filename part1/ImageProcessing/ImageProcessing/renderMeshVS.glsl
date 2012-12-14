#version 330 core



/*layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec3 vs_color;
//attribute vec3 vs_position;
//attribute vec3 vs_normal;
//attribute vec3 vs_color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_PersMatrix;
uniform mat4 u_InverseMatrix;

uniform vec3 u_LightPosition;
uniform mat4 u_worldToMap;

out vec3 fs_color;
out vec3 fs_normal;
out vec3 lightDirect;
out vec4 shadowCoord;



void main()
{
   //fs_normal = (u_ModelMatrix * vec4(vs_normal, 0.0)).xyz;
   fs_normal = (u_InverseMatrix* vec4(vs_normal, 0.0)).xyz;
   //lightDirect = vec3(u_ViewMatrix * vec4(u_LightPosition,1.0) - u_ViewMatrix * u_ModelMatrix * vec4(vs_position, 1.0)).xyz;
  
  // fs_color = vs_color;
   
   //shadowCoord = u_worldToMap * vec4(vs_position, 1.0);
   gl_Position = u_PersMatrix * u_ViewMatrix * u_ModelMatrix * vec4(vs_position, 1.0);


   //fs_position = gl_Position.xyz;
  

}*/




//there's no rules about the prefixes but they do help
//keep track of things
uniform mat4 u_modelMatrix;
uniform mat4 u_persMatrix;
uniform mat4 u_viewMatrix;
uniform vec4 u_lightPos;
uniform mat4 u_toMapMatrix;

uniform sampler2D u_growMap;

in vec4 vs_normal;
in vec4 vs_position;
in vec4 vs_uv;


out vec3 fs_normal;
out vec3 fs_light_vector;
out vec2 fs_uv;

out vec4 mapCoord;
out vec4 oldPosition;

void main() { 
  
    // Set the color and normal as just the input from the VBOs,
	// they only get interesting after they've been interpolated by the rasterizer
   fs_uv = vs_uv.xy;
    fs_normal = (u_modelMatrix * vs_normal).xyz;
	mapCoord = u_toMapMatrix * u_modelMatrix * vs_position; 

	fs_light_vector = ( u_lightPos).xyz;
	vec4 morphPosition = vs_position;
	//if((texture2D(u_growMap, mapCoord.xy)).z > mapCoord.z - 0.005 && dot(fs_light_vector, fs_normal) > 0.2)
		//morphPosition +=  u_time* normalize(u_lightPos);
    // Set up our vector for the light
   // fs_light_vector = (u_viewMatrix * u_lightPos - u_viewMatrix * u_modelMatrix * vs_position).xyz;
	
	//built-in things to pass down the pipeline
    //gl_Position = u_persMatrix * u_viewMatrix *u_modelMatrix * vs_position;
	gl_Position = u_persMatrix * u_viewMatrix *u_modelMatrix * morphPosition;
	
	
}