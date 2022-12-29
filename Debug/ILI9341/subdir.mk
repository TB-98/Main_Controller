################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ILI9341/MA_ILI9341.c \
../ILI9341/font12.c \
../ILI9341/font16.c \
../ILI9341/font20.c \
../ILI9341/font24.c \
../ILI9341/font8.c \
../ILI9341/ili9341.c 

OBJS += \
./ILI9341/MA_ILI9341.o \
./ILI9341/font12.o \
./ILI9341/font16.o \
./ILI9341/font20.o \
./ILI9341/font24.o \
./ILI9341/font8.o \
./ILI9341/ili9341.o 

C_DEPS += \
./ILI9341/MA_ILI9341.d \
./ILI9341/font12.d \
./ILI9341/font16.d \
./ILI9341/font20.d \
./ILI9341/font24.d \
./ILI9341/font8.d \
./ILI9341/ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
ILI9341/%.o: ../ILI9341/%.c ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/ILI9341" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/XPT2046" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MPU6050" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/MFRC522" -I"C:/Users/qw/STM32CubeIDE/workspace_1.7.0/final/Main_Controller/24C256" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

