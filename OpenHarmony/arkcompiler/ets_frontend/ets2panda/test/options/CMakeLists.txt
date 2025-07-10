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

if(CMAKE_CROSSCOMPILING)
    return()
endif()

add_custom_target(es2panda_tests_options)
add_dependencies(es2panda_tests es2panda_tests_options)

function(check_option_help_otput target_name INPUT_ARGS OUTPUT_HELP_LINE)
    separate_arguments(INPUT_ARGS)
    add_custom_target(es2panda_check_opts_${target_name}
        COMMENT "es2panda: checking option ${INPUT_ARGS}"
        COMMAND es2panda ${INPUT_ARGS} 2> ${CMAKE_BINARY_DIR}/es2panda_check_opts_${target_name}.out || true
        COMMAND grep -q ${OUTPUT_HELP_LINE} ${CMAKE_BINARY_DIR}/es2panda_check_opts_${target_name}.out
        DEPENDS es2panda
    )

    add_dependencies(es2panda_tests_options es2panda_check_opts_${target_name})
endfunction()

check_option_help_otput(bco_opt "--help" "bco-optimizer:")
check_option_help_otput(bco_opt_help "--bco-optimizer --help" "bytecode-opt-peepholes:")
check_option_help_otput(comp_opt "--help" "bco-compiler:")
check_option_help_otput(comp_opt_help "--bco-compiler --help" "compiler-disasm-dump:")
check_option_help_otput(opt_level "--opt-level 7" "out of range parameter value \\\"7\\\"")
check_option_help_otput(version "--version" "Es2panda Version")
check_option_help_otput(version_build "--version" "Build date:")
check_option_help_otput(version_hash "--version" "Last commit hash")
check_option_help_otput(list_phases "--list-phases" "Available phases:")
