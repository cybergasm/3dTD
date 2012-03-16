//for opacity mapping
uniform sampler opacityMap;

varying vec3 normal;
varying vec3 eyePosition;
varying vec4 color;
varying vec2 texcoord;

//Won't waste time passing stuff in as it is the same for
//most tiles  
uniform vec3 Kd = vec3(1.0, 1.0, 1.0);
uniform vec3 Ks = vec3(0.0, 0.0, 0.0);
uniform vec3 Ka = vec3(1.0, 1.0, 1.0);

void main() {
  float alpha = 3;
  //Basis of Phong as in assignment 3/2
  vec3 N = normalize(normal);
  vec3 L = normalize(gl_LightSource[0].position.xyz);
  vec3 V = normalize(-eyePosition);
  
  //Diffuse
  float Rd = max(0.0, dot(L, N));
  vec3 Td = color.rgb;
  vec3 diffuse = Rd * Kd * Td * gl_LightSource[0].diffuse.rgb;
  
  //Specular
  vec3 R = reflect(-L, N);
  float Rs = pow(max(0.0, dot(V, R)), alpha);
  vec3 Ts = color.rgb;
  vec3 specular = Rs * Ks * Ts * gl_LightSource[0].specular.rgb;

  //Ambient
  vec3 ambient = Ka * gl_LightSource[0].ambient.rgb;
  
  float opac = 1;
  if (texcoord.x != -1.0) {
    opac = texture2D(opacityMap, texcoord).r;
  }
  gl_FragColor = vec4(diffuse + specular + ambient, opac);
}