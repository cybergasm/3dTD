
//Tells us what the current time is
uniform float currentTime;

//Longest time a particle can be alive
uniform float duration;

attribute vec3 velocityIn;
attribute vec3 positionIn;
attribute vec3 color;
attribute float lifespan;


varying vec4 varyingColor;

void main() {
  uint time = currentTime;
  uint span = lifespan;
  vec4 moved = ((time%span))*vec4(velocityIn, 0);
  gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1)) + moved;
  
  varyingColor = vec4(min(color.r, length(moved)), color.b, color.g, 1 - (time%span)/duration);
}
