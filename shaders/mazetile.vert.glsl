//vertex position
attribute vec3 positionIn;

//vertex color
attribute vec3 colorIn;

//TODO: REMEMBER TO TRANSFORM THE NORMALS!!!!!!!!!!!!!!!!!!!!!!!!!!
varying vec4 color;
void main() {
  gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1));
  color = vec4(colorIn, 1);
}