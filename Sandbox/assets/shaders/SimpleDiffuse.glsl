#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;


uniform mat4 u_ViewProjection;
uniform vec3 u_PointLightPos;
uniform float u_PointLightIntensity;

out vec3 v_Position;
out vec3 v_Normal;

void main()
{
    v_Position = a_Position;
    v_Normal = a_Normal;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform mat4 u_ViewProjection;
uniform vec3 u_PointLightPos;
uniform float u_PointLightIntensity;
uniform vec4 u_Albedo;			

in vec3 v_Position;
in vec3 v_Normal;

void main()
{
    vec3 diff = u_PointLightPos - v_Position;
    float illumination = (u_PointLightIntensity * max(dot(v_Normal, normalize(diff)), 0.0)) / dot(diff, diff);
    color = vec4(u_Albedo.xyz * (illumination), 1.0);
}