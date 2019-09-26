//fragment shader
#version 330

out vec4 color;  //color output variable
in vec4 vCol; //input variable for color from vertex shader
void main()
{
	color = vCol;
}