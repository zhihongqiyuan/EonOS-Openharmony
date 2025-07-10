/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import * as fs from 'node:fs';
import * as path from 'node:path';

const TS_EXT = ".ts";
const ETS_EXT = ".ets";
const TSX_EXT = ".tsx";
const STS_EXT = ".sts";
const STSX_EXT = ".stsx";
const D_TS_EXT = '.d.ts';

class Mode {
    static DEFAULT = 1;
    static AUTOFIX = 2;
    static ARKTS2 = 3;
}

const RESULT_EXT = [];
RESULT_EXT[Mode.DEFAULT] = '.json';
RESULT_EXT[Mode.AUTOFIX] = '.autofix.json';
RESULT_EXT[Mode.ARKTS2] = '.arkts2.json';
const DIFF_EXT = '.diff';
const RESULTS_DIR = 'results';
const TEST_ARGS_EXT = '.args.json';

let testDirs = [];

// forces to update all tests regardless of whether there was diff in a test result
let force_update = false;

for (let arg of process.argv.slice(2)) {
    if (arg === '--force')
        force_update = true;
    else
        testDirs.push(arg);
}

const DEFAULT_COPYRIGHT =  [
    "Copyright (c) 2024 Huawei Device Co., Ltd.",
    "Licensed under the Apache License, Version 2.0 (the 'License');",
    "you may not use this file except in compliance with the License.",
    "You may obtain a copy of the License at",
    "",
    "http://www.apache.org/licenses/LICENSE-2.0",
    "",
    "Unless required by applicable law or agreed to in writing, software",
    "distributed under the License is distributed on an 'AS IS' BASIS,",
    "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
    "See the License for the specific language governing permissions and",
    "limitations under the License."
];

function readJsonFile(filePath) {
    try {
        let resultFile = fs.readFileSync(filePath).toString();
        return JSON.parse(resultFile);
    } catch (error) {
        return undefined;
    }
}

function updateTestFile(testDir, testFile) {
    // Temporary solution: rename '.sts' extension to '.ts'
    if (testFile.endsWith(STS_EXT) || testFile.endsWith(STSX_EXT)) {
        testFile = testFile.replace(STS_EXT, TS_EXT);
    }

    let testModes = [Mode.DEFAULT];

    const testArgsFile = path.join(testDir, testFile + TEST_ARGS_EXT);
    if (fs.existsSync(testArgsFile)) {
        const testArgs = readJsonFile(testArgsFile);
        if (testArgs?.mode?.autofix !== undefined) testModes.push(Mode.AUTOFIX);
        if (testArgs?.mode?.arkts2 !== undefined) testModes.push(Mode.ARKTS2);
    }

    for (const mode of testModes) {
        updateTest(testDir, testFile, mode);
    }
}

function updateTest(testDir, testFile, mode) {
    let resultExt = RESULT_EXT[mode];
    let resultFileWithExt = testFile + resultExt;
    let resultFilePath = path.join(testDir, resultFileWithExt);

    // Update test result when:
    // - '.diff' exists
    // - expected '.json' for specified test mode doesn't exist
    // - 'force' option is enabled
    if (fs.existsSync(resultFilePath) && !fs.existsSync(path.join(testDir, RESULTS_DIR, resultFileWithExt + DIFF_EXT)) && !force_update) {
        return;
    }

    let expectedResult = readJsonFile(resultFilePath);

    const copyright = expectedResult?.copyright ?? DEFAULT_COPYRIGHT;

    let actualResult = readJsonFile(path.join(testDir, RESULTS_DIR, resultFileWithExt));
    if (!actualResult || !actualResult.result) {
        console.log(`Failed to update ${resultFileWithExt}: couldn't process ACTUAL result file.`);
        return;
    }

    // Write file with actual test results.
    let newResultJSON = JSON.stringify({ copyright, result: actualResult.result }, null, 4);
    fs.writeFileSync(resultFilePath, newResultJSON);

    console.log(`Updated ${resultFileWithExt}`);
}

for (let testDir of testDirs) {
    if (!fs.existsSync(path.join(testDir, RESULTS_DIR))) continue;

    // Get tests from test directory.
    let testFiles = fs.readdirSync(testDir).filter(x =>
        (x.trimEnd().endsWith(TS_EXT) && !x.trimEnd().endsWith(D_TS_EXT)) ||
        x.trimEnd().endsWith(TSX_EXT) ||
        x.trimEnd().endsWith(ETS_EXT) ||
        x.trimEnd().endsWith(STS_EXT) ||
        x.trimEnd().endsWith(STSX_EXT)
    );

    if (!testFiles) continue;

    // Update result for each test:
    for (let testFile of testFiles) {
        updateTestFile(testDir, testFile);
    }
}