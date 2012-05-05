#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec4 fs_color;
in vec3 fs_view_direction;

uniform vec4 u_ambient;
uniform vec4 u_specular;
uniform float u_cosine_power;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 diffuseColor = fs_color;
    
    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(normalize(fs_normal), normalize(fs_light_vector)), 0.0, 1.0);
    
	vec4 diffTimesCol=diffuseTerm*fs_color;
	vec4 ambTimesCol=u_ambient*fs_color;
	
	//specular term
	vec4 spec;
	if(u_cosine_power!=0){
		vec3 nLight=-normalize(fs_light_vector);
		vec3 nView=normalize(fs_view_direction);
		vec3 normNorm=normalize(fs_normal);
		vec3 R=reflect(nLight, normNorm);
		float RdotE=clamp(dot(nView, R), 0,1);
		float cosPow=pow(RdotE, u_cosine_power);
		spec=cosPow*u_specular;
	}
	else{
		spec=vec4(0,0,0,0);
	}

	vec4 sum=clamp((diffTimesCol+ambTimesCol+spec),0.0,1.0);
	//vec4 sum=clamp((diffTimesCol+ambTimesCol),0.0,1.0);
	out_Color=sum;

	//out_Color=vec4(fs_light_vector,1);	//rainbow!
}