#version 330 core
// Input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

// Output vertex attributes
out vec3 FragPos; 
out vec2 TextCoord;
out vec3 tangents;
out vec3 bitangents;
out vec3 Normal;

// Uniform variables
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
   vec4 totalpos = vec4(0.0f);
   for(int i = 0;i < MAX_BONE_INFLUENCE; i++){
      if(boneIds[i] == -1)
         continue;
      if(boneIds[i] >= MAX_BONES){
         totalpos = vec4(aPos,1.0f);
         break;
      }
      vec4 localpos = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
      totalpos += localpos * weights[i];
      vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
   }
   // Transform vertex position to clip coordinates
   mat4 viewmodel = view * model;
   gl_Position = projection * viewmodel * totalpos;
   FragPos = vec3(model * totalpos);
   // Normal = mat3(transpose(inverse(model)) * aNormal);
   TextCoord = aTextCoord;
   tangents = aTangents;
   bitangents = aBitangents;

   Normal = normalize(transpose(inverse(mat3(model))) * aNormal);
}