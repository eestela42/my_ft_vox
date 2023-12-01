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

	// vec3 points[4];


    vec3 newPos;
	if (point == 0)
	{
		ourColor = vec3(0.0, 0.0, 1.0);
		newPos.x = 0;
		newPos.y = 0 ;
		newPos.z = 0;

		// newPos.x = x;
		// newPos.y = z ;
		// newPos.z = y;
	}
	else if (point == 1)
	{
		ourColor = vec3(1.0, 0.0, 0.0);
		newPos.x = 0;
		newPos.y = 1 ;
		newPos.z = 1;
		// newPos.x = x;
		// newPos.y = z + 1;
		// newPos.z = y + 1;
	}
	else if (point == 2)
	{
		ourColor = vec3(1.0, 1.0, 1.0);
		newPos.x = 0;
		newPos.y = 0 ;
		newPos.z = 1;
		// newPos.x = x;
		// newPos.y = z;
		// newPos.z = y + 1;
	}
	else 
	{
		ourColor = vec3(0.0, 0.0, 0.0);
		newPos.x = 0;
		newPos.y = 1 ;
		newPos.z = 0;
		// newPos.x = x;
		// newPos.y = z + 1;
		// newPos.z = y;
	}

	// newPos.x = x + points[point].x;
	// newPos.y = z + points[point].y;
	// newPos.z = y + points[point].z;
	gl_Position = modelView * vec4(newPos, 1.0);
	
	// vec3 colors[2] = vec3[2](vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
	// ourColor = colors[type % 2];
	// ourColor = vec3(0.0, 0.0, 0.0);
}

	// if (face == 0) // bot
	// {
	// 	points[0] = vec3(0.0, 0.0, 0.0);
	// 	points[1] = vec3(1.0, 0.0, 0.0);
	// 	points[2] = vec3(1.0, 1.0, 0.0);
	// 	points[3] =  vec3(0.0, 1.0, 0.0);
	// }
	// else if (face == 1) // top
	// {
	// 	points[0] = vec3(0.0, 0.0, 1.0);
	// 	points[1] = vec3(1.0, 0.0, 1.0);
	// 	points[2] = vec3(1.0, 1.0, 1.0);
	// 	points[3] =  vec3(0.0, 1.0, 1.0);
	// }
	// else if (face == 2) // south
	// {
	// 	points[0] = vec3(0.0, 0.0, 0.0);
	// 	points[1] = vec3(0.0, 0.0, 1.0);
	// 	points[2] = vec3(0.0, 1.0, 1.0);
	// 	points[3] = vec3(0.0, 1.0, 0.0);
	// }
	// else if (face == 3) // north
	// {
	// 	points[0] = vec3(1.0, 0.0, 0.0);
	// 	points[1] = vec3(1.0, 0.0, 1.0);
	// 	points[2] = vec3(1.0, 1.0, 1.0);
	// 	points[3] =  vec3(1.0, 1.0, 0.0);
	// }
	// else if (face == 4) // ouest
	// {
	// 	points[0] = vec3(0.0, 0.0, 0.0);
	// 	points[1] = vec3(1.0, 0.0, 0.0);
	// 	points[2] = vec3(1.0, 0.0, 1.0);
	// 	points[3] =  vec3(0.0, 0.0, 1.0);
	// }
	// else // east
	// {
	// 	points[0] = vec3(0.0, 1.0, 0.0);
	// 	points[1] = vec3(1.0, 1.0, 0.0);
	// 	points[2] = vec3(1.0, 1.0, 1.0);
	// 	points[3] =  vec3(0.0, 1.0, 1.0);
	// }
