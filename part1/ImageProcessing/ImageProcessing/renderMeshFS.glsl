/*#version 330 core
in vec3 fs_color;
in vec3 fs_normal;
in vec3 lightDirect;
in vec4 shadowCoord;

//layout(location = 0) out float fragDepth;
 uniform sampler2D u_frontDepth;
 uniform sampler2D u_topDepth;


out vec4 fragColor;
void main()
{
	vec3 ambientColor = 0.5 * fs_color;
	vec3 lightColor = vec3(1.0, 1.0,1.0);
	float diffuseC = clamp(dot(normalize(fs_normal), normalize(lightDirect)), 0.0, 1.0);
	
	//fragColor = vec4(fs_color * diffuseC * lightColor + ambientColor, 1.0);	
	fragColor = vec4(fs_normal, 1.0);	

	//find shadow map depth
    vec2 shadowMapUV = (shadowCoord.xy)/shadowCoord.w; 
	float depth = texture2D(u_topDepth, shadowMapUV).x;
    
	bool exposed = true;
	if(shadowCoord.z > depth)
		exposed = false;
	
	//filter

}
*/
#version 330 core

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec2 fs_uv;
in vec4 mapCoord;
in vec4 fogPosition;
uniform float u_time;
uniform sampler2D u_depthMap;
uniform sampler2D u_textureMap;
uniform bool u_fog;

out vec4 depth;
layout (location = 0) out vec4 color;
in vec3 diffuseLightDir;

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main() {
    //base colors for materials
    //vec4 diffuseColor = texture2D(u_textureMap, fs_uv);
    vec4 diffuseColor = vec4(0.2,0.2,0.2,1.0);
	float seed = mapCoord.x *mapCoord.y ;

	float normalDotLight = dot(normalize(fs_normal), normalize(diffuseLightDir));


    float diffuseTerm = clamp(normalDotLight, 0.0, 1.0);
    
    float exposed = 0.0;
	//float w = 1.0 / 640.0;
	//float h = 1.0 / 480.0;

	/*for(float i = -0.5; i < 1.0; i=i+0.5)
	{
		for(float j = -0.5; j < 1.0; j= j+0.5)
		{
			exposed += (texture2D(u_depthMap, (mapCoord.xy+ vec2(i * w, j * h)) )).z;
		}
	}*/

	float inclindCoeff =  dot(normalize(fs_normal), normalize(fs_light_vector));

	float limit = 1.0 - u_time * 0.001;
	exposed = exposed / 9.0;
	exposed = texture2D(u_depthMap, mapCoord.xy).z;
	
	float t = random(vec3(mapCoord.x * mapCoord.y ,mapCoord.x * mapCoord.y, mapCoord.x * mapCoord.y), 1 );

	
	vec3 nosieNormal = vec3(t, t ,t);

	diffuseColor = diffuseColor * diffuseTerm   + vec4(0.2, 0.2, 0.2, 1.0);

	if( inclindCoeff < 0.2 || exposed < mapCoord.z - 0.005 || t < limit)
	{
		color = diffuseColor;
	}
	else
	{
		color = dot(normalize(fs_normal + 2.0 * nosieNormal), normalize(fs_light_vector)) * vec4(0.9, 0.9, 0.9, 1.0) ;
		color = mix(color, diffuseColor, 1- max(inclindCoeff, 0.0));
	}

   
	//add fog
	if(u_fog)
	{
	float depth = length(fogPosition);
	float LOG2 = 1.442695;
	float density = 0.015;
	float fogFactor = exp2(-density * density * depth * depth * LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	vec4 fogColor = vec4(0.8, 0.9, 0.5, 1.0);
	color = mix(fogColor, color, fogFactor);
	}

}