/**
* Simple shader that takes in a position
* and color, transforms the position
* and passes it on to the fragment shader
*/

attribute vec3 positionIn;
attribute vec3 colorIn;
attribute vec3 texCoordIn;

uniform float texturing = 0.0;

varying vec4 color;
varying vec3 texCoords;
void main() {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1);
  
  if (texturing == 0.0) {
    color = vec4 (colorIn, 1);
    texCoords = vec3(0,0,0);
  } else {
    color = vec4(0,0,0,0);
    texCoords = texCoordIn;
  }  
}