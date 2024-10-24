// Copyright 2022 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "array/Array.h"

namespace helide {

struct Array1DMemoryDescriptor : public ArrayMemoryDescriptor
{
  uint64_t numItems{0};
};

bool isCompact(const Array1DMemoryDescriptor &d);

struct Array1D : public Array
{
  Array1D(HelideGlobalState *state, const Array1DMemoryDescriptor &d);

  void commit() override;

  size_t totalSize() const override;
  size_t totalCapacity() const override;

  void *begin() const;
  void *end() const;

  template <typename T>
  T *beginAs() const;
  template <typename T>
  T *endAs() const;

  size_t size() const;

  float4 readAsAttributeValue(
      int32_t i, WrapMode wrap = WrapMode::DEFAULT) const;
  template <typename T>
  T valueAtLinear(float in) const; // 'in' must be clamped to [0, 1]
  template <typename T>
  T valueAtClosest(float in) const; // 'in' must be clamped to [0, 1]

  void privatize() override;

 private:
  size_t m_capacity{0};
  size_t m_begin{0};
  size_t m_end{0};
};

float4 readAttributeValue(const Array1D *arr, uint32_t i);

// Inlined definitions ////////////////////////////////////////////////////////

template <typename T>
inline T *Array1D::beginAs() const
{
  if (anari::ANARITypeFor<T>::value != elementType())
    throw std::runtime_error("incorrect element type queried for array");

  return (T *)data() + m_begin;
}

template <typename T>
inline T *Array1D::endAs() const
{
  if (anari::ANARITypeFor<T>::value != elementType())
    throw std::runtime_error("incorrect element type queried for array");

  return (T *)data() + m_end;
}

template <typename T>
inline T Array1D::valueAtLinear(float in) const
{
  const T *data = dataAs<T>();
  const auto i = getInterpolant(in, size(), false);
  return linalg::lerp(data[i.lower], data[i.upper], i.frac);
}

template <typename T>
inline T Array1D::valueAtClosest(float in) const
{
  const T *data = dataAs<T>();
  const auto i = getInterpolant(in, size(), false);
  return i.frac <= 0.5f ? data[i.lower] : data[i.upper];
}

} // namespace helide

HELIDE_ANARI_TYPEFOR_SPECIALIZATION(helide::Array1D *, ANARI_ARRAY1D);
