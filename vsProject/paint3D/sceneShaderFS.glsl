uniform vec3 viewLightDir;
uniform sampler2D colorTex;         // r g b a
uniform sampler2D surfTex;          // reflLevel, reflGlossness, refrLevel, fresnelIOR
uniform sampler2D thickTex;         // thickness
uniform samplerCube envTex;         // environment map
uniform mat4      viewMatrixTranspose;// matrix converting normal from view space to world space
uniform float finalAlpha;            // final alpha used for blending

varying vec4 vertexF;
varying vec3 normalF;
varying vec3 tangentF;
varying vec3 bitangentF;
varying vec4 texCoordF;

void main(void)
{   
   vec3 normal   = normalize(normalF);
   vec3 lightDir = normalize(vec3(0.5,0.5,1.0));
   vec3 viewDir  = normalize(-vertexF.xyz);
   
   // compute view direction in tangent space
   vec3 TCrossB  = cross(tangentF, bitangentF);
   vec3 NCrossV  = cross(normal,   viewDir);
   float det     = dot(TCrossB, normal);
   vec2 viewDir_UV = vec2(dot(NCrossV, bitangentF), -dot(NCrossV, tangentF)) / det;
   
   // find the currect uv position by fix-point iteration
   float NDotV   = abs(dot(normal, viewDir));
   vec2  uv      = texCoordF.xy;
   float height  = 0.0;
   for(int i = 0; i < 5; i++)
   {
      float newHeight = texture2D(thickTex, uv).w * 0.1;
      uv = uv + (newHeight - height) / NDotV * viewDir_UV;
      height = newHeight;
   }
   
   // compute bumped normal
   float offsetHeightU = texture2D(thickTex, uv + vec2(2.0 / 1024.0 ,0.0)).w * 0.1;
   float offsetHeightV = texture2D(thickTex, uv + vec2(0.0, 2.0 / 1024.0)).w * 0.1;
   vec3  deltaNormalU  = (height - offsetHeightU) * 1024.0 / length(tangentF) * normalize(tangentF);
   vec3  deltaNormalV  = (height - offsetHeightV) * 1024.0 / length(bitangentF) * normalize(bitangentF);
   vec3  finalNormal   = normalize(normal + deltaNormalU + deltaNormalV);
   
   // compute diffuse and specular color
   vec4 color = texture2D(colorTex, uv);
   vec4 surf  = texture2D(surfTex,  uv);
   
   float NDotL   = abs(dot(finalNormal, lightDir));
   vec3  R = reflect(lightDir, finalNormal);
   float RDotV   = abs(dot( R, viewDir));
   
   // diffuse color
   vec4 diffuseClr = color * NDotL;
   
   // specular color
   float specularLevel = pow(surf.x + 0.0001, 0.5);
   float glossnessExponent = surf.y * surf.y * 400.0 + 1.0;
   float specularGlossness = pow(RDotV + 0.01, glossnessExponent);
   vec4 specularClr = vec4(1) * specularLevel * specularGlossness;
   
   // compute reflection color
   vec3 cubeMapDir   = reflect(viewDir, finalNormal);
   vec3 cubeMapDirWorld = (viewMatrixTranspose * vec4(cubeMapDir,0.0)).xyz;
   vec4 cubeMapClr = textureCubeLod(envTex, -cubeMapDirWorld, (1.0 - surf.y) * 5.0);
   
   // compute fresnel coefficient
   float fresnelProj = 1.0 - NDotV;
   float fresnelProjSq = fresnelProj * fresnelProj;
   float fresnelN = 1.0 - surf.w;
   float fresnelA0 = (fresnelN - 1.0) / (fresnelN + 1.0);
   float fresnelA = fresnelA0 * fresnelA0;
   float fresnelCoef = fresnelA + (1.0 - fresnelA) * fresnelProjSq * fresnelProjSq * fresnelProj;
   
   // blend colors
   float reflRatio = fresnelCoef * surf.x;
   vec4 finalRGB = reflRatio * cubeMapClr + (1.0 -  reflRatio) * diffuseClr + specularClr;
   gl_FragColor = vec4(finalRGB.xyz, finalAlpha);
}