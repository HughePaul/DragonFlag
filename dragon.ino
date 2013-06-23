/*

 Dragon Flag
 
 */


const int el_all = A7;
const int el_wing0 = 3;
const int el_wing1 = 4;
const int el_wing2 = 5;
const int el_wing3 = 6;
const int el_wing4 = 7;
const int el_wing5 = 8;
const int el_fire1 = 9;
const int el_fire2 = 10;

const int light_sensor = A1;

const int loop_delay = 30;
const int disabled_delay = 30;
const int fire_delay = 10;
const int fire_flicker = 30;

int enabled = 0;
int enable() {
  digitalWrite(13, LOW);
  enabled = 1;
  digitalWrite(el_all, HIGH);
}
int disable() {
  digitalWrite(13, HIGH);
  enabled = 0;
  digitalWrite(el_all, LOW);
  digitalWrite(el_wing0, LOW);
  digitalWrite(el_wing1, LOW);
  digitalWrite(el_wing2, LOW);
  digitalWrite(el_wing3, LOW);
  digitalWrite(el_wing4, LOW);
  digitalWrite(el_wing5, LOW);
  digitalWrite(el_fire1, LOW);
  digitalWrite(el_fire2, LOW);
}

// set two fire el as chased 
int fire_buffer[fire_delay];
int fire_pos=0;
void fire_set(int val)  {
  analogWrite(el_fire1, val);
  analogWrite(el_fire2, fire_buffer[fire_pos]);
  fire_buffer[fire_pos] = val;
  fire_pos++;
  if(fire_pos >= fire_delay) { fire_pos = 0; }
}

// animate fire
int fire_speed = fire_flicker;
int fire_val = 0;
void fire_animate() {
  // animate fire
  fire_speed += random(fire_flicker)-(fire_flicker/2);
  fire_val += fire_speed;
  if(fire_val < 0) {
    fire_val = 0;
    fire_speed = fire_flicker;
  }
  if(fire_val >= 255) {
    fire_val = 255;
    fire_speed = -fire_flicker;
  }
  fire_set(fire_val);
}

int wing_mode_frame = 0;
int wing_mode = 0;
int wing_pos = 0;
int wing_frame = 10;
void wings_animate()  {
  wing_frame++;
  if(wing_frame > 3) {
    wing_frame = 0;
  
    switch(wing_mode) {
      case 0:
        digitalWrite(el_wing0, wing_pos >= 1 ? HIGH : LOW);
        digitalWrite(el_wing1, wing_pos >= 2 ? HIGH : LOW);
        digitalWrite(el_wing2, wing_pos >= 3 ? HIGH : LOW);
        digitalWrite(el_wing3, wing_pos >= 4 ? HIGH : LOW);
        digitalWrite(el_wing4, wing_pos >= 5 ? HIGH : LOW);
        digitalWrite(el_wing5, wing_pos >= 6 ? HIGH : LOW);
      break;
      case 1:
        digitalWrite(el_wing0, wing_pos >= 6 ? HIGH : LOW);
        digitalWrite(el_wing1, wing_pos >= 5 ? HIGH : LOW);
        digitalWrite(el_wing2, wing_pos >= 4 ? HIGH : LOW);
        digitalWrite(el_wing3, wing_pos >= 3 ? HIGH : LOW);
        digitalWrite(el_wing4, wing_pos >= 2 ? HIGH : LOW);
        digitalWrite(el_wing5, wing_pos >= 1 ? HIGH : LOW);
      break;
      case 2:
        digitalWrite(el_wing0, wing_pos == 0 || wing_pos == 11 ? HIGH : LOW);
        digitalWrite(el_wing1, wing_pos == 1 || wing_pos == 10 ? HIGH : LOW);
        digitalWrite(el_wing2, wing_pos == 2 || wing_pos == 9 ? HIGH : LOW);
        digitalWrite(el_wing3, wing_pos == 3 || wing_pos == 8 ? HIGH : LOW);
        digitalWrite(el_wing4, wing_pos == 4 || wing_pos == 7 ? HIGH : LOW);
        digitalWrite(el_wing5, wing_pos == 5 || wing_pos == 6 ? HIGH : LOW);
      break;
      case 3:
        digitalWrite(el_wing0, wing_pos >= 1 && wing_pos <= 11  ? HIGH : LOW);
        digitalWrite(el_wing1, wing_pos >= 2 && wing_pos <= 10  ? HIGH : LOW);
        digitalWrite(el_wing2, wing_pos >= 3 && wing_pos <= 9  ? HIGH : LOW);
        digitalWrite(el_wing3, wing_pos >= 4 && wing_pos <= 8  ? HIGH : LOW);
        digitalWrite(el_wing4, wing_pos >= 5 && wing_pos <= 7  ? HIGH : LOW);
        digitalWrite(el_wing5, wing_pos >= 6 && wing_pos <= 6 ? HIGH : LOW);
      break;
      default:
        wing_mode = 0;
    }

    // advance wing mode
    wing_mode_frame++;
    if(wing_pos == 0 && wing_mode_frame > 30) {
      wing_mode_frame = 0;
      wing_mode++;
      wing_pos = 0;
    }

    // advance pos
    wing_pos++;
    if(wing_pos > 11) { wing_pos = 0;}

  }
}

void check_light() {
  int light_level = 1024 - analogRead(light_sensor);
  if(light_level > 400) {
    disable();
  } 
  if(light_level < 300) {
    enable();
  }
}

void setup()  { 
  pinMode(light_sensor, INPUT);
  pinMode(el_all, OUTPUT);
  pinMode(el_wing0, OUTPUT);
  pinMode(el_wing1, OUTPUT);
  pinMode(el_wing2, OUTPUT);
  pinMode(el_wing3, OUTPUT);
  pinMode(el_wing4, OUTPUT);
  pinMode(el_wing5, OUTPUT);
  pinMode(el_fire1, OUTPUT);
  pinMode(el_fire2, OUTPUT);
} 


int s = LOW;
void flash_light(){
  int light_level = analogRead(light_sensor);
  delay(light_level);
  s = s?LOW:HIGH;
  digitalWrite(el_wing5, s);
  return;
}

void loop()  {
  //return flash_light();

  check_light();

  if(enabled) {

    fire_animate();
    wings_animate();
  
    delay(loop_delay);

  } else {
    delay(disabled_delay);    
  }
  
}


