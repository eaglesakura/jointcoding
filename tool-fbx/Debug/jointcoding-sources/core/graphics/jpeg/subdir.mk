################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/jpeg/JpegImageDecoder.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/jpeg/JpegStreamSource.cpp 

OBJS += \
./jointcoding-sources/core/graphics/jpeg/JpegImageDecoder.o \
./jointcoding-sources/core/graphics/jpeg/JpegStreamSource.o 

CPP_DEPS += \
./jointcoding-sources/core/graphics/jpeg/JpegImageDecoder.d \
./jointcoding-sources/core/graphics/jpeg/JpegStreamSource.d 


# Each subdirectory must supply rules for building sources it contributes
jointcoding-sources/core/graphics/jpeg/JpegImageDecoder.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/jpeg/JpegImageDecoder.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core/graphics/jpeg/JpegStreamSource.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/jpeg/JpegStreamSource.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


