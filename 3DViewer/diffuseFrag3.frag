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
    
    out_Color = diffuseColor * (diffuseTerm*0.1 + 0.9);
	//out_Color = diffuseColor;
}