#version 330 core

out vec4 FragColor;
uniform vec4 PrimitivesColor;

void main()
{
	FragColor = PrimitivesColor;
}