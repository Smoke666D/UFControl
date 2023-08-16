################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DIN_DOUT.c \
../Core/Src/EEPROM.c \
../Core/Src/LCD.c \
../Core/Src/data_model.c \
../Core/Src/datastorage.c \
../Core/Src/freertos.c \
../Core/Src/journal.c \
../Core/Src/lamp.c \
../Core/Src/main.c \
../Core/Src/menu.c \
../Core/Src/menudata.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_hal_timebase_tim.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/DIN_DOUT.o \
./Core/Src/EEPROM.o \
./Core/Src/LCD.o \
./Core/Src/data_model.o \
./Core/Src/datastorage.o \
./Core/Src/freertos.o \
./Core/Src/journal.o \
./Core/Src/lamp.o \
./Core/Src/main.o \
./Core/Src/menu.o \
./Core/Src/menudata.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_hal_timebase_tim.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/DIN_DOUT.d \
./Core/Src/EEPROM.d \
./Core/Src/LCD.d \
./Core/Src/data_model.d \
./Core/Src/datastorage.d \
./Core/Src/freertos.d \
./Core/Src/journal.d \
./Core/Src/lamp.d \
./Core/Src/main.d \
./Core/Src/menu.d \
./Core/Src/menudata.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_hal_timebase_tim.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DIN_DOUT.cyclo ./Core/Src/DIN_DOUT.d ./Core/Src/DIN_DOUT.o ./Core/Src/DIN_DOUT.su ./Core/Src/EEPROM.cyclo ./Core/Src/EEPROM.d ./Core/Src/EEPROM.o ./Core/Src/EEPROM.su ./Core/Src/LCD.cyclo ./Core/Src/LCD.d ./Core/Src/LCD.o ./Core/Src/LCD.su ./Core/Src/data_model.cyclo ./Core/Src/data_model.d ./Core/Src/data_model.o ./Core/Src/data_model.su ./Core/Src/datastorage.cyclo ./Core/Src/datastorage.d ./Core/Src/datastorage.o ./Core/Src/datastorage.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/journal.cyclo ./Core/Src/journal.d ./Core/Src/journal.o ./Core/Src/journal.su ./Core/Src/lamp.cyclo ./Core/Src/lamp.d ./Core/Src/lamp.o ./Core/Src/lamp.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/menudata.cyclo ./Core/Src/menudata.d ./Core/Src/menudata.o ./Core/Src/menudata.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_hal_timebase_tim.cyclo ./Core/Src/stm32f1xx_hal_timebase_tim.d ./Core/Src/stm32f1xx_hal_timebase_tim.o ./Core/Src/stm32f1xx_hal_timebase_tim.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

