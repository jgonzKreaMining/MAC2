################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/7segments.cpp \
../Core/Src/EEPROM.cpp \
../Core/Src/app.cpp \
../Core/Src/fifoUart.cpp \
../Core/Src/gpsGNSS.cpp \
../Core/Src/hardwareIn.cpp \
../Core/Src/hardwareOut.cpp \
../Core/Src/linkIn.cpp \
../Core/Src/linkOut.cpp \
../Core/Src/loraApp.cpp \
../Core/Src/loraLink.cpp \
../Core/Src/loraTcp.cpp \
../Core/Src/main.cpp \
../Core/Src/tcpIn.cpp \
../Core/Src/tcpOut.cpp 

C_SRCS += \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

C_DEPS += \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 

OBJS += \
./Core/Src/7segments.o \
./Core/Src/EEPROM.o \
./Core/Src/app.o \
./Core/Src/fifoUart.o \
./Core/Src/gpsGNSS.o \
./Core/Src/hardwareIn.o \
./Core/Src/hardwareOut.o \
./Core/Src/linkIn.o \
./Core/Src/linkOut.o \
./Core/Src/loraApp.o \
./Core/Src/loraLink.o \
./Core/Src/loraTcp.o \
./Core/Src/main.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tcpIn.o \
./Core/Src/tcpOut.o 

CPP_DEPS += \
./Core/Src/7segments.d \
./Core/Src/EEPROM.d \
./Core/Src/app.d \
./Core/Src/fifoUart.d \
./Core/Src/gpsGNSS.d \
./Core/Src/hardwareIn.d \
./Core/Src/hardwareOut.d \
./Core/Src/linkIn.d \
./Core/Src/linkOut.d \
./Core/Src/loraApp.d \
./Core/Src/loraLink.d \
./Core/Src/loraTcp.d \
./Core/Src/main.d \
./Core/Src/tcpIn.d \
./Core/Src/tcpOut.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/7segments.cyclo ./Core/Src/7segments.d ./Core/Src/7segments.o ./Core/Src/7segments.su ./Core/Src/EEPROM.cyclo ./Core/Src/EEPROM.d ./Core/Src/EEPROM.o ./Core/Src/EEPROM.su ./Core/Src/app.cyclo ./Core/Src/app.d ./Core/Src/app.o ./Core/Src/app.su ./Core/Src/fifoUart.cyclo ./Core/Src/fifoUart.d ./Core/Src/fifoUart.o ./Core/Src/fifoUart.su ./Core/Src/gpsGNSS.cyclo ./Core/Src/gpsGNSS.d ./Core/Src/gpsGNSS.o ./Core/Src/gpsGNSS.su ./Core/Src/hardwareIn.cyclo ./Core/Src/hardwareIn.d ./Core/Src/hardwareIn.o ./Core/Src/hardwareIn.su ./Core/Src/hardwareOut.cyclo ./Core/Src/hardwareOut.d ./Core/Src/hardwareOut.o ./Core/Src/hardwareOut.su ./Core/Src/linkIn.cyclo ./Core/Src/linkIn.d ./Core/Src/linkIn.o ./Core/Src/linkIn.su ./Core/Src/linkOut.cyclo ./Core/Src/linkOut.d ./Core/Src/linkOut.o ./Core/Src/linkOut.su ./Core/Src/loraApp.cyclo ./Core/Src/loraApp.d ./Core/Src/loraApp.o ./Core/Src/loraApp.su ./Core/Src/loraLink.cyclo ./Core/Src/loraLink.d ./Core/Src/loraLink.o ./Core/Src/loraLink.su ./Core/Src/loraTcp.cyclo ./Core/Src/loraTcp.d ./Core/Src/loraTcp.o ./Core/Src/loraTcp.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tcpIn.cyclo ./Core/Src/tcpIn.d ./Core/Src/tcpIn.o ./Core/Src/tcpIn.su ./Core/Src/tcpOut.cyclo ./Core/Src/tcpOut.d ./Core/Src/tcpOut.o ./Core/Src/tcpOut.su

.PHONY: clean-Core-2f-Src

