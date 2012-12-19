################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/Mesh.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/Node.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/prv_FbxDeformer.cpp 

OBJS += \
./jointcoding-sources/core-fbx/node/Mesh.o \
./jointcoding-sources/core-fbx/node/Node.o \
./jointcoding-sources/core-fbx/node/prv_FbxDeformer.o 

CPP_DEPS += \
./jointcoding-sources/core-fbx/node/Mesh.d \
./jointcoding-sources/core-fbx/node/Node.d \
./jointcoding-sources/core-fbx/node/prv_FbxDeformer.d 


# Each subdirectory must supply rules for building sources it contributes
jointcoding-sources/core-fbx/node/Mesh.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/Mesh.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core-fbx/node/Node.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/Node.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core-fbx/node/prv_FbxDeformer.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core-fbx/node/prv_FbxDeformer.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


