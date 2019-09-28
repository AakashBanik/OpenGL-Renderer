//fragment shader
#version 330

out vec4 color;  //color output variable

in vec4 vCol; //input variable for color from vertex shader
in vec2 TexCoord;

uniform sampler2D theTexture;

void main()
{
	color = texture(theTexture, TexCoord) *vCol;
}