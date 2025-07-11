/**
 * Copyright 2023 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "backend/common/graph_kernel/adapter/symbol_engine_builder.h"
#include <memory>
#include "include/common/utils/anfalgo.h"
#include "backend/common/graph_kernel/symbol_engine/multi_symbol_engine.h"

namespace mindspore::graphkernel {
bool SymbolEngineBuilder::Run(const FuncGraphPtr &func_graph) {
  if (!common::AnfAlgo::IsDynamicGraph(func_graph)) {
    return false;
  }
  if (multi_engine_) {
    symshape::MultiSymbolEngine::Build(func_graph);
  } else {
    symshape::SymbolEngineImpl::Build(func_graph);
  }
  return true;
}
}  // namespace mindspore::graphkernel
