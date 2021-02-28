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
	if(height < 0.5) {
    	col = vec4(0.00392156,0.4745098,0.43529412,1.0);
	}
	else if(height < 1.0) {
    	col = vec4(46.0/255.0,139.0/255.0,87.0/255.0,1.0);
	}
	else if(height < 2.0) {
    	col = vec4(79.0/255.0,121.0/255.0,66.0/255.0,1.0);
	}
	else if(height < 4.0) {
    	col = vec4(1.0/255.0,121.0/255.0,111.0/255.0,1.0);
	}
	else if(height < 7.0) {
    	col = vec4(0.0/255.0,165.0/255.0,114.0/255.0,1.0);
	}
	else {
    	col = vec4(143.0/255.0,151.0/255.0,121.0/255.0,1.0);
	}
	col *= CalcDirectionLight();
}