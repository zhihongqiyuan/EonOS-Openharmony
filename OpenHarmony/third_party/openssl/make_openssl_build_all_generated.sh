#!/usr/bin/env bash
# Copyright (c) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e
pwd # out/target_name
openssl_source_path="$1"
build_all_generated_path="$2"
openssl_selected_platform="$3"
build_with_liblegacy="$4"

# https://github.com/openssl/openssl/blob/master/INSTALL.md#out-of-tree-builds
# OpenSSL can be configured to build in a build directory separate from the source code directory.
# It's done by placing yourself in some other directory and invoking the configuration commands from there.
rm -rf ${build_all_generated_path}
mkdir -p ${build_all_generated_path}
pushd ${build_all_generated_path}
    # https://github.com/openssl/openssl/issues/20112#issuecomment-1400388204
    # no-shared will disable building shared libcrypto and libssl libraries.
    # But the legacy provider would still be built as a shared module.
    # So you would need the legacy shared module present on the installed
    # system and the paths would have to be correct.
    # You can use no-module to make the legacy provider built-in.

    # https://github.com/openssl/openssl/issues/17679#issue-1130060263
    # Is there a way to build a static version of openssl3, including the "legacy" OSSL_PROVIDER?
    # https://github.com/openssl/openssl/issues/17679#issuecomment-1034949099
    # Configure with no-shared no-module. The legacy provider is then part of libcrypto.a.
    # You still need to "load" it via OSSL_PROVIDER_load() - but no .so file is required in that case.

    # https://github.com/openssl/openssl/issues/19368#issuecomment-1274558844
    # no-shared affects the building of libcrypto*.dll and libssl*.dll,
    # not dynamically loadable modules (which are governed by the configuration option no-module / enable-module,
    # which is enabled by default).
    if [ "$build_with_liblegacy" = "false" ]; then
        echo "openssl disable liblegacy"
        configure_cmd="${openssl_source_path}/Configure ${openssl_selected_platform} no-filenames"
    else
        echo "openssl enable liblegacy"
        configure_cmd="${openssl_source_path}/Configure ${openssl_selected_platform} no-shared no-module no-filenames"
    fi
    echo $configure_cmd
    $configure_cmd
    make build_all_generated -j256 >/dev/null 2>&1
popd
