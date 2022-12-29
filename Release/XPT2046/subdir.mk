################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../XPT2046/XPT2046_touch.c 

OBJS += \
./XPT2046/XPT2046_touch.o 

C_DEPS += \
./XPT2046/XPT2046_touch.d 


# Each subdirectory must supply rules for building sources it contributes
XPT2046/%.o: ../XPT2046/%.c XPT2046/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/ILI9341" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/XPT2046" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/MPU6050" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/MFRC522" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/24C256" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

