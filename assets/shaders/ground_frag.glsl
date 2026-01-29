#version 330 core

in vec3 v_WorldPos;
out vec4 FragColor;

uniform float u_TileSize;   // e.g. 1.0
uniform vec3 u_ColorA;
uniform vec3 u_ColorB;

void main() {
    float x = floor(v_WorldPos.x / u_TileSize);
    float z = floor(v_WorldPos.z / u_TileSize);

    float checker = mod(x + z, 2.0);
    vec3 color = mix(u_ColorA, u_ColorB, checker);

    FragColor = vec4(color, 1.0);
}
