// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Texture.proto

#ifndef PROTOBUF_Texture_2eproto__INCLUDED
#define PROTOBUF_Texture_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace jc_res_texture {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Texture_2eproto();
void protobuf_AssignDesc_Texture_2eproto();
void protobuf_ShutdownFile_Texture_2eproto();


enum PixelFormat {
  RGBA8888 = 0,
  RGB888 = 1,
  RGB565 = 2,
  RGBA4444 = 3,
  RGB5551 = 4
};
bool PixelFormat_IsValid(int value);
const PixelFormat PixelFormat_MIN = RGBA8888;
const PixelFormat PixelFormat_MAX = RGB5551;
const int PixelFormat_ARRAYSIZE = PixelFormat_MAX + 1;

const ::google::protobuf::EnumDescriptor* PixelFormat_descriptor();
inline const ::std::string& PixelFormat_Name(PixelFormat value) {
  return ::google::protobuf::internal::NameOfEnum(
    PixelFormat_descriptor(), value);
}
inline bool PixelFormat_Parse(
    const ::std::string& name, PixelFormat* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PixelFormat>(
    PixelFormat_descriptor(), name, value);
}
enum TextureFormat {
  PNG = 0,
  JPEG = 1,
  KTX = 2
};
bool TextureFormat_IsValid(int value);
const TextureFormat TextureFormat_MIN = PNG;
const TextureFormat TextureFormat_MAX = KTX;
const int TextureFormat_ARRAYSIZE = TextureFormat_MAX + 1;

const ::google::protobuf::EnumDescriptor* TextureFormat_descriptor();
inline const ::std::string& TextureFormat_Name(TextureFormat value) {
  return ::google::protobuf::internal::NameOfEnum(
    TextureFormat_descriptor(), value);
}
inline bool TextureFormat_Parse(
    const ::std::string& name, TextureFormat* value) {
  return ::google::protobuf::internal::ParseNamedEnum<TextureFormat>(
    TextureFormat_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace jc_res_texture

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::jc_res_texture::PixelFormat>() {
  return ::jc_res_texture::PixelFormat_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::jc_res_texture::TextureFormat>() {
  return ::jc_res_texture::TextureFormat_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Texture_2eproto__INCLUDED