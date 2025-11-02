include($ENV{IDF_PATH}/tools/cmake/toolchain-esp32s3.cmake)

set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE "LINKER:--whole-archive"
                                           "<LINK_ITEM>"
                                           "LINKER:--no-whole-archive")
set(CMAKE_LINK_LIBRARY_USING_WHOLE_ARCHIVE_SUPPORTED TRUE)
set(CMAKE_LINK_LIBRARY_WHOLE_ARCHIVE_ATTRIBUTES LIBRARY_TYPE=STATIC DEDUPLICATION=YES OVERRIDE=DEFAULT)


set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -fno-threadsafe-statics -fno-rtti -fno-exceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fno-threadsafe-statics -fno-rtti -fno-exceptions")



message("CMAKE_C_FLAGS   used for this build: ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS used for this build: ${CMAKE_CXX_FLAGS}")




set(FORTE_FREERTOS_INCLUDE_DIR 
  "$ENV{IDF_PATH}/components/freertos/FreeRTOS-Kernel/include"
  "$ENV{IDF_PATH}/components/freertos/config/include"
  "$ENV{IDF_PATH}/components/freertos/config/include/freertos"
  "$ENV{IDF_PATH}/components/freertos/config/xtensa/include"
  "$ENV{IDF_PATH}/components/xtensa/include"
  "$ENV{IDF_PATH}/components/xtensa/esp32s3/include"
  "$ENV{IDF_PATH}/components/freertos/FreeRTOS-Kernel/portable/xtensa/include/freertos"
  "$ENV{IDF_PATH}/components/esp_hw_support/include"
  "$ENV{IDF_PATH}/components/soc/esp32s3/include"
  "$ENV{IDF_PATH}/components/esp_common/include"
  "$ENV{IDF_PATH}/components/esp_system/include"
  "$ENV{IDF_PATH}/components/soc/esp32s3/register"
  "$ENV{IDF_PATH}/components/newlib/platform_include"
  "$ENV{IDF_PATH}/components/heap/include"
  "$ENV{IDF_PATH}/components/esp_rom/include"
  "$ENV{IDF_PATH}/components/esp_timer/include"
  "$ENV{IDF_PATH}/components/lwip/port/freertos/include" 
  "$ENV{IDF_PATH}/components/lwip/port/esp32xx/include"
  "$ENV{IDF_PATH}/components/lwip/include"
  "$ENV{IDF_PATH}/components/lwip/port/include" 
  "$ENV{IDF_PATH}/components/lwip/port/esp32xx/include" 
  "$ENV{IDF_PATH}/components/lwip/lwip/src/include"
)


set(FORTE_LOGIBUS_IO_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/managed_components/espressif__button/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/managed_components/espressif__button/interface"
  "$ENV{IDF_PATH}/components/esp_adc/include"
  "$ENV{IDF_PATH}/components/hal/include"
  "$ENV{IDF_PATH}/components/esp_driver_gpio/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/managed_components/espressif__led_indicator/include"
  "$ENV{IDF_PATH}/components/esp_driver_ledc/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/managed_components/espressif__led_strip/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/managed_components/espressif__led_strip/interface"
  "$ENV{IDF_PATH}/components/esp_driver_rmt/include"
  "$ENV{IDF_PATH}/components/esp_driver_spi/include"
)

set(FORTE_ISOBUS_UT_INCLUDE_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/App_VTClient/ext_include" 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/VTClientHelper"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/cci/ecu_driver/IsoLib"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/IsoConfig"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/AddOn"
)


set(FORTE_ISOBUS_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/App_Base_NODE1/ext_include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/cci/ecu_driver/IsoLib"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/IsoConfig"
)

set(FORTE_DATAPANEL_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/CAN2_Datapanel/include"
)

set(FORTE_BLINKMARINE_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/CAN2_Blinkmarine/include"
)


set(FORTE_SETTINGS_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/Settings"
)

set(FORTE_NVS_INCLUDE_DIR 
  "$ENV{IDF_PATH}/components/nvs_flash/include"
  "$ENV{IDF_PATH}/components/esp_partition/include"
)

set(FORTE_CAN2_IO_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/CAN2_DualDigitalInput/include"
)

set(FORTE_COUNTER_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/counter/ext_include"
)

set(FORTE_FUNK_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/../Application/components/CAN_Funk/ext_include"
)


SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ../../Application/components/CAN2_Datapanel/include)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ../../Application/components/CAN2_Blinkmarine/include)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ../../Application/components/CAN2_DualDigitalInput/include)


