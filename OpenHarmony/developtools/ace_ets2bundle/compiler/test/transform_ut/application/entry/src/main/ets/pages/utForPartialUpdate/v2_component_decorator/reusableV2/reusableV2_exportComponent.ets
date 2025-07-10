/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

/*
 * This test case is used to export components decorated by @Component, @ComponentV2 and @ReusableV2.
 */
@Component
export struct V1Child2 {
  build() {}
}

@ComponentV2
export struct V2Child2 {
  build() {}
}

@ReusableV2
@ComponentV2
export struct ReuseV2Child3 {
  build() {}
}
