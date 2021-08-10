// code for the open loop step response of the motor 


#include <Encoder.h>

Encoder myEnc(47, 37);                                    // definiton of the Arduino digital pins for the encoder
float current_time;
float previous_time;
unsigned long time0;

float t_cycle=10000;                                      // sampling period (in microsecondqs ; 1000 = 1ms)
int t_test = 40000000;   
int nb_cycles = t_test/t_cycle;                            // duration of the motor control experiment (in microseconds)
int t_start = 10000000;                                   // start time of the experiement
int t_end = t_start + t_test;                             // end time of the experiment
int t_step = t_start + 20000000;                          // time of the step change in the motor current

// Saving of the data in lists
float cmd_sav[4000];                                      // saving list of commands data
float time_sav[4000];                                     // saving list of time data
float vel_sav[4000];                                      // saving list of system angular velocity data
unsigned long t_cycle_sav[4000];    


float vel;                                                // angular velocity of the system
float newTime;                                            // involved in velocity calculation
float oldTime = 0;                                        // involved in velocity calculation
long oldPosition  = 0;                                    // involved in velocity calculation

float cmd1 = 2114;                                        // motor command before the step
float cmd2 = 2130;                                        // motor command after the step
float cmd;                                                // actual motor command


void setup(){
  Serial.begin(9600);
  analogWriteResolution(12);
  Serial.println("Waiting phase... ");
}

void loop(){
  while (micros()<t_start){                              // waiting phase of duration t_start to enable the motor with the switch
    analogWrite(DAC0, 2048);
  }
  
  while (micros()>=t_start and micros()<t_end){          // start of the experiment
  Serial.println("Start of the experiment... ");
  time0 = micros();
  previous_time=time0;


  for (int j = 0; j < nb_cycles; j = j + 1) {    
    current_time = micros();  
    if (micros()<t_step){
      cmd = cmd1;
    }
    else {
      cmd = cmd2;
    }
    long newPosition = myEnc.read();                     // reading of the motor angular position via the encoder
    if (newPosition < oldPosition){                     
      newPosition = 0;
    }
    vel = ((newPosition - oldPosition) / (t_cycle)) * 6.28 * 100 / 7;          // system angular velocity calculation
    oldPosition = newPosition;
    
  
    analogWrite(DAC0, cmd);
  
    // saving of the data in lists every 10ms
    cmd_sav[j] = cmd;       
    time_sav[j]=(current_time-t_start)/1000000.0;     
    vel_sav[j]=vel;
    previous_time =current_time;  
    
    previous_time=current_time;  
    while(micros()-time0 < (j+1)*t_cycle){   
      }
  }
  
    analogWrite(DAC0, 2048);  
    Serial.println("Display of the saved data... ");
    for (int j = 0; j < nb_cycles/10; j = j + 1) {
      Serial.print(vel_sav[j],3);
      Serial.print(", \t");
      Serial.print(cmd_sav[j]);
      Serial.print(", \t");
      Serial.println(time_sav[j],3);
}

}

}
