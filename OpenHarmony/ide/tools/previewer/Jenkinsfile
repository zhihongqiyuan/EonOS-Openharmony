/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
 @Library('codeflow') _
node("mesos") {
    stage("init") {
        cloudBuildConfig {
            group_id = "g38b174f570b34dbdb562635ad22ae5c5"
            service_id = "673c67902543447fbfd984ef5538b6db"
            project_id = "02e81629db4242ac88bf77ea66999fd5"
            project_name = "Simulator"
            gate_name = "C_ACE_Simulator"
        }
    }

    cloudBuild {
        jobs = [
            "cmetrics": {
                cloudDragonGate()
            },
            "codingstylecheck": {
                cloudDragonGate()
            },
            "FossBot": {
                cloudDragonGate()
            },
            "codedex": {
                cloudDragonGate()
            },
            "compile": {
                cloudDragonGate()
            },
        ]
    }
}

