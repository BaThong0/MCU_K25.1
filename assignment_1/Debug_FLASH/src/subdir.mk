################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/driver_gpio.c \
../src/driver_port.c \
../src/main.c \
../src/nvic_custom.c 

OBJS += \
./src/driver_gpio.o \
./src/driver_port.o \
./src/main.o \
./src/nvic_custom.o 

C_DEPS += \
./src/driver_gpio.d \
./src/driver_port.d \
./src/main.d \
./src/nvic_custom.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/driver_gpio.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


