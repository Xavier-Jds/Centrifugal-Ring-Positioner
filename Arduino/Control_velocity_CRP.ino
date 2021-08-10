//code for the P control of the angular velocity of the motor


#include <Encoder.h>

Encoder myEnc(47, 37);                                    // definiton of the Arduino digital pins for the encoder
float current_time;
float previous_time;
unsigned long time0;

float t_cycle=1000;                                       // sampling period (in microsecondqs ; 1000 = 1ms)
int t_exp = 10000000;   
int nb_cycles = t_exp/t_cycle;                            // duration of the motor control experiment (in microseconds)
int t_start = 10000000;                                   // start time of the motor control
int t_end = 20000000;                                     // end time of the motor control  

// Saving of the data in lists
float cmd_sav[1000];                                      // saving list of commands data
float time_sav[1000];                                     // saving list of time data
float vel_sav[1000];                                      // saving list of system angular velocity data
unsigned long t_cycle_sav[1000];    

float vel;                                                // angular velocity of the system
float newTime;                                            // involved in velocity calculation
float oldTime = 0;                                        // involved in velocity calculation
long oldPosition  = 0;                                    // involved in velocity calculation


float kp = 0.15;                                          // inner loop P controller gain
float cmd = 0;                                            // output of the inner loop P controller
float cmd_I;                                              // output of the inner loop P controller (in A)
float sp_vel = 3.15;                                      // setpoint of the system angular velocity (in rad/s)
int k;

void setup(){
  Serial.begin(9600);
  analogWriteResolution(12);
  Serial.println("Waiting phase... ");
}

void loop(){
  while (micros()<t_start){                              // waiting phase of duration t_mot to enable the motor with the switch
    analogWrite(DAC0, 2048);
  }
  
  while (micros()>=t_start and micros()<t_end){          // start of the velocity control
  Serial.println("Start of the P control of the motor... ");
  time0 = micros();
  previous_time=time0;

  

  for (int j = 0; j < nb_cycles; j = j + 1) {  
    current_time = micros();
    long newPosition = myEnc.read();                    // reading of the motor angular position via the encoder
    if (newPosition < oldPosition){                     
      newPosition = 0;
    }
    vel = ((newPosition - oldPosition) / (t_cycle)) * 6.28 * 100 / 7;          // system angular velocity calculation
    oldPosition = newPosition;

    if (micros()<t_start+1500000){                     // velocity step profile
    sp_vel = 5.5;
  }
  else if (micros()>=t_start+1500000){
    sp_vel = 3;
  }

    float error_vel = sp_vel - vel;                    // system angular velocity error
    cmd_I = kp*error_vel;                              // inner loop P controller command
    cmd = int((cmd_I+0.652)*4095/(2*0.652));           // conversion of the inner loop command into Arduino output voltage
      
      
    if (cmd < 0){                                      // normalization of the P controller command
        cmd = 0;                                               
      }
    else if (cmd > 4095){
        cmd = 4095;
      }
    analogWrite(DAC0, cmd);                            // command signal sent to the motor driver

    if (j %10 == 0) {                                  // saving of the data in lists every 10ms
        k = j/10;
        cmd_sav[k] = cmd;       
        time_sav[k]=(current_time-t_start)/1000000.0;     
        vel_sav[k]=vel;    
        //t_cycle_sav[j]=current_time-previous_time;     
    }
  
   
    
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
  //Serial.print(" ; t_cycle : ");
  //Serial.print(t_cycle_sav[j]);
  //Serial.print(" ; Temps : ");
  Serial.println(time_sav[j],3);
}

}
}
