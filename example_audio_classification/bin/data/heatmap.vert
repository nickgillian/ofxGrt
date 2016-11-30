#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

in vec4  position;
in vec2  texcoord;
in vec4  color;
in vec3  normal;

out vec4 colorVarying;
out vec2 texCoordVarying;
out vec4 normalVarying;

void main()
{
	texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}