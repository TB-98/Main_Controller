################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f429zitx.s 

OBJS += \
./Core/Startup/startup_stm32f429zitx.o 

S_DEPS += \
./Core/Startup/startup_stm32f429zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/ILI9341" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/XPT2046" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/MPU6050" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/MFRC522" -I"C:/Users/sky98/STM32CubeIDE/Ver1.0/LCD_test_0505/24C256" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

