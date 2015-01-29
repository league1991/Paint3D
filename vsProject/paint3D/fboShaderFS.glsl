varying vec4 vertexF;
varying vec3 normalF;
varying vec4 texCoordF;
uniform sampler2D baseThickTex;

// the following uniform variables control depth component for output

// isGeometryDepth == 1        use geometry depth (persp/ortho)
// isGeometryDepth == 0        use thickness
uniform float isGeometryDepth;

// isPerspDepth == 1           use persp depth
// isPerspDepth == 0           use ortho depth
uniform float isPerspDepth;            

void main( void )
{
    vec3 normal = normalize(normalF);
    float thickness = texture2D(baseThickTex,texCoordF.xy).w;
    float geoDepth  = length(vertexF.xyz) * isPerspDepth + (-vertexF.z) * (1.0 - isPerspDepth);
    float l = isGeometryDepth * geoDepth + (1.0 - isGeometryDepth) * thickness;
    float depth = l / (l + 1.0);
    vec3 nor = (normal + vec3(1.0,1.0,1.0)) / 2.0;
    
    float faceID = texCoordF.w;
    float faceID_H = floor(faceID / 65536.0);
    float faceID_L = faceID - faceID_H * 65536.0;
    float objectID = texCoordF.z;
    
    float texZ = faceID_L / 65535.0;
    float texW = (objectID * 256.0 + faceID_H) / 65535.0;
    
    gl_FragData[0] = vec4(texCoordF.xy, texZ, texW);
    gl_FragData[1] = vec4(nor.xyz, depth);
}