################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/SceneGraph.cpp 

OBJS += \
./jointcoding-sources/core/graphics/SceneGraph.o 

CPP_DEPS += \
./jointcoding-sources/core/graphics/SceneGraph.d 


# Each subdirectory must supply rules for building sources it contributes
jointcoding-sources/core/graphics/SceneGraph.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/graphics/SceneGraph.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


