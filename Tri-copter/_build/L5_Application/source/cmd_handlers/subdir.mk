################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/source/cmd_handlers/handlers.cpp \
../L5_Application/source/cmd_handlers/prog_handlers.cpp \
../L5_Application/source/cmd_handlers/wireless_handlers.cpp 

OBJS += \
./L5_Application/source/cmd_handlers/handlers.o \
./L5_Application/source/cmd_handlers/prog_handlers.o \
./L5_Application/source/cmd_handlers/wireless_handlers.o 

CPP_DEPS += \
./L5_Application/source/cmd_handlers/handlers.d \
./L5_Application/source/cmd_handlers/prog_handlers.d \
./L5_Application/source/cmd_handlers/wireless_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/source/cmd_handlers/%.o: ../L5_Application/source/cmd_handlers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Git\projects\Tricopter\Tri-copter" -I"C:\Git\projects\Tricopter\Tri-copter\newlib" -I"C:\Git\projects\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Git\projects\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Git\projects\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Git\projects\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Git\projects\Tricopter\Tri-copter\L2_Drivers" -I"C:\Git\projects\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Git\projects\Tricopter\Tri-copter\L3_Utils" -I"C:\Git\projects\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Git\projects\Tricopter\Tri-copter\L4_IO" -I"C:\Git\projects\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Git\projects\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Git\projects\Tricopter\Tri-copter\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


