/****************************************************
 * Gesture-Controlled Trash Bin Bot
 * Board: Seeed Studio XIAO ESP32S3
 * Vision: Grove Vision AI Module V2
 * Framework: Arduino
 ****************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "SSCMA.h"   // Seeed Studio Vision AI communication library

/******************** I2C CONFIG ********************/
#define I2C_SDA  SDA
#define I2C_SCL  SCL
#define VISION_I2C_ADDR 0x62   // Default Vision V2 I2C address

SSCMA AI;

/******************** MOTOR PINS ********************/
// Motor driver pin definitions
#define MOTOR_1_A D10
#define MOTOR_1_B D9
#define MOTOR_2_A D8
#define MOTOR_2_B D7
#define MOTOR_3_A D0
#define MOTOR_3_B D1
#define MOTOR_4_A D2
#define MOTOR_4_B D3

/******************** SERVO CONFIG ******************/
#define SERVO_PIN        D6
#define SERVO_CHANNEL    0
#define SERVO_FREQ       50
#define SERVO_RESOLUTION 10

/******************** GLOBAL VARS *******************/
int servo_angle = 0;

/******************** MOTOR FUNCTIONS ***************/
void motor_init()
{
  pinMode(MOTOR_1_A, OUTPUT);
  pinMode(MOTOR_1_B, OUTPUT);
  pinMode(MOTOR_2_A, OUTPUT);
  pinMode(MOTOR_2_B, OUTPUT);
  pinMode(MOTOR_3_A, OUTPUT);
  pinMode(MOTOR_3_B, OUTPUT);
  pinMode(MOTOR_4_A, OUTPUT);
  pinMode(MOTOR_4_B, OUTPUT);

  digitalWrite(MOTOR_1_A, LOW);
  digitalWrite(MOTOR_1_B, LOW);
  digitalWrite(MOTOR_2_A, LOW);
  digitalWrite(MOTOR_2_B, LOW);
  digitalWrite(MOTOR_3_A, LOW);
  digitalWrite(MOTOR_3_B, LOW);
  digitalWrite(MOTOR_4_A, LOW);
  digitalWrite(MOTOR_4_B, LOW);
}

void motor_stop()
{
  digitalWrite(MOTOR_1_A, LOW);
  digitalWrite(MOTOR_1_B, LOW);
  digitalWrite(MOTOR_2_A, LOW);
  digitalWrite(MOTOR_2_B, LOW);
  digitalWrite(MOTOR_3_A, LOW);
  digitalWrite(MOTOR_3_B, LOW);
  digitalWrite(MOTOR_4_A, LOW);
  digitalWrite(MOTOR_4_B, LOW);
}

void motor_forward()
{
  digitalWrite(MOTOR_1_A, HIGH);
  digitalWrite(MOTOR_1_B, LOW);
  digitalWrite(MOTOR_2_A, HIGH);
  digitalWrite(MOTOR_2_B, LOW);

  digitalWrite(MOTOR_3_A, HIGH);
  digitalWrite(MOTOR_3_B, LOW);
  digitalWrite(MOTOR_4_A, HIGH);
  digitalWrite(MOTOR_4_B, LOW);
}

void motor_backward()
{
  digitalWrite(MOTOR_1_A, LOW);
  digitalWrite(MOTOR_1_B, HIGH);
  digitalWrite(MOTOR_2_A, LOW);
  digitalWrite(MOTOR_2_B, HIGH);

  digitalWrite(MOTOR_3_A, LOW);
  digitalWrite(MOTOR_3_B, HIGH);
  digitalWrite(MOTOR_4_A, LOW);
  digitalWrite(MOTOR_4_B, HIGH);
}

/******************** SERVO FUNCTIONS ***************/
void servo_init()
{
  ledcSetup(SERVO_CHANNEL, SERVO_FREQ, SERVO_RESOLUTION);
  ledcAttachPin(SERVO_PIN, SERVO_CHANNEL);
}

void servo_set_angle(int angle)
{
  angle = constrain(angle, 0, 180);

  // 10-bit resolution, 50Hz:
  // 0°  -> ~1ms pulse  -> duty ≈ 51
  // 180°-> ~2ms pulse  -> duty ≈ 102
  int duty = map(angle, 0, 180, 51, 102);
  ledcWrite(SERVO_CHANNEL, duty);
}

/******************** GESTURE HANDLERS **************/
// Target ID meanings depend on the model
// Example mapping:
// 0 -> Rock     -> Move Forward
// 1 -> Paper    -> Move Backward
// 2 -> Scissors -> Toggle Servo

void handle_target_0()
{
  Serial.println("Gesture: Rock -> Forward");
  motor_forward();
}

void handle_target_1()
{
  Serial.println("Gesture: Paper -> Backward");
  motor_backward();
}

void handle_target_2()
{
  Serial.println("Gesture: Scissors -> Servo Toggle");
  servo_angle = (servo_angle == 0) ? 90 : 0;
  servo_set_angle(servo_angle);
}

/******************** SETUP *************************/
void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Gesture-Controlled Trash Bin Bot");

  // I2C init
  Wire.begin(I2C_SDA, I2C_SCL);

  // Vision AI init
  if (!AI.begin(Wire, VISION_I2C_ADDR))
  {
    Serial.println("Failed to init Vision AI Module");
    while (1);
  }
  Serial.println("Vision AI Module Ready");

  motor_init();
  servo_init();
  servo_set_angle(0);
}

/******************** LOOP **************************/
void loop()
{
  sscma::Frame frame;

  // Try to read inference results
  if (AI.invoke(frame))
  {
    if (frame.boxes.size() > 0)
    {
      int target_id = frame.boxes[0].target;

      switch (target_id)
      {
        case 0:
          handle_target_0();
          break;

        case 1:
          handle_target_1();
          break;

        case 2:
          handle_target_2();
          break;

        default:
          motor_stop();
          break;
      }
    }
    else
    {
      motor_stop();
    }
  }

  delay(100);
}
