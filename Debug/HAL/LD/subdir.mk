################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/LD/LD_config.c \
../HAL/LD/LD_prog.c 

OBJS += \
./HAL/LD/LD_config.o \
./HAL/LD/LD_prog.o 

C_DEPS += \
./HAL/LD/LD_config.d \
./HAL/LD/LD_prog.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/LD/%.o: ../HAL/LD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


