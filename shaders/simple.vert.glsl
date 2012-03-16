/**
* Simple shader that takes in a position
* and color, transforms the position
* and passes it on to the fragment shader
*/

attribute vec3 positionIn;
attribute vec3 colorIn;

varying vec4 color;
void main() {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1);
  color = vec4 (colorIn, 1);
}