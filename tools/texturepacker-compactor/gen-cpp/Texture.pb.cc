// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Texture.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Texture.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace jc_res_texture {

namespace {

const ::google::protobuf::EnumDescriptor* PixelFormat_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* TextureFormat_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_Texture_2eproto() {
  protobuf_AddDesc_Texture_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Texture.proto");
  GOOGLE_CHECK(file != NULL);
  PixelFormat_descriptor_ = file->enum_type(0);
  TextureFormat_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Texture_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_Texture_2eproto() {
}

void protobuf_AddDesc_Texture_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rTexture.proto\022\016jc_res_texture*N\n\013Pixel"
    "Format\022\014\n\010RGBA8888\020\000\022\n\n\006RGB888\020\001\022\n\n\006RGB5"
    "65\020\002\022\014\n\010RGBA4444\020\003\022\013\n\007RGB5551\020\004*+\n\rTextu"
    "reFormat\022\007\n\003PNG\020\000\022\010\n\004JPEG\020\001\022\007\n\003KTX\020\002B\"\n "
    "com.eaglesakura.resource.texture", 192);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Texture.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Texture_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Texture_2eproto {
  StaticDescriptorInitializer_Texture_2eproto() {
    protobuf_AddDesc_Texture_2eproto();
  }
} static_descriptor_initializer_Texture_2eproto_;
const ::google::protobuf::EnumDescriptor* PixelFormat_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PixelFormat_descriptor_;
}
bool PixelFormat_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* TextureFormat_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TextureFormat_descriptor_;
}
bool TextureFormat_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace jc_res_texture

// @@protoc_insertion_point(global_scope)
