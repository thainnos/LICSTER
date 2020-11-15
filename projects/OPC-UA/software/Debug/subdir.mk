################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup_stm32f767xx.s 

OBJS += \
./startup_stm32f767xx.o 

S_DEPS += \
./startup_stm32f767xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup_stm32f767xx.o: ../startup_stm32f767xx.s
	arm-none-eabi-gcc -g3 -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -DUA_ENABLE_AMALGAMATION -c -IMiddlewares/Third_Party/open62541 -x assembler-with-cpp -MMD -MP -MF"startup_stm32f767xx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

