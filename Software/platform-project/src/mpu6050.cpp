#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <mpu6050.h>


Adafruit_MPU6050 mpu;
unsigned long mpu6050Times = 0;                                               //Record the device running time
float mpu6050Temp = 0;                                                        //Define a variable
float xAcceleration , yAcceleration , zAcceleration ;                         //Define three variables
float xAccele , yAccele , zAccele ;                                           //Define three variables
float xGyro = 0, yGyro = 0, zGyro = 0;                                        //Define three variables
float gravity = 9.8;                                                          //Define a variable

position_state_t position;
void mpu6050_init() 
{

  Wire.begin(33, 32);
  Serial.println("Adafruit MPU6050 test!");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}


position_state_t get_position_state()
{
    return position;
}


/**
 * @brief 整理是检测刷牙动作的，没有细调，需要研究可以拿MPU6050的值去研究
 * 
 */
void mpu6050_task() {

    static uint32_t timeout = 0;
    if (millis() - timeout >= 100) {                       //This command is executed once in a while
        timeout = millis();
    
        sensors_event_t a, g, temp;                                               //Set three variables
        mpu.getEvent(&a, &g, &temp);                                              //Read the corresponding three values
    
        xAcceleration = a.acceleration.x ;                                        //Acquired acceleration
        yAcceleration = a.acceleration.y ;                                        //Acquired acceleration
    
    
        xAccele = (xAcceleration / gravity) * 100;                                        //Convert the units of acceleration into g
        yAccele = (yAcceleration / gravity) * 100;                                        //Convert the units of acceleration into g
    
        // Serial.print(" , x-accele: ");
        // Serial.print(xAccele);                                                    //Serial print acceleration
        // Serial.print(" , y-accele: ");
        // Serial.print(yAccele);                                                    //Serial print acceleration
        
        if (xAccele > -90 && xAccele < - 40 && yAccele > -90 && yAccele < 0)
        {
            // Serial.print(" 左上");
            position = POSITION_LEFT_TOP;
        }
        else if (xAccele > -40 && xAccele < 40 && yAccele > -90 && yAccele < 0)
        {
            position = POSITION_TOP;
        }
        else if (xAccele > 40 && xAccele < 90 && yAccele > -90 && yAccele < 0)
        {
            // Serial.print(" 右上");
            position = POSITION_RIGH_TOP;
        }
        else if (xAccele > 0 && xAccele < 90 && yAccele > 0 && yAccele < 90)
        {
            // Serial.print(" 右下");
            position = POSITION_RIGH_DOWN;
        }
        else if (xAccele > -90 && xAccele < 0 && yAccele > 0 && yAccele < 90)
        {
            // Serial.print(" 左下");
            position = POSITION_LEFT_DOWN;
        }
        else
        {
            position = POSITION_LEFT_UNKNOW;
        }
        // Serial.println();       

    }
}
