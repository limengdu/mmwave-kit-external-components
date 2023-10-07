#pragma once
#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif
#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#include <map>

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace mr24hpc1 {

#define FRAME_BUF_MAX_SIZE 128
#define PRODUCT_BUF_MAX_SIZE 32

#define FRAME_HEADER1_VALUE 0x53
#define FRAME_HEADER2_VALUE 0x59
#define FRAME_TAIL1_VALUE 0x54
#define FRAME_TAIL2_VALUE 0x43

#define FRAME_CONTROL_WORD_INDEX 2
#define FRAME_COMMAND_WORD_INDEX 3
#define FRAME_DATA_INDEX 6

enum
{
    FRAME_IDLE,
    FRAME_HEADER2,
    FRAME_CTL_WORLD,
    FRAME_CMD_WORLD,
    FRAME_DATA_LEN_H,
    FRAME_DATA_LEN_L,
    FRAME_DATA_BYTES,
    FRAME_DATA_CRC,
    FRAME_TAIL1,
    FRAME_TAIL2,
};

enum
{
    STANDARD_FUNCTION_QUERY_PRODUCT_MODE = 0,
    STANDARD_FUNCTION_QUERY_PRODUCT_ID,
    STANDARD_FUNCTION_QUERY_FIRMWARE_VERDION,
    STANDARD_FUNCTION_QUERY_HARDWARE_MODE,
    STANDARD_FUNCTION_QUERY_HUMAN_STATUS,
    STANDARD_FUNCTION_QUERY_SCENE_MODE,
    STANDARD_FUNCTION_QUERY_SENSITIVITY,
    STANDARD_FUNCTION_QUERY_RADAR_INIT_STATUS,
    STANDARD_FUNCTION_QUERY_MOV_TARGET_DETECTION_MAX_DISTANCE,
    STANDARD_FUNCTION_QUERY_STATIC_TARGET_DETECTION_MAX_DISTANCE,
    STANDARD_FUNCTION_QUERY_UNMANNED_TIME,
    STANDARD_FUNCTION_QUERY_RADAR_OUITPUT_INFORMATION_SWITCH,
    STANDARD_FUNCTION_MAX,

    CUSTOM_FUNCTION_QUERY_RADAR_OUITPUT_INFORMATION_SWITCH,
    CUSTOM_FUNCTION_QUERY_PRESENCE_OF_DETECTION_RANGE,
    CUSTOM_FUNCTION_QUERY_JUDGMENT_THRESHOLD_EXISTS,
    CUSTOM_FUNCTION_QUERY_MOTION_AMPLITUDE_TRIGGER_THRESHOLD,
    CUSTOM_FUNCTION_QUERY_PRESENCE_OF_PERCEPTION_BOUNDARY,
    CUSTOM_FUNCTION_QUERY_MOTION_TRIGGER_BOUNDARY,
    CUSTOM_FUNCTION_QUERY_MOTION_TRIGGER_TIME,
    CUSTOM_FUNCTION_QUERY_MOVEMENT_TO_REST_TIME,
    CUSTOM_FUNCTION_QUERY_TIME_OF_ENTER_UNMANNED,
    CUSTOM_FUNCTION_MAX,
};

enum
{
    OUTPUT_SWITCH_INIT,
    OUTPUT_SWTICH_ON,
    OUTPUT_SWTICH_OFF,
};
static char s_heartbeat_str[2][20] = {"Abnormal", "Normal"};
static char s_scene_str[5][20] = {"None", "Living Room", "Area Detection", "Washroom", "Bedroom"};
static char s_someoneExists_str[2][20] = {"Nobody", "Someone"};
static char s_motion_status_str[3][20] = {"None", "Motionless", "Active"};
static char s_keep_away_str[3][20] = {"None", "Close", "Away"};
static char s_unmanned_time_str[9][20] = {"none", "10s", "30s", "1min", "2min", "5min", "10min", "30min", "60min"};
static char s_motion_trig_boundary_str[10][5] = {"0.5m", "1.0m", "1.5m", "2.0m", "2.5m", "3.0m", "3.5m", "4.0m", "4.5m", "5.0m"};
static char s_presence_of_perception_boundary_str[10][5] = {"0.5m", "1.0m", "1.5m", "2.0m", "2.5m", "3.0m", "3.5m", "4.0m", "4.5m", "5.0m"};
static char s_presence_of_detection_range_str[7][10] = {"None", "0.5m", "1.0m", "1.5m", "2.0m", "2.5m", "3.0m"};

static uint8_t s_output_info_switch_flag = OUTPUT_SWITCH_INIT;

static uint8_t sg_recv_data_state = FRAME_IDLE;
static uint8_t sg_frame_len = 0;
static uint8_t sg_data_len = 0;
static uint8_t sg_frame_buf[FRAME_BUF_MAX_SIZE] = {0};
static uint8_t sg_frame_prase_buf[FRAME_BUF_MAX_SIZE] = {0};
static bool sg_init_flag = false;
static int sg_start_query_data = -1;
static int sg_start_query_data_max = -1;
static uint8_t sg_movementSigns_bak;
static uint32_t sg_motion_trigger_time_bak;
static uint32_t sg_move_to_rest_time_bak;
static uint32_t sg_enter_unmanned_time_bak;
static uint8_t sg_spatial_static_value_bak;
static uint8_t sg_static_distance_bak;
static uint8_t sg_spatial_motion_value_bak;
static uint8_t sg_motion_distance_bak;
static uint8_t sg_motion_speed_bak;
static uint8_t sg_heartbeat_flag = 255;
static uint8_t s_power_on_status = 0;

class mr24hpc1Component : public Component, public uart::UARTDevice {      // 类名必须是text_sensor.py定义的名字
/**** 
#define SUB_TEXT_SENSOR(name)
  protected:
   text_sensor::TextSensor *name##_text_sensor_{nullptr};
  public:
   void set_##name##_text_sensor(text_sensor::TextSensor *text_sensor) { this->name##_text_sensor_ = text_sensor; }
****/
#ifdef USE_TEXT_SENSOR
  SUB_TEXT_SENSOR(heartbeat_state)
  SUB_TEXT_SENSOR(product_model)
  SUB_TEXT_SENSOR(product_id)
  SUB_TEXT_SENSOR(hardware_model)
  SUB_TEXT_SENSOR(firware_version)
#endif

  public:
    void setup() override;
    void dump_config() override;
    void loop() override;
    void update();
    void R24_split_data_frame(uint8_t value);
    void R24_parse_data_frame(uint8_t *data, uint8_t len);
    void R24_frame_parse_product_Information(uint8_t *data);
    void send_query(uint8_t *query, size_t string_length);
    void get_product_mode(void);
    void get_product_id(void);
    void get_hardware_model(void);
    void get_firmware_version(void);
};

}  // namespace mr24hpc1
}  // namespace esphome