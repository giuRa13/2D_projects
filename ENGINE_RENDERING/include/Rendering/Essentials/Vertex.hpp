#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ENGINE_RENDERING
{

	struct Color
	{
		GLubyte r, g, b, a; // 8 bit each
	};


    struct Vertex
	{
		glm::vec2 position{ 0.f }, uvs{ 0.f };
		Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };
	
		void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}


		/*
		0xff00ffac
		      R          G          B          A
		0b1111 1111  0000 0000  1111 1111  1010 1100
		*/
		void set_color(GLuint newColor)
		{
			color.r = (newColor >> 24) & 0xFF; //move Out 24 bit (0xFF mask, if theres athor bits)
			color.g = (newColor >> 16) & 0xFF;
			color.b = (newColor >> 8) & 0xFF;
			color.a = (newColor >> 0) & 0xFF;
		}
	};


	struct CircleVertex
	{
		glm::vec2 position, uvs;
		Color color;
		float lineThickness;
	};
    
}