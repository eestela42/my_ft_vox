#version 330 core
layout (location = 0) in int chunk_x;
layout (location = 1) in int chunk_y;
layout (location = 2) in int pos;
layout (location = 3) in int face;
layout (location = 4) in int type;
layout (location = 5) in int point;



out vec3 ourColor;



uniform mat4 modelView;
uniform float zoom;


void main()
{
	int pos_block = pos;
	float x = pos_block % 32;
	float y = (pos_block % 1024) / 32;
	float z = pos_block / 1024;

	x += chunk_x * 32;
	y += chunk_y * 32;

	vec3 points[4];


    vec3 newPos;
	// if (point == 0)
	// {
	// 	ourColor = vec3(0.0f, 0.0f, 1.0f);
	// 	newPos = vec3(0.0f, 0.0f, 0.0f);

	// }
	// else if (point == 1)
	// {
	// 	ourColor = vec3(0.0, 1.0, 0.0);
	// 	newPos = vec3(0.0f, 1.0f, 1.0f);
	// }
	// else if (face == 2)
	// {
	// 	ourColor = vec3(1.0, 1.0, 1.0);
	// 	newPos = vec3(0.0f, 0.0f, 1.0f);
		
	// }
	// else 
	// {
	// 	ourColor = vec3(1.0, 0.0, 0.0);
	// 	newPos = vec3(0.0f, 1.0f, 0.0f);

	// }
	// est 0 west 1 nord 2 sud 3 haut 4 bas 5 
	if (face == 3) // bot
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 1.0, 0.0);
		points[3] =  vec3(0.0, 1.0, 0.0);
	}
	else if (face == 2) // top
	{
		points[0] = vec3(0.0, 0.0, 1.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);
	}
	else if (face == 1) // south
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(0.0, 0.0, 1.0);
		points[2] = vec3(0.0, 1.0, 1.0);
		points[3] = vec3(0.0, 1.0, 0.0);
	}
	else if (face == 0) // north
	{
		points[0] = vec3(1.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(1.0, 1.0, 0.0);
	}
	else if (face == 5) // ouest
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 0.0, 1.0);
		points[3] =  vec3(0.0, 0.0, 1.0);
	}
	else if (face == 4) // east
	{
		points[0] = vec3(0.0, 1.0, 0.0);
		points[1] = vec3(1.0, 1.0, 0.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);
	}

	newPos.x = x + points[point].x;
	newPos.y = z + points[point].y;
	newPos.z = y + points[point].z;
	gl_Position = modelView * vec4(newPos, 1.0);
	
	vec3 colors[2] = vec3[2](vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
	ourColor = colors[type % 2];
	// ourColor = vec3(0.0, 0.0, 0.0);
}

	
