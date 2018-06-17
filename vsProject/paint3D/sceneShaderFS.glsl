uniform vec3 viewLightDir;
uniform sampler2D colorTex;         // r g b a
uniform sampler2D surfTex;          // reflLevel, reflGlossness, refrLevel, fresnelIOR
uniform sampler2D thickTex;         // thickness
uniform samplerCube envTex;         // environment map
uniform sampler2D brdfLUT;
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
   
   // specular color
   float roughness = surf.y;
   vec3 F0 = vec3(0.04);
   float metalness = surf.w;
   F0 = mix(F0, color.rgb, metalness);
   vec3 cubeMapDir   = reflect(viewDir, finalNormal);
   vec3 cubeMapDirWorld = (viewMatrixTranspose * vec4(cubeMapDir,0.0)).xyz;
   vec4 cubeMapClr = textureCubeLod(envTex, -cubeMapDirWorld, roughness * 10);
   vec2 envBRDF = textureLod(brdfLUT, vec2(NDotV, roughness)).rg;
   vec3 specularClr = cubeMapClr * (F0 * envBRDF.r + envBRDF.g);

   // diffuse color
   vec4 diffuseClr = color * NDotL;
   vec4 finalRGB = diffuseClr + specularClr;
   gl_FragColor = vec4(finalRGB.xyz, finalAlpha);
}