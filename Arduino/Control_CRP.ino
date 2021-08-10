//Code for the cascade control of the centrifugal ring positioner

#include <Encoder.h>

Encoder myEnc(47, 37);          // definiton of the Arduino digital pins for the encoder
int sensorPin = A1;             // select the input pin for the position sensor


float current_time; 
float previous_time;
unsigned long time0;

float t_cycle = 1000;           // sampling period (in microsecondqs ; 1000 = 1ms)  
int t_exp = 20000000;           // duration of the ring control experiment (in microseconds)
int nb_cycles = t_exp / t_cycle; 
int t_start = 30000000;         // start time of the ring position control
int t_end = 50000000;           // end time of the ring position control
int t_mot = 10000000;           // start time of the system rotation

// Saving of the data in lists
float cmd_sav[3000];            // saving list of commands data
float time_sav[3000];           // saving list of time data
float vel_sav[3000];            // saving list of system angular velocity data
float pos_sav[3000];            // saving list of ring position data
float sp_sav[3000];             // saving list of ring setpoint data
float reg_sav[3000];            // saving list of outer controller data

float vel;                      // angular velocity of the system
float newTime;                  // involved in velocity calculation
float oldTime = 0;              // involved in velocity calculation
long oldPosition  = 0;          // involved in velocity calculation

float kp = 0.15;                // inner loop P controller gain
float cmd = 0;                  // output of the inner loop P controller
float cmd_I;                    // output of the inner loop P controller (in A)
float sp_vel = 0;               // setpoint of the system angular velocity (in rad/s)
float sp_pos = 0.08;            // setpoint of the ring position (in m) 
float err_pos_prev = 0;         // previous ring position error 
float err_pos_prevprev;         // second previous ring position error
float reg_out_prev = 0;         // previous outer loop controller output                                         
float reg_out_prevprev;         // second previous outer loop controller output
float reg_out;                  // outer loop controller output
float sp_pos_prec;              // previous ring position setpoint
float reg;                      // outer loop controller output (+ equilibrium point)
float w0;                       // equilibrium point velocity
float ring_position;            // position of the ring
float sensorValue;              // variable to store the value coming from the sensor

int k;


//Uncomment for the PID outer loop controller 
     
float num1 =  1000*1.051456138860390;
float num2 = 1000*2.080401718559944;
float num3 = 1000*1.029045015404039;
float den1 = 1.000000000000000; 
float den2 = 1.926343922603877;
float den3 = 0.926343922603876;

//Uncomment for the PD outer loop controller
/*
float num1 =  1000*1.591681613228143;
float num2 = 1000*1.567522419842214;
float den1 = 1.000000000000000; 
float den2 = 0.926343922603876;
*/

// List with the data from the position sensor characteristic
float list_bit[14] = {2556, 2383, 2046, 1789, 1567, 1432, 1331, 1214, 1137, 1070, 994, 931, 893, 878};
float list_abs[14] = {0.1845, 0.1557, 0.1347, 0.1187, 0.1061, 0.09596, 0.08756, 0.08051, 0.074515, 0.06934, 0.06485, 0.0609, 0.0574, 0.05549};


void setup() {
  Serial.begin(9600);
  analogWriteResolution(12);
  analogReadResolution(12);
  Serial.println("Waiting phase... ");
}

