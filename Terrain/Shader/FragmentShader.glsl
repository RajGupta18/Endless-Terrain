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

void main() {
	if (height < maxHeight * 0.1)  {
		col = vec4(14.0/255.0, 86.0/255.0, 114.0/255.0, 1.0);	//water color
	}
	else if(height < maxHeight * 0.3) {
		col = vec4(215.0/255.0, 136.0/255.0, 79.0/255.0, 1.0);	//sand color
	}
	else if(height < maxHeight * 0.7) {
		col = vec4(16.0/255.0, 113.0/255.0, 35.0/255.0, 1.0);	//forest color
	}
	else {
    	col = vec4(247.0/255.0, 252.0/255.0, 254.0/255.0, 1.0);	//snow color
	}
	col *= CalcDirectionLight();
}