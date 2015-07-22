################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
./1charger.c \
./display.c \
./tinywatt.c \
./strutils.c  \
./tx_log.c


OBJS += \
./1charger.o \
./display.o \
./tinywatt.o \
./strutils.o \
./tx_log.o

C_DEPS += \
./1charger.d \
./display.d \
./tinywatt.d \
./strutils.d \
./tx_log.d

# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -DBUILD_SIMPLE -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=attiny861 -DF_CPU=1000000UL  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


