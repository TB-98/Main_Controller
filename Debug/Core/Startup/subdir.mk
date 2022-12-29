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
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/ILI9341" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/XPT2046" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MPU6050" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MFRC522" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/24C256" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

