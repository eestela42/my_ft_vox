#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int norm_type;
layout (location = 2) in int block_type;


out vec3 ourColor;



uniform mat4 modelView;
uniform float zoom;



void main()
{
	// vec3 norms[6] = vec3[6](vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0));	
    
	gl_Position = modelView * vec4(aPos / zoom, 1.0);
	
	vec3 colors[2] = vec3[2]( vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0));
	ourColor = colors[block_type];
}
