################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L2_Drivers/base/char_dev.cpp \
../L2_Drivers/base/i2c_base.cpp \
../L2_Drivers/base/uart_dev.cpp 

OBJS += \
./L2_Drivers/base/char_dev.o \
./L2_Drivers/base/i2c_base.o \
./L2_Drivers/base/uart_dev.o 

CPP_DEPS += \
./L2_Drivers/base/char_dev.d \
./L2_Drivers/base/i2c_base.d \
./L2_Drivers/base/uart_dev.d 


# Each subdirectory must supply rules for building sources it contributes
L2_Drivers/base/%.o: ../L2_Drivers/base/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\newlib" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


