################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/source/10dof.cpp \
../L5_Application/source/ServoController.cpp \
../L5_Application/source/high_level_init.cpp \
../L5_Application/source/remote.cpp \
../L5_Application/source/terminal.cpp 

C_SRCS += \
../L5_Application/source/BM_Controller.c \
../L5_Application/source/LCD.c 

OBJS += \
./L5_Application/source/10dof.o \
./L5_Application/source/BM_Controller.o \
./L5_Application/source/LCD.o \
./L5_Application/source/ServoController.o \
./L5_Application/source/high_level_init.o \
./L5_Application/source/remote.o \
./L5_Application/source/terminal.o 

C_DEPS += \
./L5_Application/source/BM_Controller.d \
./L5_Application/source/LCD.d 

CPP_DEPS += \
./L5_Application/source/10dof.d \
./L5_Application/source/ServoController.d \
./L5_Application/source/high_level_init.d \
./L5_Application/source/remote.d \
./L5_Application/source/terminal.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/source/%.o: ../L5_Application/source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\newlib" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L5_Application/source/%.o: ../L5_Application/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\newlib" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\Michael\Documents\GitHub\Tricopter\Tri-copter\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


