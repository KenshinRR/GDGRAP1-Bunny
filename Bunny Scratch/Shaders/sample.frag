#version 330 core

out vec4 FragColor; //returns a color
//simple shader that colors the model Red

uniform float r;
uniform float g;
uniform float b;

void main()
{
	//					r	g	b	a		Ranges from 0->1
	FragColor = vec4(0.0f + r, 0.0f + g, 0.9f + b, 1.0f); //Sets the color of the Fragment
}