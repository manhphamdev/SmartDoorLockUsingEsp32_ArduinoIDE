#define IN1 12
#define IN2 13

byte motor_status = 0; // 0 Stop , 1 Open 2s, 2 Stop 10s, 3 Close 2s, 
byte  monitor_status = 0;// 0 Turn off , 1 Turn on Monitor 
volatile unsigned long timer_check_motor = 0;
volatile unsigned long timer_check_monitor = 0;
#define TIME_OPEN_DOOR 10000
#define TIME_OPEN_MOTOR 2000
#define TIME_CLOSE_MOTOR 2000

void checkMoto(){
// Kiem tra trang thai dong co monitor
//int motor_status = 0; // 0 Stop , 1 Open 2s, 2 Stop 10s, 3 Close 2s, 4, Stop 
//bool monitor_status = 0;// 0 Turn off , 1 Turn on Monitor 
if (motor_status == 1)
  {
    timer_check_motor = millis();
    motor_status = 2; 
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
if ((motor_status == 2) && (millis() - timer_check_motor > TIME_OPEN_MOTOR))  // Stop the motor
  {
       motor_status = 3;
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, LOW); 
  }
if ((motor_status == 3) && (millis() - timer_check_motor > TIME_OPEN_MOTOR + TIME_OPEN_DOOR))  // Stop the motor
  {
       motor_status = 4;
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, HIGH);
  }
if ((motor_status == 4) && (millis() - timer_check_motor > TIME_OPEN_MOTOR + TIME_OPEN_DOOR + TIME_CLOSE_MOTOR))  // Stop the motor
  {
       motor_status = 0;
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, LOW);

  }
}
