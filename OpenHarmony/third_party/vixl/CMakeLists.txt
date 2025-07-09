# Copyright (c) 2021-2022 Huawei Device Co., Ltd.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#   * Neither the name of ARM Limited nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.10)

project(vixl)

set(VIXL_SOURCES
    src/code-buffer-vixl.cc
    src/compiler-intrinsics-vixl.cc
    src/cpu-features.cc
    src/utils-vixl.cc )

set(VIXL_DIR
    .
    src/ )

if (PANDA_TARGET_MACOS)
set(VIXL_FLAGS -DVIXL_CODE_BUFFER_MALLOC -DPANDA_BUILD)
else()
set(VIXL_FLAGS -DVIXL_CODE_BUFFER_MMAP -DPANDA_BUILD)
endif()

set(VIXL_AARCH32_SOURCES
    src/aarch32/assembler-aarch32.cc
    src/aarch32/disasm-aarch32.cc
    src/aarch32/location-aarch32.cc
    src/aarch32/operands-aarch32.cc
    src/aarch32/constants-aarch32.cc
    src/aarch32/instructions-aarch32.cc
    src/aarch32/macro-assembler-aarch32.cc )

set(VIXL_AARCH32_DIR src/aarch32/)

set(VIXL_AARCH64_SOURCES
    src/aarch64/assembler-aarch64.cc
    src/aarch64/operands-aarch64.cc
    src/aarch64/cpu-aarch64.cc
    src/aarch64/instructions-aarch64.cc
    src/aarch64/macro-assembler-aarch64.cc )

if (NOT PANDA_MINIMAL_VIXL)
    list(APPEND VIXL_AARCH64_SOURCES
        #src/aarch64/assembler-sve-aarch64.cc
        src/aarch64/decoder-aarch64.cc
        src/aarch64/disasm-aarch64.cc
        src/aarch64/logic-aarch64.cc
        src/aarch64/pointer-auth-aarch64.cc
        src/aarch64/cpu-features-auditor-aarch64.cc
        #src/aarch64/macro-assembler-sve-aarch64.cc
        src/aarch64/debugger-aarch64.cc
        src/aarch64/simulator-aarch64.cc )
endif()

set(VIXL_AARCH64_DIR src/aarch64/)

if (NOT(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo))
    list(APPEND VIXL_FLAGS -DVIXL_DEBUG)
endif()

# !TODO Add test suite to check with PANDA_COMPILER_TARGET_AARCH32
if ((PANDA_TARGET_ARM32) OR (PANDA_TARGET_AMD64))
    list(APPEND VIXL_FLAGS -DVIXL_INCLUDE_TARGET_A32)
    list(APPEND VIXL_SOURCES ${VIXL_AARCH32_SOURCES})
    list(APPEND VIXL_DIR ${VIXL_AARCH32_DIR})
endif()

# !TODO Add  test suite to check with PANDA_COMPILER_TARGET_AARCH64
if ((PANDA_TARGET_ARM64) OR (PANDA_TARGET_AMD64))
    list(APPEND VIXL_FLAGS -DVIXL_INCLUDE_SIMULATOR_AARCH64 -DVIXL_INCLUDE_TARGET_A64)
    list(APPEND VIXL_SOURCES ${VIXL_AARCH64_SOURCES})
    list(APPEND VIXL_DIR ${VIXL_AARCH64_DIR})
endif()

include_directories(${VIXL_DIR})

add_library(vixl STATIC ${VIXL_SOURCES})
target_link_libraries(vixl arkbase)

target_compile_options(vixl PUBLIC ${VIXL_FLAGS})
target_compile_options(vixl PRIVATE -Wno-shadow -Wno-deprecated-declarations)

target_compile_definitions(vixl PUBLIC "VIXL_USE_PANDA_ALLOC")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14.0.0)
    target_compile_options(vixl PRIVATE -Wno-bitwise-instead-of-logical)
endif()

set_property(TARGET vixl PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET vixl PROPERTY EXCLUDE_FROM_ALL TRUE)

target_include_directories(vixl
    SYSTEM INTERFACE src/
)

panda_add_sanitizers(TARGET vixl SANITIZERS ${PANDA_SANITIZERS_LIST})
