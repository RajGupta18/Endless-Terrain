#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec3 norm;

out vec3 Normal;
out float height;

uniform float modelScale;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 NormalMatrix;

void main() {
    gl_Position = proj*view*model*vec4(pos.x, pos.y, pos.z, 1.0);
    Normal = (NormalMatrix * vec4(norm,0.0)).xyz;
    height = pos.y*modelScale;
}