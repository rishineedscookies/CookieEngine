#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec3 v_Normal;

void main()
{
    v_Position = (u_Transform * vec4(a_Position, 1.0)).xyz;
    v_Normal = a_Normal;
    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform mat4 u_ViewProjection;
uniform vec3 u_PointLightPos;
uniform vec3 u_PointLightDiffuse;
uniform vec3 u_DirectionalLightDirection;
uniform vec3 u_DirectionalLightDiffuse;
uniform vec4 u_Albedo;			

in vec3 v_Position;
in vec3 v_Normal;

vec3 calculateDirectionalLight()
{
    float illumination = max(dot(v_Normal, normalize(-u_DirectionalLightDirection)), 0.0);
    return illumination * u_DirectionalLightDiffuse;
}

vec3 calculatePointLight()
{
    vec3 diff = u_PointLightPos - v_Position;
    float illumination = max(dot(v_Normal, normalize(diff)), 0.0) / dot(diff, diff);
    return illumination * u_PointLightDiffuse;
}

void main()
{
    vec3 output = vec3(0.0);
    output += calculateDirectionalLight();
    output += calculatePointLight();
    color = vec4(u_Albedo.xyz * output, 1.0);
}