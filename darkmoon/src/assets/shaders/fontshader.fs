#version 330 core
in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uAtlas;
uniform vec4      customColor;

void main() {
    float alpha = texture(uAtlas, vUV).r;
    fragColor   = vec4(customColor.rgb, customColor.a * alpha);
}