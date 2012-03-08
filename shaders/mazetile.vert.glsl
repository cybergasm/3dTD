//vertex position
attribute vec3 positionIn;

void main() {
  gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1));
}