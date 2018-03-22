#ifndef KREMBO_H
#define KREMBO_H

#include "application.h"
#include "battery.h"
#include "mobile_base.h"
#include "rgb_led.h"
#include "rgba_sensor.h"
#include "com_layer.h"
#include "wkc_krembot2pc.h"
#include "wkc_pc2krembot.h"
#include "bluesky_timer.h"
#include "dac_bumpers.h"
//#include "imu_sensor.h"

/*TODO:
1. add connection between photons
2. add errors checking in code
4. implement connection between master and photon (protocol)
5. add documentation
6. add documentation of Kiril wirings
7. add documentation of how to solve problems like breathing green, and how to flash version 0.6.1
8. add photon light functions from photon api
9. add photon timer functions from photon api
10. add photon logging functions from photon api
11. add documentation of wkp protocol
12. add getting photon details functions from photon api (like name)
13. change rgba_sensor to color_sensor
14. initiate Serial.begin inside photon and encapsulte Serial methods
*/

#define SEND_DATA_INTERVAL 100 //ms

enum class RGBAAddr //TODO: extract this to krembot, and make constructor here take int8
{
  Front = 0,
  FrontRight = 1,
  Right = 2,
  RearRight = 3,
  Rear = 4,
  RearLeft = 5,
  Left = 6,
  FrontLeft = 7
};

class Krembot
{
private:

  bool id_was_sent_,
  master_asks_for_data_,
  skip_led_gui_cmds_,	
  skip_base_gui_cmds_,
  bump_calib_mode_;
  COMLayer com_;
  BlueSkyTimer send_data_timer_;
  String my_name_;
  String master_ip_;
  uint16_t port_;


  WKCKrembot2PC createWKC();
  void rcveWKC();
  void sendWKC(WKCKrembot2PC& wkc_msg);
  void handleWKCFromPC(WKCPC2Krembot wkc_msg);
  void saveMyName(const char *topic, const char *data);


public:

  RGBASensor RgbaFront; ///< front color sensor
  RGBASensor RgbaRear; ///< rear color sensor
  RGBASensor RgbaRight; ///< right color sensor
  RGBASensor RgbaLeft; ///< left color sensor
  RGBASensor RgbaFrontRight; ///< front right color sensor
  RGBASensor RgbaFrontLeft; ///< front left color sensor
  RGBASensor RgbaRearRight; ///< rear right color sensor
  RGBASensor RgbaRearLeft; ///< rear left color sensor

  MobileBase Base; ///< controls the motors 
  DacBumpers Bumpers; ///< controls the bumper sensors
  Battery Bat; ///< controls the battery
  RGBLed Led; ///< controls the rgb leds

  /** 
  *   @brief  Resets the robot  
  *  
  *   @param  topic is a char array containing the topic
  *   @param  data is a char array containing the data
  *   @return void
  */  
  

  void reset(const char *topic, const char *data) {
    if (strcmp(topic,"reset")==0 && (strcmp(data,"all")==0 || strcmp(data,getName().c_str())==0) ) {
      System.reset();
    }
  }

  /** 
  *   @brief  Setups the robot with the controllers ip and port.
  *  
  *   @param  master_ip is a char array containing the controllers ip
  *   @param  port is an uint16_t containing the controllers port
  *   @return void
  */  

  void setup(String master_ip="10.0.0.3", uint16_t port=8000);

  //void setup() {
  //  setup("192.168.2.112",8000);
  //}

  /** 
  *   @brief  Loop function is called inside the loop function of the krembot_main.ino file.
  *  
  *   @return void
  */  
  void loop();


  /** 
  *   @brief  Gets the ID of the robot. 
  *  
  *   @return String, the ID of the robot
  */  
  String getID() { return System.deviceID(); }

  /** 
  *   @brief  Gets the name of the robot. 
  *  
  *   @return String, the name of the robot
  */  
  String getName() { return  my_name_; }

  /** 
  *   @brief  Checks if the robot has name. 
  *  
  *   @return bool, true if the robot's name length is longer then 0, false otherwise.
  */  
  bool have_name() {return my_name_.length()>0 ;}


  /* 
  *   Publishes and prints the battery level of the robot. 
  *  
  *   
  */  
  void pub_battery() {
    char str[50];
    int level= Bat.getBatLvl();
    sprintf(str,"%d",level);
    Particle.publish("Battery level",str);
  }

};

#endif //KREMBO_H
