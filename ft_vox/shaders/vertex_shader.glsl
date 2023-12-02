#version 330 core
layout (location = 0) in int chunk_x;
layout (location = 1) in int chunk_y;
layout (location = 2) in int pos;
layout (location = 3) in int face_facing_point_type;



out vec3 ourColor;
out vec2 TexCoord;


uniform float size_texture = 128;
uniform float width_Texture = 2048;

uniform mat4 modelView;
uniform vec3 cameraPos;
uniform vec3 cameraFront;


void main()
{



	int face = 0;
	int none = 0;
	int point = 0;
	int type = 0;
	

	face = (face_facing_point_type & 0xFF000000) >> 24;
	none = (face_facing_point_type & 0x00FF0000) >> 16;
	point = (face_facing_point_type & 0x0000FF00) >> 8;
	type = face_facing_point_type & 0x000000FF;

	vec2 zero_texture = vec2(0.0, 0.0);


	float tmp = ((type - 1) * size_texture) / width_Texture;
	zero_texture = vec2(tmp - floor(tmp),
						tmp / width_Texture);
	vec2 texCoords[4];
	
	vec3 points[4];

	if (face == 3) 
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 1.0, 0.0);
		points[3] =  vec3(0.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture/ width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture/ width_Texture, zero_texture.y + size_texture/ width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture/ width_Texture);
	}
	else if (face == 2)
	{
		points[0] = vec3(0.0, 0.0, 1.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);

		
		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
	}
	else if (face == 1) 
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(0.0, 0.0, 1.0);
		points[2] = vec3(0.0, 1.0, 1.0);
		points[3] = vec3(0.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
	}
	else if (face == 0) 
	{
		points[0] = vec3(1.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(1.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
	}
	else if (face == 5) 
	{
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 0.0, 1.0);
		points[3] =  vec3(0.0, 0.0, 1.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
	}
	else if (face == 4) 
	{
		points[0] = vec3(0.0, 1.0, 0.0);
		points[1] = vec3(1.0, 1.0, 0.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
	}


    vec3 newPos;

	int pos_block = pos;
	int x = pos_block % 32;
	int y = (pos_block % 1024) / 32;
	int z = pos_block / 1024;

	x += chunk_x * 32;
	y += chunk_y * 32;

	newPos.x = x + points[point].x;
	newPos.y = z + points[point].y;
	newPos.z = y + points[point].z;
	
	// vec3 viewDir = vec3(0.0, 0.0, 0.0);
	// vec3 newCamPos = vec3(cameraPos.x, cameraPos.z, cameraPos.y);
	// viewDir = newCamPos - newPos;
	// // viewDir = cameraFront;
	// vec3 normal = cross(points[0] -points[1], points[0] - points[2]);
	// float dotProduct = dot(viewDir, normal);
	// if (dotProduct > 0.0)
	// 	return ;

	gl_Position = modelView * vec4(newPos, 1.0);
	
	vec3 colors[2] = vec3[2](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 1.0));
	ourColor = colors[type % 2];
	TexCoord = texCoords[point];
	// ourColor = vec3(0.0, 1.0, 1.0);
}

	
