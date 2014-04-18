################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L3_Utils/tlm/src/c_tlm_binary.c \
../L3_Utils/tlm/src/c_tlm_comp.c \
../L3_Utils/tlm/src/c_tlm_stream.c \
../L3_Utils/tlm/src/c_tlm_var.c 

OBJS += \
./L3_Utils/tlm/src/c_tlm_binary.o \
./L3_Utils/tlm/src/c_tlm_comp.o \
./L3_Utils/tlm/src/c_tlm_stream.o \
./L3_Utils/tlm/src/c_tlm_var.o 

C_DEPS += \
./L3_Utils/tlm/src/c_tlm_binary.d \
./L3_Utils/tlm/src/c_tlm_comp.d \
./L3_Utils/tlm/src/c_tlm_stream.d \
./L3_Utils/tlm/src/c_tlm_var.d 


# Each subdirectory must supply rules for building sources it contributes
L3_Utils/tlm/src/%.o: ../L3_Utils/tlm/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"C:\Users\sloretz\git\Tricopter\Tri-copter" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\newlib" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L0_LowLevel" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L1_FreeRTOS" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L1_FreeRTOS\include" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L1_FreeRTOS\portable" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L2_Drivers" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L2_Drivers\base" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L3_Utils" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L3_Utils\tlm" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L4_IO" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L4_IO\fat" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L4_IO\wireless" -I"C:\Users\sloretz\git\Tricopter\Tri-copter\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


