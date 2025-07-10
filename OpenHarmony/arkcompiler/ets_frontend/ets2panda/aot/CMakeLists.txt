# Copyright (c) 2021-2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(ES2PANDA_AOT_SRC
  main.cpp
)

panda_add_executable(es2panda ${ES2PANDA_AOT_SRC})
panda_target_link_libraries(es2panda es2panda-public es2panda-lib arkassembler arkbytecodeopt)
panda_target_include_directories(es2panda PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
panda_target_include_directories(es2panda PRIVATE ${OUTPUT_DIR})
panda_target_include_directories(es2panda PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

if (PANDA_FUZZILLI)
    panda_target_compile_options(es2panda
        PRIVATE -fPIC
    )
endif()

panda_target_compile_options(es2panda
  PRIVATE -Werror=shadow
)

panda_add_sanitizers(TARGET es2panda SANITIZERS ${PANDA_SANITIZERS_LIST})