void loop() {
  while (micros() < t_mot) {                                               // waiting phase of duration t_mot to enable the motor with the switch
    analogWrite(DAC0, 2048);
  }

  while (micros() >= t_mot and micros() < t_start) {                       // start of the system rotation at time t_mot, inner loop control
    Serial.println("Start of the experiment... ");
    Serial.println("System rotation... ");
    time0 = micros();
    previous_time = time0;


    for (int j = 0; j < nb_cycles; j = j + 1) {
      current_time = micros();
      long newPosition = myEnc.read();                                     // reading of the motor angular position via the encoder
      vel = ((newPosition - oldPosition) / (t_cycle)) * 6.28 * 100 / 7;    // system angular velocity calculation
      oldPosition = newPosition;

      if (micros() >= t_mot and micros () < t_mot + 10000000) {            // linear increase of the system velocity from rest to equilibrium point velocity
        sp_vel = sp_vel + 0.0006893; 
      }
      if (micros() >= t_mot + 10000000 and micros() < t_mot + 20000000 ) { // constant system velocity at equilibrium point velocity
        sp_vel = sp_vel;
      }

      float error_vel = sp_vel - vel;                                      // system angular velocity error
      cmd_I = kp*error_vel;                                                // inner loop P controller command
      cmd = int((cmd_I+0.652)*4095/(2*0.652));                             // conversion of the inner loop command into Arduino output voltage
      
      if (cmd < 0){                                                        // normalization of the P controller command
        cmd = 0;                                               
      }
      else if (cmd > 4095){
        cmd = 4095;
      }
      analogWrite(DAC0, cmd);                                               // command signal sent to the motor driver

      previous_time = current_time;
      while (micros() - time0 < (j + 1)*t_cycle) { 
      }
    }

    w0 = sp_vel;                                                            // definition of the system angular equilibrium velocity as the current system velocity
    sp_pos = bit2m(analogRead(sensorPin));                                  // definition of the first ring position setpoint as the current sensor measure
    err_pos_prev =  0 ;
    err_pos_prevprev =  0 ;
    reg_out_prev =0; 
    reg_out_prevprev = 0; 
    
  } 

  while (micros() >= t_start and micros() < t_end) {                        // start of the cascade control of the system
    Serial.println("Start of the ring position control... ");
    time0 = micros();
    previous_time = time0;


    for (int j = 0; j < nb_cycles; j = j + 1) {                             // definition of the ring position setpoint profile
      current_time = micros();

      if (micros() >= t_start and micros () < t_start + 5000000) {
        sp_pos = sp_pos + 0.000016;
      }
      if (micros() >= t_start + 5000000 and micros() < t_start + 10000000 ) {
        sp_pos = sp_pos;
      }
      if (micros() >= t_start + 10000000 and micros() < t_start + 15000000 ) {
        sp_pos = sp_pos-0.000008;
      }
      if (micros() >= t_start + 15000000 and micros() < t_start + 20000000 ) {
        sp_pos = sp_pos;
      }


      long newPosition = myEnc.read();                                       // reading of the motor angular position via the encoder
      vel = ((newPosition - oldPosition) / (t_cycle)) * 6.28 * 100 / 7;      // system angular velocity calculation
      oldPosition = newPosition; 
      
      ring_position = analogRead(sensorPin);                                 // reading of the position sensor signal 
      sensorValue = bit2m(ring_position);                                    // conversion of the sensor signal into a position via the defined function bit2m
      float error_ring_pos = sp_pos - sensorValue;                           // ring position error

      // uncomment for PID outer loop controller
      reg_out = (den2 * reg_out_prev - den3 * reg_out_prevprev + num1 * error_ring_pos - num2 * err_pos_prev + num3 * err_pos_prevprev); 

      // uncomment for PD outer loop controller
      //reg_out = (den2* reg_out_prev + num1 * error_ring_pos - num2 * err_pos_prev); 
      
      reg = reg_out + w0;                                                    // addition of the equilibrium velocity to the discrete controller output 
      float error_vel = (reg - vel);                                         // system angular velocity error                                   
      cmd_I = kp*error_vel;                                                  // inner loop P controller command
      cmd = int((cmd_I+0.652)*4095/(2*0.652));                               // conversion of the inner loop command into Arduino output voltage
      
     
      err_pos_prevprev = err_pos_prev;
      err_pos_prev = error_ring_pos;
      reg_out_prevprev = reg_out_prev;
      reg_out_prev = reg_out;
      sp_pos_prec = sp_pos;
      
      if (cmd < 0){                                                          // normalization of the P controller command
        cmd = 0;                                               
      }
      else if (cmd > 4095){
        cmd = 4095;
      }
      analogWrite(DAC0, cmd);                                                // command signal sent to the motor driver

      

      if (j % 10 == 0) {                                                     // saving of the data in lists every 10ms
        k = j/10;
        reg_sav[k] = reg;
        sp_sav[k] = sp_pos;
        pos_sav[k] = sensorValue;
        cmd_sav[k] = int(cmd);         
        time_sav[k] = (current_time - t_start) / 1000000.0; 
        vel_sav[k] = vel;   
      }
   
      previous_time = current_time;
      
      while (micros() - time0 < (j + 1)*t_cycle) { 
      }
    }

    analogWrite(DAC0, 2048);  
    Serial.println("Display of the saved data... ");
    for (int j = 0; j < nb_cycles/10; j = j + 1) {
      Serial.print(vel_sav[j], 3);
      Serial.print(", \t");
      Serial.print(cmd_sav[j]);
      Serial.print(", \t");
      Serial.print(reg_sav[j],2);
      Serial.print(", \t");
      Serial.print(sp_sav[j],4);
      Serial.print(", \t");
      Serial.print(pos_sav[j], 4);
      Serial.print(", \t");
      Serial.println(time_sav[j], 3);
    }

  }
}


