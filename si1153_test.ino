#include "Si1153.h"

Si1153 si1153;

/**
 * Setup for configuration
 */
void setup() {
    uint8_t conf[4];
  
    Wire.begin();
    Serial.begin(115200);
/*
    Wire.beginTransmission(DEVICE_ADDR);
    Wire.write(COMMAND_ADDR);
    Wire.write(RST_CMD_CTR);
    Wire.endTransmission();
*/
    // Configure CHAN_LIST, enable channel 1 and 3
    si1153.param_set(Si1153::CHAN_LIST,5);

    /*
     * Configure timing parameters
     */
    si1153.param_set(Si1153::MEASRATE_H, 0);
    si1153.param_set(Si1153::MEASRATE_L, 1);
    si1153.param_set(Si1153::MEASCOUNT_0, 5);
    si1153.param_set(Si1153::MEASCOUNT_1, 10);
    si1153.param_set(Si1153::MEASCOUNT_2, 1);

    si1153.param_set(Si1153::THRESHOLD0_L, 0xE8);
    si1153.param_set(Si1153::THRESHOLD0_H, 0x03);

    si1153.param_set(Si1153::LED1_A, 0x3F);
    /*
     * IRQ_ENABLE
     */
    Wire.beginTransmission(Si1153::DEVICE_ADDRESS);
    Wire.write(Si1153::IRQ_ENABLE);
    Wire.write(1);
    Wire.endTransmission();

    /*
     * Configuration for channel 1
     */
    conf[0] = 0B00000000;
    conf[1] = 0B00000000; 
    conf[2] = 0B00000001;
    conf[3] = 0B11000001;
    si1153.config_channel(0, conf);

    delay(10);
    
    Serial.println("start configure channel2");
    /*
    //  * Configuation for channel 3
    //  */
    conf[0] = 0B00000000;
    conf[1] = 0B00000000; 
    conf[2] = 0B00000000;
    conf[3] = 0B00000001;
    si1153.config_channel(2, conf);

    Serial.println("finish configure channel2");

    

    // Send start command
    si1153.send_command(Si1153::START);
}

/**
 * Loops and reads data from registers
 */
void loop() {

    si1153.send_command(Si1153::PAUSE);
    si1153.send_command(Si1153::FORCE);

    Serial.println("Finish force");

    uint8_t data[4];

    data[0] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_0, 1);
    data[1] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_1, 1);
    data[2] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_2, 1);
    data[3] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_3, 1);


    Serial.println(data[0]*256 + data[1]);
    Serial.println(data[2]*256 + data[3]);

    Wire.beginTransmission(Si1153::DEVICE_ADDRESS);
    Wire.write(Si1153::IRQ_ENABLE);
    Wire.endTransmission();
    Wire.requestFrom(Si1153::DEVICE_ADDRESS,1);
    int irq = Wire.read();
    Serial.print("IRQ_ENABLE : ");
    Serial.println(irq);

    Wire.beginTransmission(Si1153::DEVICE_ADDRESS);
    Wire.write(Si1153::IRQ_STATUS);
    Wire.endTransmission();
    Wire.requestFrom(Si1153::DEVICE_ADDRESS,1);
    int irqs = Wire.read();
    Serial.print("IRQ_STATUS : ");
    Serial.println(irqs);

    si1153.send_command(Si1153::START);

    delay(1000);
}
