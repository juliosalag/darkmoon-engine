#version 330 core

const int MAX_POINT_LIGHTS = 30;
const int MAX_SPOT_LIGHTS  = 10;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

out vec4 FragColor;

// TEXTURE //

uniform sampler2D texture0;
uniform vec4 customColor;

// MATERIAL //

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

// LIGHTS //

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    Attenuation attenuation;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float cutOff;
};

uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform int gNumSpotLights;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

// FUNCTIONS //

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * 
                        light.ambientIntensity * 
                        vec4(Ka, 1.0f);

    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normal, -lightDirection);

    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(light.color, 1.0f) * 
                       light.diffuseIntensity * 
                       vec4(Kd, 1.0f) *
                       diffuseFactor;
    }

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specularFactor = dot(viewDir, reflectDir);

    if(specularFactor > 0)
    {
        specularFactor = pow(specularFactor, Shininess);
        specularColor = vec4(light.color, 1.0f) *
                        Shininess *
                        vec4(Ks, 1.0f) *
                        specularFactor;
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternal(gDirectionalLight.base, gDirectionalLight.direction, normal);
}

vec4 CalcPointLight(PointLight l, vec3 normal)
{
    vec3 lightDirection = FragPos - l.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = CalcLightInternal(l.base, lightDirection, normal);
    float attenuation = l.attenuation.constant +
                        l.attenuation.linear * distance +
                        l.attenuation.exp * distance * distance;

    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight l, vec3 normal)
{
    float auxCutOff = cos(radians(l.cutOff));

    vec3 lightToPixel = normalize(FragPos - l.base.position);
    float spotFactor = dot(lightToPixel, l.direction);

    if(spotFactor > auxCutOff)
        return CalcPointLight(l.base, normal) * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - auxCutOff));
    else 
        return vec4(0.0);
}

void main(){
    vec3 normal = normalize(Normal);
    vec4 totalLight = CalcDirectionalLight(normal);

    for(int i = 0; i < gNumPointLights; i++)
        totalLight += CalcPointLight(gPointLights[i], normal);

    for(int i = 0; i < gNumSpotLights; i++)
        totalLight += CalcSpotLight(gSpotLights[i], normal);

    FragColor = texture(texture0, TextCoord) * customColor * totalLight;
}