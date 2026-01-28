#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform vec3 u_ObjectColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // --- Ambient ---
    vec3 ambient = 0.05 * u_ObjectColor;

    // --- Camera-facing diffuse ---
    float viewDiffuse = max(dot(norm, viewDir), 0.0);
    vec3 camDiffuse = viewDiffuse * u_ObjectColor;

    // --- Bottom darkening ---
    float bottomFactor = 1.0;
    if(norm.y < 0.0) bottomFactor = 0.5; // darken the bottom
    camDiffuse *= bottomFactor;

    // --- Specular ---
    vec3 sunDir = normalize(vec3(0.0, 1.0, 0.0));
    vec3 reflectDir = reflect(-sunDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = 0.2 * spec * vec3(1.0);

    vec3 color = ambient + camDiffuse + specular;
    FragColor = vec4(color, 1.0);
}
