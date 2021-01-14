#include "app_main.h"
#include "dji_m2006_motor.h"
#include "mpu6500_ist8310.h"


#include <iostream>
#include <vector>
#include <sstream>
#include <string>


using namespace stf;
using namespace std;

GPIO green_led(Green_LED_GPIO_Port, Green_LED_Pin);
GPIO red_led(Red_LED_GPIO_Port, Red_LED_Pin);
GPIO button(Button_GPIO_Port, Button_Pin);
GPIO motor_power_switch_01(Motor_Power_Switch_01_GPIO_Port, Motor_Power_Switch_01_Pin);
GPIO motor_power_switch_02(Motor_Power_Switch_02_GPIO_Port, Motor_Power_Switch_02_Pin);
GPIO motor_power_switch_03(Motor_Power_Switch_03_GPIO_Port, Motor_Power_Switch_03_Pin);
GPIO motor_power_switch_04(Motor_Power_Switch_04_GPIO_Port, Motor_Power_Switch_04_Pin);


extern UART_HandleTypeDef huart2;
USART serial(&huart2);

extern CAN_HandleTypeDef hcan1;
DjiRM::M2006_Motor motors(&hcan1, 1.00, 0.00, 0.00); // Manually Enter PID consts here (P.S.: can also use update_pid_const(...) to dynamically updating them later)

extern SPI_HandleTypeDef hspi4;
SPI ras_spi(&hspi4);

extern SPI_HandleTypeDef hspi5;
SPI imu_spi(&hspi5);
GPIO imu_chip_select(SPI5_CS_GPIO_Port, SPI5_CS_Pin);
MPU6500 imu(imu_spi, imu_chip_select);


GPIO ras_spi_cs(SPI4_CS_GPIO_Port, SPI4_CS_Pin);
GPIO ist8310_reset(IST8310_Reset_GPIO_Port, IST8310_Reset_Pin);

bool blinkLED_switch = true;


void setup(void) {
    
    // util_test(); while(1);
    
    blinkLED_switch = false;
//    serial << "=========================================================" << stf::endl;
    
    motor_power_switch_01.write(High);
    motor_power_switch_02.write(High);
    motor_power_switch_03.write(High);
    motor_power_switch_04.write(High);

    // byte_t id = imu.init(ist8310_reset);
//    serial << "IMU[MPU6500] ID = " << int(id) << stf::endl;

/*
    serial << ">";
    string start_str = serial.readWord();
    serial << start_str << stf::endl;
    while(start_str != "start") {
        serial << ">";
        start_str = serial.readWord();
        serial << start_str << stf::endl;
    }
    */
    blinkLED_switch = true;

}

void defaultLoop(void) {

	motors.init();

    // wait until white button is pressed to proceed, for safety reasons
	while(button.read() == Low){
		motors.set_current(0, 0, 0, 0);
	}

    motors.motor_test(DjiRM::Motor2);


    while(1);
    /*
    while(1) {
        imu.read_data();
        
        
        serial << imu.data_string() << stf::endl;
        
        delay(500);
    }
    */

/*
    ras_spi.set_txrx_timeout(1000000);

    string str;
    byte_t byte;
    string dummy_str = "xxxx";
    while(1) {
        serial << "[receiving ...]" << stf::endl;
        //serial << ras_spi.get_txrx_status() << ", " << ras_spi.get_txrx_status() << stf::endl;
        while(ras_spi_cs.read() == Low);
        str = ras_spi.tranceive(dummy_str); 
        //byte = ras_spi.tranceive(0xFF);
        //serial << (char)byte << stf::endl;
        serial << str << stf::endl;
        //delay(500);
    }
*/


}



void blinkLEDLoop(void) {
    
    while (blinkLED_switch == true) {
       delay(300);
       green_led.write(High);
       red_led.write(Low);
       delay(300);
       green_led.write(Low);
       red_led.write(High);
   }
}

void updatePIDLoop(void) {
//	motors.pid_update_motor_currents();
//	//delay(motors.get_ctrl_period_ms());
//	delay(1000);
}

// Allows for continuous output of motor info
void printInfoLoop(void) {
//    int16_t speed, torque;
//    uint16_t angle;
//	// serial << "Motor on" << stf::endl;
//	angle = motors.get_raw_angle(DjiRM::Motor2);
//	speed = motors.get_raw_speed(DjiRM::Motor2);
//	torque = motors.get_raw_torque(DjiRM::Motor2);
//	serial << "[Angle : " << angle  << "]";
//	serial << "[Speed : " << speed  << "]";
//	serial << "[Torque: " << torque << "]" << stf::endl;

	// float or double CANNOT be printed
	serial << (int32_t)(motors.get_velocity(DjiRM::Motor2)*100.00) << stf::endl;
}

void stf::exception(const char* str) {
    serial << stf::endl << "*****" << string(str) << stf::endl;
}
