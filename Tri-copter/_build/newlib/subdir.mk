################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../newlib/memory.cpp 

C_SRCS += \
../newlib/malloc_lock.c \
../newlib/newlib_syscalls.c \
../newlib/newlib_time.c \
../newlib/printf_lib.c 

OBJS += \
./newlib/malloc_lock.o \
./newlib/memory.o \
./newlib/newlib_syscalls.o \
./newlib/newlib_time.o \
./newlib/printf_lib.o 

C_DEPS += \
./newlib/malloc_lock.d \
./newlib/newlib_syscalls.d \
./newlib/newlib_time.d \
./newlib/printf_lib.d 

CPP_DEPS += \
./newlib/memory.d 


# Each subdirectory must supply rules for building sources it contributes
newlib/%.o: ../newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\newlib" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

newlib/%.o: ../newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\newlib" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\sloretz\Documents\GitHub\Tricopter\Tri-copter\L5_Application" -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


