//vertex shader
#version 330

layout (location = 0) in vec3 pos; //provides the layout of the viewport to the pos variable
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol; //for passing the color to the fragment shader
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform	mat4 model; //creates the model matrix for transformations
uniform mat4 projection; //create the projection matrix for orthographic view
uniform mat4 view; //create the view matrix for camera view

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0); //determines the position of the rendered object
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f); //dynamic colors
	TexCoord = tex;

	Normal = mat3(transpose(inverse(model))) * norm;

	FragPos = (model * vec4(pos, 1.0)).xyz;
}