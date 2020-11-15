################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f767zitx.s 

OBJS += \
./Core/Startup/startup_stm32f767zitx.o 

S_DEPS += \
./Core/Startup/startup_stm32f767zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f767zitx.o: ../Core/Startup/startup_stm32f767zitx.s
	arm-none-eabi-gcc -g3 -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -DUA_ENABLE_AMALGAMATION -c -IMiddlewares/Third_Party/open62541 -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f767zitx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

