#version 150

layout (points) in; 
layout (triangle_strip, max_vertices = 4) out;
in vec4 vColour[];

out vec4 fColour;

uniform mat4 ProjectionView;

void CreateVertex(vec4 offset)
{ 
	vec4 actualOffset = vec4(offset * 1.0); 
	vec4 worldPosition = gl_in[0].gl_Position + actualOffset; 
	gl_Position = worldPosition; 
	fColour = vColour[0];
	EmitVertex(); 
}

void main()
{
	CreateVertex(vec4(-0.1, 0.1, 0.1, 1.0));
	CreateVertex(vec4(-0.1, -0.1, 0.1, 1.0));
	CreateVertex(vec4(0.1, 0.1, 0.1, 1.0));
	CreateVertex(vec4(0.1, -0.1, 0.1, 1.0));
	EndPrimitive(); 
};