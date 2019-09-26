//vertex shader
#version 330

layout (location = 0) in vec3 pos; //provides the layout of the viewport to the pos variable

out vec4 vCol; //for passing the color to the fragment shader

uniform	mat4 model; //identity matrix model which remains uniform throughout the lifetime of the variable

uniform mat4 projection;  //projection matrix variable

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0); //determines the position of the rendered object
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f); //dynamic colors
}