
//************************************
//Global state
//************************************
//Tells us what the current time is
uniform float currentTime;

//Longest time a particle can be alive
uniform float duration;


//************************************
//Particle information
//************************************
//velocity at which the particle is traveling
attribute vec3 velocityIn;

//acceleration of particle
attribute vec3 accelerationIn;

//where the particle starts
attribute vec3 positionIn;

//the color of the particle
attribute vec3 color;

//How long the particle lives
attribute float lifespan;

//Color we calculate from time and other variables
varying vec4 varyingColor;

void main() {
  //downcast so we can mod mod mod
  uint time = currentTime;
  uint span = lifespan;
  
  //Get the real velocity
  vec3 vel = velocityIn + (time%span)*accelerationIn;
  
  //How far have we moved? The mod ensures we 'die' and start back where
  //we started
  vec4 moved = ((time%span))*vec4(vel, 0);
  vec4 maxMoved = duration*vec4(velocityIn, 0);
  float ratio = 1 - length(moved) / length(maxMoved);
  
  //Get transformed position and then move how much we had to move
  gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * (vec4(positionIn, 1) + moved));
  
  
  //Modulate the color depending on how far we are from the center and make ourselves get more 
  //translucent as we die.
  varyingColor = vec4(color.r, color.g, color.b, ratio);
}
