
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
  
    vec4 diffuseColor = vec4(0.2,0.2,0.2,1.0);


	float normalDotLight = dot(normalize(fs_normal), normalize(diffuseLightDir));

    float diffuseTerm = clamp(normalDotLight, 0.0, 1.0);
	float inclindCoeff =  dot(normalize(fs_normal), normalize(fs_light_vector));

	float limit = 1.0 - u_time * 0.001;

	float exposed = texture2D(u_depthMap, mapCoord.xy).z;
	
	//generate a random number t
	float t = random(vec3(mapCoord.x * mapCoord.y ,mapCoord.x * mapCoord.y, mapCoord.x * mapCoord.y), 1 );

	//noise normal for snow shading
	vec3 nosieNormal = vec3(t, t ,t);

	diffuseColor = diffuseColor * diffuseTerm   + vec4(0.2, 0.2, 0.2, 1.0);

	if( inclindCoeff < 0.2 || exposed < mapCoord.z - 0.005 || t < limit ||u_time < 100.0)
	{
		//if points are not exposed to sky
		color = diffuseColor;
	}
	else
	{
		//points are exposed to sky
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