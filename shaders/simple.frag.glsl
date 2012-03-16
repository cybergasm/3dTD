/**
* Simple fragment shader just writes out
* passed in color
*/

varying vec4 color;
void main() {
  gl_FragColor = color;
}