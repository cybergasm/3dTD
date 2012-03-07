
//Tells us what the current time is
uniform float currentTime;

//Longest time a particle can be alive
uniform float duration;

attribute vec3 velocityIn;
attribute vec3 positionIn;
attribute float lifespan;


varying vec4 varyingColor;

void main() {
  uint time = currentTime;
  uint span = lifespan;
  gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(positionIn, 1)) + ((time%span))*vec4(velocityIn, 0);
  
  if (lifespan > duration/2.0) {
    varyingColor = vec4(214.0/255.0, 210.0/255.0, 211.0/255.0, max(.2, lifespan/duration));
  } else {
    varyingColor = vec4(168.0/255.0, 166.0/255.0, 162.0/255.0, max(.2, lifespan/duration));
  }
  //varyingColor = vec4(.2, .3, .4, 1);
}
