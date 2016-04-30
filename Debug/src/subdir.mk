################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/indexador.cpp \
../src/indexador01.cpp \
../src/indexador02.cpp \
../src/indexador03.cpp \
../src/indexador04.cpp \
../src/indexador05.cpp \
../src/indexador06.cpp \
../src/indexador07.cpp \
../src/indexador08.cpp \
../src/indexador09.cpp \
../src/indexador10.cpp \
../src/indexador11.cpp 

OBJS += \
./src/indexador.o \
./src/indexador01.o \
./src/indexador02.o \
./src/indexador03.o \
./src/indexador04.o \
./src/indexador05.o \
./src/indexador06.o \
./src/indexador07.o \
./src/indexador08.o \
./src/indexador09.o \
./src/indexador10.o \
./src/indexador11.o 

CPP_DEPS += \
./src/indexador.d \
./src/indexador01.d \
./src/indexador02.d \
./src/indexador03.d \
./src/indexador04.d \
./src/indexador05.d \
./src/indexador06.d \
./src/indexador07.d \
./src/indexador08.d \
./src/indexador09.d \
./src/indexador10.d \
./src/indexador11.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


