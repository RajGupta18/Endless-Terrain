#version 330

in vec3 Normal;
in float height;

struct Light {
	vec3 color;
	float intensity;
	float diffuseIntensity;
};

struct DirectionLight {
	Light base;
	vec3 direction;
};

struct ColorData {
	vec3 color;
	float baseHeight;
	float blendStrength;
};

const int maxColorDataCount = 4;

uniform int ColorDataCount;
uniform ColorData coldata[maxColorDataCount];

uniform DirectionLight directionlight;
uniform float maxHeight;

out vec4 col;

//-----------------------------------------------------------------------------------------

vec4 CalcLightByDirection(Light light, vec3 direction) {
	vec4 ambientColor = vec4(light.color,1.0) * light.intensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)),0.0);
	vec4 diffuseColor = vec4(light.color,1.0) * light.diffuseIntensity * diffuseFactor;
	return (ambientColor + diffuseColor);
}

vec4 CalcDirectionLight() {
	return CalcLightByDirection(directionlight.base, -directionlight.direction);
}

float sat(float x) {
	if(x <= 0.0) return 0.0;
	return 1.0;
}

float invLerp(float a, float b, float x) {
	return clamp((x-a)/(b-a), 0.0, 1.0);
}

void main() {
	float heightPercent = height / maxHeight;
	vec3 color = vec3(0.0, 0.0, 0.0);
	for(int i =0; i<ColorDataCount; i++) {
		//float strength = sat(heightPercent - coldata[i].baseHeight);
		float strength = invLerp(-coldata[i].blendStrength/2 - 0.0001, coldata[i].blendStrength/2, heightPercent-coldata[i].baseHeight);
		color = color*(1.0-strength) + (strength)*coldata[i].color;
	}
	col = vec4(color, 1.0);
	col *= CalcDirectionLight();
}