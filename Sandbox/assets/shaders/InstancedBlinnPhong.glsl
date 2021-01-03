#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in mat4 a_MVP;
layout(location = 7) in mat4 a_NormalModel;

//uniform mat4 u_Transform;
//uniform mat4 u_NormalTransform;
uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec3 v_Normal;

void main()
{
    v_Position = (a_MVP * vec4(a_Position, 1.0)).xyz;
    v_Normal = mat3(a_NormalModel) * a_Normal;
    gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform mat4 u_ViewProjection;
uniform vec3 u_ViewPos;
uniform vec3 u_PointLightPos;
uniform vec3 u_PointLightDiffuse;
uniform vec3 u_DirectionalLightDirection;
uniform vec3 u_DirectionalLightDiffuse;
uniform vec4 u_Albedo;			

in vec3 v_Position;
in vec3 v_Normal;

vec3 calculateDirectionalLight(vec3 viewDir)
{
    vec3 lightDir = normalize(-u_DirectionalLightDirection);
    float lambertian = max(dot(v_Normal, lightDir), 0.0);
    float specular = 0.0;
    if(lambertian > 0.0)
    {
        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, lightDir), 0.0);
        specular = pow(specAngle, 16.0);
    }
    vec3 color = u_Albedo.xyz * lambertian  * u_DirectionalLightDiffuse + 
                 vec3(1.0, 1.0, 1.0) * specular * u_DirectionalLightDiffuse;
    return color;
}

vec3 calculatePointLight(vec3 viewDir)
{
    vec3 diff = u_PointLightPos - v_Position;
    float illumination = max(dot(v_Normal, normalize(diff)), 0.0) / dot(diff, diff);
    return illumination * u_PointLightDiffuse;
}

void main()
{
    vec3 viewDir = normalize(u_ViewPos - v_Position);
    vec3 output = vec3(0.0);
    output += calculateDirectionalLight(viewDir);
    output += calculatePointLight(viewDir);
    color = vec4(u_Albedo.xyz * output, 1.0);
}