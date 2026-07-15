#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform float depthThreshold;
uniform float normalThreshold;

uniform sampler2D screenTexture;

mat3 sobelX = mat3(
    -1.0, 0.0, 1.0,
    -2.0, 0.0, 2.0,
    -1.0, 0.0, 1.0
);

mat3 sobelY = mat3(
    -1.0, -2.0, -1.0,
     0.0,  0.0,  0.0,
     1.0,  2.0,  1.0
);

mat3 scharrX = mat3(
    -3.0, 0.0, 3.0,
    -10.0, 0.0, 10.0,
    -3.0, 0.0, 3.0
);

mat3 scharrY = mat3(
    -3.0, -10.0, -3.0,
     0.0,  0.0,  0.0,
     3.0,  10.0,  3.0
);

vec4 shaderCartoonOutline(mat3 matX, mat3 matY){
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0);

    float gx_depth = 0.0;
    float gy_depth = 0.0;
    float gx_normal = 0.0;
    float gy_normal = 0.0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            vec2 offset = vec2(i - 1, j - 1) * tex_offset;
            vec4 sample = texture(screenTexture, TexCoords + offset);
            float depth = sample.a;
            vec3 normal = sample.rgb;

            gx_depth += depth * matX[i][j];
            gy_depth += depth * matY[i][j];

            gx_normal += length(normal) * matX[i][j];
            gy_normal += length(normal) * matY[i][j];
        }
    }  

    float gradient_depth = sqrt(gx_depth * gx_depth + gy_depth * gy_depth);
    float gradient_normal = sqrt(gx_normal * gx_normal + gy_normal * gy_normal);

    float edgeDepth = min(pow(gradient_depth / depthThreshold, 2), 1.0);
    float edgeNormal = min(pow(gradient_normal / normalThreshold, 2), 1.0);

    float edge = max(edgeDepth, edgeNormal);

    vec3 sceneColor = texture(screenTexture, TexCoords).rgb;
    vec3 finalColor = mix(sceneColor, vec3(0.0), edge * 0.95);

    return vec4(finalColor, 1.0);
}

void main()
{
    FragColor = shaderCartoonOutline(scharrX, scharrY);
}