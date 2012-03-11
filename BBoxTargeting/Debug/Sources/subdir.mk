################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/HelperFunctions.cpp \
../Sources/Line.cpp \
../Sources/Target.cpp \
../Sources/Threshold.cpp 

OBJS += \
./Sources/HelperFunctions.o \
./Sources/Line.o \
./Sources/Target.o \
./Sources/Threshold.o 

CPP_DEPS += \
./Sources/HelperFunctions.d \
./Sources/Line.d \
./Sources/Target.d \
./Sources/Threshold.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Iopencv_core -Iopencv_highgui -Iaruco -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


