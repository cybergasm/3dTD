/**
* Simple fragment shader just writes out
* passed in color
*/
uniform float texturing = 0.0;

uniform sampler2D texture;

varying vec4 color;
varying vec3 texCoords;

void main() {
  if (texturing == 0.0) {
    gl_FragColor = color;
  } else {
    gl_FragColor = vec4(texture2D(texture, texCoords.xy).rgb,1);
  }
}