// Copyright 2021 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

// This file was generated by generate_device_frontend.py from Device.cpp

#include "TreeDevice.h"
#include "anari/backend/Library.h"
#include <cstdarg>
#include <cstdint>

// debug interface
#include "anari/ext/debug/DebugObject.h"

namespace anari_sdk{
namespace tree{


template <typename T>
void writeToVoidP(void *_p, T v)
{
   T *p = (T *)_p;
   *p = v;
}

int TreeDevice::deviceImplements(const char *extension) {
   (void)extension;
   return 0;
}

void * TreeDevice::mapArray(ANARIArray handle) {
   if(auto obj = fromHandle<ArrayObjectBase*>(handle)) {
      return obj->map();
   } else {
      return nullptr;
   }
}
void TreeDevice::unmapArray(ANARIArray handle) {
   if(auto obj = fromHandle<ArrayObjectBase*>(handle)) {
      obj->unmap();
   }
}

anari::debug_device::ObjectFactory *getDebugFactory();

int TreeDevice::getProperty(ANARIObject handle,
      const char *name,
      ANARIDataType type,
      void *mem,
      uint64_t size,
      ANARIWaitMask mask) {
   if(handle == this_device()) {
      if(std::strncmp(name, "debugObjects", 12) == 0 && type == ANARI_FUNCTION_POINTER) {
         writeToVoidP(mem, getDebugFactory);
         return 1;
      } else if(std::strncmp(name, "version", 7) == 0 && type == ANARI_INT32) {
         writeToVoidP<std::int32_t>(mem, 1); // TODO: set this to actual value
         return 1;
      } else if(std::strncmp(name, "geometryMaxIndex", 16) == 0 && type == ANARI_UINT64) {
         writeToVoidP<std::uint64_t>(mem, UINT32_MAX); // TODO: set this to actual value
         return 1;
      } else {
         return 0;
      }
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      return obj->getProperty(name, type, mem, size, mask);
   } else {
      return 0;
   }
}

void TreeDevice::setParameter(ANARIObject handle,
      const char *name,
      ANARIDataType type,
      const void *mem) {
   if(handle == this_device()) {
      staging.set(name, type, mem);
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->set(name, type, mem);
   }
}

void TreeDevice::unsetParameter(ANARIObject handle, const char *name) {
   if(handle == this_device()) {
      staging.unset(name);
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->unset(name);
   }
}

void TreeDevice::commit(ANARIObject handle) {
   if(handle == this_device()) {
      current = staging;
      if(current.statusCallback.get(ANARI_STATUS_CALLBACK, &statusCallback)) {
         statusCallbackUserData = nullptr;
         current.statusCallbackUserData.get(ANARI_VOID_POINTER, &statusCallbackUserData);
      } else {
         statusCallback = defaultStatusCallback();
         statusCallbackUserData = defaultStatusCallbackUserPtr();
      }
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->commit();
   }
}

void TreeDevice::release(ANARIObject handle) {
   if(handle == this_device()) {
      if(refcount.fetch_sub(1) == 1) {
         delete this;
      }
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->release();
   }
}
void TreeDevice::retain(ANARIObject handle) {
   if(handle == this_device()) {
      refcount++;
   } else if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->retain();
   }
}
void TreeDevice::releaseInternal(ANARIObject handle, ANARIObject owner) {
   if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->releaseInternal(owner);
   }
}
void TreeDevice::retainInternal(ANARIObject handle, ANARIObject owner) {
   if(auto obj = fromHandle<ObjectBase*>(handle)) {
      obj->retainInternal(owner);
   }
}

const void* TreeDevice::frameBufferMap(ANARIFrame handle, const char *channel) {
   if(auto obj = fromHandle<FrameObjectBase*>(handle)) {
      return obj->mapFrame(channel);
   } else {
      return 0;
   }
}

void TreeDevice::frameBufferUnmap(ANARIFrame handle, const char *channel) {
if(auto obj = fromHandle<FrameObjectBase*>(handle)) {
      obj->unmapFrame(channel);
   }
}

