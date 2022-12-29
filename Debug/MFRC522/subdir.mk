################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MFRC522/MFRC522.c 

OBJS += \
./MFRC522/MFRC522.o 

C_DEPS += \
./MFRC522/MFRC522.d 


# Each subdirectory must supply rules for building sources it contributes
MFRC522/%.o: ../MFRC522/%.c MFRC522/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/ILI9341" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/XPT2046" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MPU6050" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MFRC522" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/24C256" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

