################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/examples/examples.cpp \
../L5_Application/examples/rn_xv_task.cpp 

OBJS += \
./L5_Application/examples/examples.o \
./L5_Application/examples/rn_xv_task.o 

CPP_DEPS += \
./L5_Application/examples/examples.d \
./L5_Application/examples/rn_xv_task.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/examples/%.o: ../L5_Application/examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"D:\projects\Tri-copter" -I"D:\projects\Tri-copter\newlib" -I"D:\projects\Tri-copter\L0_LowLevel" -I"D:\projects\Tri-copter\L1_FreeRTOS" -I"D:\projects\Tri-copter\L1_FreeRTOS\include" -I"D:\projects\Tri-copter\L1_FreeRTOS\portable" -I"D:\projects\Tri-copter\L2_Drivers" -I"D:\projects\Tri-copter\L2_Drivers\base" -I"D:\projects\Tri-copter\L3_Utils" -I"D:\projects\Tri-copter\L3_Utils\tlm" -I"D:\projects\Tri-copter\L4_IO" -I"D:\projects\Tri-copter\L4_IO\fat" -I"D:\projects\Tri-copter\L4_IO\wireless" -I"D:\projects\Tri-copter\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


