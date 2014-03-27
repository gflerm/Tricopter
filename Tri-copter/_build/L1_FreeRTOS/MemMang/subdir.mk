################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L1_FreeRTOS/MemMang/heap_1.c \
../L1_FreeRTOS/MemMang/heap_2.c \
../L1_FreeRTOS/MemMang/heap_3.c \
../L1_FreeRTOS/MemMang/heap_4.c 

OBJS += \
./L1_FreeRTOS/MemMang/heap_1.o \
./L1_FreeRTOS/MemMang/heap_2.o \
./L1_FreeRTOS/MemMang/heap_3.o \
./L1_FreeRTOS/MemMang/heap_4.o 

C_DEPS += \
./L1_FreeRTOS/MemMang/heap_1.d \
./L1_FreeRTOS/MemMang/heap_2.d \
./L1_FreeRTOS/MemMang/heap_3.d \
./L1_FreeRTOS/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
L1_FreeRTOS/MemMang/%.o: ../L1_FreeRTOS/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"D:\projects\Tri-copter" -I"D:\projects\Tri-copter\newlib" -I"D:\projects\Tri-copter\L0_LowLevel" -I"D:\projects\Tri-copter\L1_FreeRTOS" -I"D:\projects\Tri-copter\L1_FreeRTOS\include" -I"D:\projects\Tri-copter\L1_FreeRTOS\portable" -I"D:\projects\Tri-copter\L2_Drivers" -I"D:\projects\Tri-copter\L2_Drivers\base" -I"D:\projects\Tri-copter\L3_Utils" -I"D:\projects\Tri-copter\L3_Utils\tlm" -I"D:\projects\Tri-copter\L4_IO" -I"D:\projects\Tri-copter\L4_IO\fat" -I"D:\projects\Tri-copter\L4_IO\wireless" -I"D:\projects\Tri-copter\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


