TARGET_CHIP := NRF51822_QFAA_C0
BOARD := LEDBOARD
CFLAGS += -DNRF51822_QFAA_C0
PLATFORM=POSIX
LDFLAGS += -D__HEAP_SIZE=0

GNU_INSTALL_ROOT := /Users/bpatterson/Documents/workspace/wildlights/nrfworkspace/gcc-arm-none-eabi-4_9-2015q1
GNU_VERSION := 4.7.4
GNU_PREFIX := arm-none-eabi

C_SOURCE_PATHS += ../src/
C_SOURCE_PATHS += ../src/services
C_SOURCE_FILES += src/main.c
C_SOURCE_FILES += src/light_ws2812_cortex.c
C_SOURCE_FILES += src/LEDVM.c
C_SOURCE_FILES += src/LEDMain.c
C_SOURCE_FILES += src/battery.c
C_SOURCE_FILES += src/services/ble_led_program.c
C_SOURCE_FILES += nrf_delay.c
C_SOURCE_FILES += ble_bas.c
C_SOURCE_FILES += app_timer.c
C_SOURCE_FILES += ble_advdata.c
C_SOURCE_FILES += ble_bondmngr.c
C_SOURCE_FILES += ble_conn_params.c
C_SOURCE_FILES += ble_flash.c
C_SOURCE_FILES += ble_srv_common.c
C_SOURCE_FILES += ble_radio_notification.c
C_SOURCE_FILES += ble_stack_handler.c
C_SOURCE_FILES += ble_dis.c

OUTPUT_FILENAME := LEDMain
#SDK_PATH =c:/Nordic~1/NRF51S~1.335/Nordic/nrf51822/
#SDK_PATH = z:/nrfworkspace/LEDProject/nrf51822/
SDK_PATH = /Users/bpatterson/Documents/workspace/wildlights/nrfworkspace/LEDProject/nrf51822/


INCLUDEPATHS += -I"${SDK_PATH}Include/ble"
INCLUDEPATHS += -I"${SDK_PATH}Include/ble/softdevice"
INCLUDEPATHS += -I"${SDK_PATH}Include/app_common"
INCLUDEPATHS += -I"${SDK_PATH}Include/ble/ble_services"
INCLUDEPATHS += -I"../include/"


#Uncomment correct line if you have softdevice programmed on the chip.
#DEVICE_VARIANT := xxaa
DEVICE_VARIANT := xxab

USE_SOFTDEVICE := S110
#USE_SOFTDEVICE := S210

include $(SDK_PATH)Source/templates/gcc/Makefile.common

JLINK_HOME=/usr/bin/
JLINK_EXE=JLinkExe
JLINK_GDBSERVER=JLinkGDBServer
GDB=$(GNU_INSTALL_ROOT)/$(GNU_PREFIX)-gdb
SOFTDEVICE=/Users/bpatterson/Documents/workspace/wildlights/nrfworkspace/LEDProject/softdevice/s110_nrf51822_5.2.1_softdevice.hex

pin-reset:
	$(JLINK_EXE) reset.jlink
upload:
	$(JLINK_EXE) upload.jlink
startdebug:
	$(JLINK_GDBSERVER) -single -if swd -speed 1000 -port 2331 
erase-all:
	$(JLINK_EXE) erase-all.jlink
recover:
	$(JLINK_EXE) recover.jlink
	