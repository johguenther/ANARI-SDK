// Copyright 2023 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <anari/anari_cpp.hpp>
#include <vector>

namespace remote {

struct Frame
{
  enum State
  {
    Unmapped,
    Render,
    Ready,
    Mapped,
  };

  void resizeColor(uint32_t width, uint32_t height, ANARIDataType type);
  void resizeDepth(uint32_t width, uint32_t height, ANARIDataType type);

  State state{Unmapped};

  uint32_t size[2] = {1, 1};
  ANARIDataType colorType = ANARI_UNKNOWN, depthType = ANARI_UNKNOWN;

  std::vector<uint8_t> color;
  std::vector<uint8_t> depth;
};

} // namespace remote
