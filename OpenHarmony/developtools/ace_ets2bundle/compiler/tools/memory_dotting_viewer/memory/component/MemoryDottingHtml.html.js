/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
export const memoryDottingHtml = `<style>
        :host{
          padding: 10px 10px;
          display: flex;
          flex-direction: column;
        }
        #memory-tool-tip {
           position: absolute;
           visibility: hidden;
           background-color: white;
           border: 1px solid black;
           padding: 5px;
           z-index: 1000;
        }
        </style>
        <div width="1240" height="100%">
        <canvas id="canvas" width="1140" height="600"></canvas>
        <div id="memory-tool-tip"></div>
        </div>
        `;