void TreeDevice::renderFrame(ANARIFrame handle) {
   if(auto obj = fromHandle<FrameObjectBase*>(handle)) {
      obj->renderFrame();
   }
}
int TreeDevice::frameReady(ANARIFrame handle, ANARIWaitMask mask) {
   if(auto obj = fromHandle<FrameObjectBase*>(handle)) {
      return obj->frameReady(mask);
   } else {
      return 0;
   }
}
void TreeDevice::discardFrame(ANARIFrame handle) {
   if(auto obj = fromHandle<FrameObjectBase*>(handle)) {
      obj->discardFrame();
   }
}

/////////////////////////////////////////////////////////////////////////////
// Helper/other functions and data members
/////////////////////////////////////////////////////////////////////////////

TreeDevice::TreeDevice()
   : refcount(1),
   staging(this_device(), this_device()),
   current(this_device(), this_device())
{
   objects.emplace_back(nullptr); // reserve the null index for the null handle
}
TreeDevice::~TreeDevice() {

}

// query functions
const char ** query_object_types(ANARIDataType type);
const ANARIParameter * query_params(ANARIDataType type, const char *subtype);
const void * query_param_info(ANARIDataType type, const char *subtype,
   const char *paramName, ANARIDataType paramType,
   const char *infoName, ANARIDataType infoType);

// internal "api" functions
void anariRetainInternal(ANARIDevice d, ANARIObject handle, ANARIObject owner) {
   reinterpret_cast<TreeDevice*>(d)->retainInternal(handle, owner);
}
void anariReleaseInternal(ANARIDevice d, ANARIObject handle, ANARIObject owner) {
   reinterpret_cast<TreeDevice*>(d)->releaseInternal(handle, owner);
}
void anariDeleteInternal(ANARIDevice d, ANARIObject handle) {
   reinterpret_cast<TreeDevice*>(d)->deallocate(handle);
}
void anariReportStatus(ANARIDevice handle,
      ANARIObject source,
      ANARIDataType sourceType,
      ANARIStatusSeverity severity,
      ANARIStatusCode code,
      const char *format, ...) {

   if(TreeDevice *d = deviceHandle<TreeDevice*>(handle)) {
      if(d->statusCallback) {
         va_list arglist;
         va_list arglist_copy;
         va_start(arglist, format);
         va_copy(arglist_copy, arglist);
         int count = std::vsnprintf(nullptr, 0, format, arglist);
         va_end(arglist);

         std::vector<char> formattedMessage(size_t(count+1));

         va_start(arglist_copy, format);
         std::vsnprintf(formattedMessage.data(), size_t(count+1), format, arglist_copy);
         va_end( arglist_copy );

         d->statusCallback(d->statusCallbackUserData, d->this_device(),
            source, sourceType, severity, code, formattedMessage.data());
      }
   }
}

} //namespace anari_sdk
} //namespace tree



static char deviceName[] = "tree";

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_NEW_DEVICE(
      tree, subtype)
{
   if (subtype == std::string("default") || subtype == std::string("tree"))
      return (ANARIDevice) new anari_sdk::tree::TreeDevice();
   return nullptr;
}

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_INIT(tree)
{

}

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_GET_DEVICE_SUBTYPES(
      tree, libdata)
{
   (void)libdata;
   static const char *devices[] = {deviceName, nullptr};
   return devices;
}

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_GET_OBJECT_SUBTYPES(
      tree, libdata, deviceSubtype, objectType)
{
   (void)libdata;
   (void)deviceSubtype;
   return anari_sdk::tree::query_object_types(objectType);
}

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_GET_OBJECT_PARAMETERS(
      tree, libdata, deviceSubtype, objectSubtype, objectType)
{
   (void)libdata;
   (void)deviceSubtype;
   return anari_sdk::tree::query_params(objectType, objectSubtype);
}

extern "C" DEVICE_INTERFACE ANARI_DEFINE_LIBRARY_GET_PARAMETER_PROPERTY(
      tree,
      libdata,
      deviceSubtype,
      objectSubtype,
      objectType,
      parameterName,
      parameterType,
      propertyName,
      propertyType)
{
   (void)libdata;
   (void)deviceSubtype;
   return anari_sdk::tree::query_param_info(
      objectType,
      objectSubtype,
      parameterName,
      parameterType,
      propertyName,
      propertyType);
}
