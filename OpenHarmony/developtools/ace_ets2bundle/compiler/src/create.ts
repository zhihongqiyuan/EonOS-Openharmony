/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

const fs = require('fs');
const program = require('commander');

program.parse(process.argv);
let name:string = 'HelloAce';
let appID:string = 'ace.helloworld';
let appName:string = 'HelloAce';
if (program.args && program.args[0]) {
  name = program.args[0];
  appID = program.args[0];
  appName = program.args[0];
}

const regPath: RegExp = /[`~!@#$%^&*()_+<>?:"{},./;'[\]]/im;

/*
 * Create sample project and files.
 * @param dist {String}
 */
function createProject(dist: string) {
  const dist_ = dist.trim().split('/');
  if (dist_.length > 1 || regPath.test(dist)) {
    return console.error(
      'ERROR: The project name cannot be a path nor contain any special symbol.\n' +
      "NOTE: To create the template project, run 'npm run create' in the root directory.\n" +
      "NOTE: To customize the project name, run 'npm run create <projectName>'.");
  }
  const appPath:string = dist + '/app.ets';
  const manifestPath:string = dist + '/manifest.json';
  const indexPath:string = dist + '/pages/index.ets';

  const app:string = `export default {
    onCreate() {
        console.info('Application onCreate')
    },
    onDestroy() {
        console.info('Application onDestroy')
    },
}`;

  const manifest:string = `{
  "appID": "com.example.` + appID + `",
  "appName": "` + appName + `",
  "versionName": "1.0.0",
  "versionCode": 1,
  "minPlatformVersion": "1.0.1",
  "pages": [
    "pages/index"
  ],
  "window": {
    "designWidth": 750,
    "autoDesignWidth": false
  }
}`;

  const index:string = `@Entry
@Component
struct MyComponent {
  private value1: string = "hello world 1";
  private value2: string = "hello world 2";
  private value3: string = "hello world 3";

  build() {
    Column() {
      Text(this.value1);
      Text(this.value2);
      Text(this.value3);
    }
  }
}`;

  fs.mkdir(dist + '/pages', { recursive: true }, (err) => {
    if (err) {
      return console.error('ERROR: Failed to create project directory.');
    }
    fs.writeFile(appPath, app, (err) => {
      if (err) {
        return console.error('ERROR: Failed to write app.ets.');
      }
      return undefined;
    });
    fs.writeFile(manifestPath, manifest, (err) => {
      if (err) {
        return console.error('ERROR: Failed to write manifest.json.');
      }
      return undefined;
    });
    fs.writeFile(indexPath, index, (err) => {
      if (err) {
        return console.error('ERROR: Failed to write index.ets.');
      }
      return undefined;
    });
    return undefined;
  });
  return undefined;
}

createProject(name);
