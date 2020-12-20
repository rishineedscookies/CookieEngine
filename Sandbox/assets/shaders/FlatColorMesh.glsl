#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;


uniform mat4 u_ViewProjection;

out vec3 v_Position;

void main()
{
    v_Position = a_Position;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform mat4 u_ViewProjection;
uniform vec4 u_Albedo;			

in vec3 v_Position;

void main()
{
    color = u_Albedo;
}