// Function convertting a sensor signal into a ring position
// Input: sensor signal in bit
// Output: ring position in m
float bit2m(float pos_bit) {  
  float val = 0;

  if (pos_bit > 2556){
          val = 0.08;
      }
  if ((pos_bit < 2556) && (pos_bit >= 2383)){
          val = ((1/(((0.1845-0.1557)*(pos_bit-2383))/(2556-2383)+0.1557))-0.42)/100.0 + 0.03;
      }
  if ((pos_bit < 2383) && (pos_bit >= 2046)){
          val = ((1/(((0.1557-0.1347)*(pos_bit-2046))/(2383-2046)+0.1347))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 2046) && (pos_bit >= 1789)){
          val = ((1/(((0.1347-0.1187)*(pos_bit-1789))/(2046-1789)+0.1187))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1789) && (pos_bit >= 1567)){
          val = ((1/(((0.1187-0.1061)*(pos_bit-1567))/(1789-1567)+0.1061))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1567) && (pos_bit >= 1432)){
          val = ((1/(((0.1061-0.09596)*(pos_bit-1432))/(1567-1432)+0.09596))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1432) && (pos_bit >= 1331)){
          val = ((1/(((0.09596-0.08756)*(pos_bit-1331))/(1432-1331)+0.08756))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1331) && (pos_bit >= 1214)){
          val = ((1/(((0.08756-0.08051)*(pos_bit-1214))/(1331-1214)+0.08051))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1214) && (pos_bit >= 1137)){
          val = ((1/(((0.08051-0.074515)*(pos_bit-1137))/(1214-1137)+0.074515))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1137) && (pos_bit >= 1070)){
          val = ((1/(((0.074515-0.06934)*(pos_bit-1070))/(1137-1070)+0.06934))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 1070) && (pos_bit >= 994)){
          val = ((1/(((0.06934-0.06485)*(pos_bit-994))/(1070-994)+0.06485))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 994) && (pos_bit >= 931)){
          val = ((1/(((0.06485-0.0609)*(pos_bit-931))/(994-931)+0.0609))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 931) && (pos_bit >= 893)){
          val = ((1/(((0.0609-0.0574)*(pos_bit-893))/(931-893)+0.0574))-0.42)/100.0 + 0.03;
      }
   if ((pos_bit < 893) && (pos_bit >= 878)){
          val = ((1/(((0.0574-0.05549)*(pos_bit-878))/(893-878)+0.05549))-0.42)/100.0 + 0.03;
      }
   if (pos_bit < 878){
          val = 0.206;
      }
       
    return val;    
        
}



    
