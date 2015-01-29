attribute vec4 vertexV;
attribute vec3 normalV;
attribute vec3 tangentV;
attribute vec3 bitangentV;
attribute vec4 texCoordV;

varying vec4 vertexF;
varying vec3 normalF;
varying vec3 tangentF;
varying vec3 bitangentF;
varying vec4 texCoordF;

void main(void)
{
   gl_Position =gl_ModelViewProjectionMatrix * vertexV;
   vertexF    = gl_ModelViewMatrix * vertexV;
   normalF    = gl_NormalMatrix    * normalV;
   tangentF   = (gl_ModelViewMatrix * vec4(tangentV,0.0)).xyz;
   bitangentF = (gl_ModelViewMatrix * vec4(bitangentV,0.0)).xyz;
   texCoordF  = texCoordV;
}