#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTextCoord;

out vec3 ourColor;
out vec2 TextCoord;

void main()
{
   gl_Position = vec4(aPos, 1.0f);
   ourColor = aColor;
   TextCoord = vec2(aTextCoord.x, aTextCoord.y);
}