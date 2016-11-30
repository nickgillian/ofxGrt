#version 330

uniform sampler2D src_tex_unit0;
uniform vec4 globalColor;
in vec2 texCoordVarying;

out vec4 fragColor;

vec3 DARK_BLUE = vec3( 30.0,  95.0, 190.0) / 255.0;
vec3 CYAN      = vec3(  0.0, 175.0, 240.0) / 255.0;
vec3 GREEN     = vec3(0.0, 255.0, 0.0) / 255.0;
vec3 YELLOW    = vec3(255.0, 240.0, 0.0) / 255.0;
vec3 RED       = vec3(255.0, 0.0,   0.0) / 255.0;

vec4 getColor(float value){
	if( value<= 0.0 )
	{
	  return vec4(DARK_BLUE, value);
	}
	else if( value<0.25 )
	{
	  return vec4(CYAN, value);
	}
	else if( value<0.5 )
	{
	  return vec4(GREEN, value-0.25);
	}
	else if( value<0.75 )
	{
	  return vec4(YELLOW, value-0.5);
	}
	else if( value<1.0)
	{
		return vec4(RED,value-0.75);
	}
	else
	{
		return vec4(RED,1.0);
	}
}

void main(){
	float value = texture(src_tex_unit0, texCoordVarying).r;
	vec4 currC = getColor(value);
	vec4 prevC = getColor(value-0.25);
	fragColor = vec4(mix(prevC.rgb, currC.rgb, currC.a/0.25),1.0);
}