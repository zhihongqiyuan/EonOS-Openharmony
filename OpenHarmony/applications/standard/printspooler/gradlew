#
# Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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
#
function init_npm
{
    npm config set registry https://repo.huaweicloud.com/repository/npm/
    npm config set @ohos:registry https://repo.harmonyos.com/npm/
    npm config set strict_ssl false
    npm config set sslVerify false
}

init_npm

function npm_install {
    cd  $1
    npm install
}

APP_HOME="`pwd - P`"
npm_install "$APP_HOME"
npm_install "$APP_HOME/common"
npm_install "$APP_HOME/feature/ippPrint"
npm_install "$APP_HOME/entry"

cd $APP_HOME
node ./node_modules/@ohos/hvigor/bin/hvigor.js --mode module assembleHap -p debuggable=false