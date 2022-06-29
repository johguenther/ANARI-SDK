// This file was generated by $generator from $template

#include "anari/type_utility.h"
#include "$prefixSpecializations.h"

#include <cstdlib>
#include <cstring>

$begin_namespaces

Object<Frame>::Object(ANARIDevice d, ANARIObject handle)
 : FrameObjectBase(d, handle), staging(d, handle), current(d, handle)
{

}

bool Object<Frame>::set(const char *paramname, ANARIDataType type, const void *mem) {
   return staging.set(paramname, type, mem);
}
void Object<Frame>::unset(const char *paramname) {
   staging.unset(paramname);
}
void Object<Frame>::commit() {
   current = staging;
   uint32_t size[2] = {16, 16}; // some non null placeholder value
   current.size.get(ANARI_UINT32_VEC2, size);

   uint32_t elements = size[0]*size[1];

   ANARIDataType colorType = ANARI_UNKNOWN;
   ANARIDataType depthType = ANARI_UNKNOWN;
   if(current.color.get(ANARI_DATA_TYPE, &colorType)) {
      color.resize(elements*anari::sizeOf(colorType));
   }
   if(current.depth.get(ANARI_DATA_TYPE, &depthType)) {
      depth.resize(elements); // only FLOAT32 is valid
   }
}
int Object<Frame>::getProperty(
   const char *propname, ANARIDataType type,
   void *mem, uint64_t size, ANARIWaitMask mask) {
   (void)propname;
   (void)type;
   (void)mem;
   (void)size;
   (void)mask;
   return 0;
}
void* Object<Frame>::mapFrame(const char *channel) {
   if(std::strncmp(channel, "color", 5) == 0) {
      return color.data();
   } else if(std::strncmp(channel, "depth", 5) == 0) {
      return depth.data();
   } else {
      return nullptr;
   }
}
void Object<Frame>::unmapFrame(const char*channel) {
   (void)channel;
}
void Object<Frame>::renderFrame() {

}
void Object<Frame>::discardFrame() {

}
int Object<Frame>::frameReady(ANARIWaitMask mask) {
   (void)mask;
   return 1;
}

$end_namespaces
