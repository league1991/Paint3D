attribute vec4 vertexV;
attribute vec3 normalV;
attribute vec4 texCoordV;

varying vec4 vertexF;
varying vec3 normalF;
varying vec4 texCoordF;

void main( void )
{
   gl_Position =gl_ModelViewProjectionMatrix * vertexV;
   vertexF    = gl_ModelViewMatrix * vertexV;
   //normalF    = normalV;
   normalF    = gl_NormalMatrix    * normalV;
   texCoordF  = texCoordV;
}