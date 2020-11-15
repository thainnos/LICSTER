################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.c \
../Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.o \
./Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.d \
./Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.o: ../Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

