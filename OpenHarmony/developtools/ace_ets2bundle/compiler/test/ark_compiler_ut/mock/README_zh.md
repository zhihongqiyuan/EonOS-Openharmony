# Mock

#### 介绍

提供上游(IDE)触发编译构建时的输入rollup对象、类私有方法导出的Mock能力，供UT测试使用。

#### 软件架构

* mock/class_mock: 类私有方法Export Mock源码
* mock/rollup_mock: DevEco上游rollup环节输入 Mock源码

#### 快速开始

ets-loader UT测试采用mocha架构实现，在此基础上介绍Rollup Mock、类私用方法Mock使用方法。

##### Rollup Mock使用

```
import RollUpMock from './mock/rollup_mock/rollup_plugin_mock';

mocha.beforeEach(function () {
	this.rollup = new RollUpMock();
});

mocha.afterEach(() => {
	delete this.rollup;
});

mocha.it('1-1: build debug, getBuildModeInLowerCase', function () {
	this.rollup.build();
	let buildMode = getBuildModeInLowerCase(this.rollup.share.projectConfig);
	expect(buildMode == 'debug').to.be.true;
});
```
如上，使用rollup数据前，请先在beforeEach/before时机创建RollupMock对象(默认为debug模式，请根据实际场景选择使用beforeEach或者before)，调用build接口模拟IDE build Rollup初始化过程。同理preview接口模拟IDE preview Rollup初始化过程，hotreload接口模拟IDE hot reload Rollup初始化过程。

##### 类私用方法Mock使用

```
import { ModuleHotfixMode } from '../../../../lib/fast_build/ark_compiler/module/module_hotfix_mode';

class ModuleHotfixModeMock extends ModuleHotfixMode {
  generateEs2AbcCmdForHotfixMock() {
    this.generateEs2AbcCmdForHotfix();
  }
}

export default ModuleHotfixModeMock;
```

在测试私有方法前，请先参照上述案例追加类私有方法Mock实现。
```
import RollUpMock from '../mock/rollup_mock/rollup_plugin_mock';
import ModuleHotfixModeMock from '../mock/class_mock/module_hotfix_mode_mock';

mocha.beforeEach(function () {
	this.rollup = new RollUpMock();
});

mocha.afterEach(() => {
	delete this.rollup;
});

mocha.it('1-1: build debug, generateEs2AbcCmdForHotfix', function () {
	this.rollup.build();
	let hotFixMode = new ModuleHotfixModeMock(this.rollup);
	hotFixMode.generateEs2AbcCmdForHotfixMock();
});
```
如上，调用Mock提供的接口，间接测试类私有方法。
