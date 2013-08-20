################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../JNI/struct_martyr_Ontology.cpp 

OBJS += \
./JNI/struct_martyr_Ontology.o 

CPP_DEPS += \
./JNI/struct_martyr_Ontology.d 


# Each subdirectory must supply rules for building sources it contributes
JNI/%.o: ../JNI/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


