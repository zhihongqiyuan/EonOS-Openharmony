# napi_generator-1.4

## 版本概述<a name="section249611124916"></a>

发布napi_generator-1.4。

## 版本特性<a name="section249611124917"></a>
**表 1**  已支持特性

[当前已知不支持的推荐方案](https://gitee.com/openharmony/napi_generator/blob/master/docs/SOLUTION.md)

<a name="table143385853321"></a>

<table><thead align="left"><tr id="row53375863312"><th class="cellrowborder" valign="top" width="18%" id="mcps1.2.3.1.1"><p id="p20331858193317"><a name="p20331858193317"></a><a name="p20331858193317"></a><ul>类别</ul></p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p1133115820331"><a name="p1133115820331"></a><a name="p1133115820331"></a><ul>支持特性</ul></p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.3.1.3"><p id="p1133115820331"><a name="p1133115820333"></a><a name="p1133115820333"></a><ul>用例</ul></p>
</th>
<th class="cellrowborder" valign="top" width="12%" id="mcps1.2.3.1.4"><p id="p1133115820332"><a name="p1133115820332"></a><a name="p1133115820332"></a><ul>版本号</ul></p>
</th>
</tr>
</thead>
<tbody><tr id="row333115812331"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p2142111345714"><a name="p2142111345714"></a><a name="p2142111345714"></a><ul>变量/返回值</ul></p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><a name="ul9264132010"></a><a name="ul9264132010"></a><ul id="ul9264132010"><li>支持ts语言中type复合类型的变量/函数入参自动转换为C++类型 [注：type类型源范围基本类型(string/number/boolean)/map/array/enum]</li><li>支持interface/type成员变量为基本类型(string/number/boolean)时，成员变量为可选的,如：
interface Test {
    v1: string;
    v2?: boolean;
}</li></ul>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418001">
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_type">type用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345715"><a name="p2142111345715"></a><a name="p2142111345715"></a><ul>V1.4</ul></p>
</td>
</tr>
<tr id="row334175803317"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p382391145710"><a name="p382391145710"></a><a name="p382391145710"></a><ul>函数类型</ul></p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><a name="ul334485413318"></a><a name="ul334485413318"></a><ul id="ul334485413318"><li>支持ts接口文件中namespace域中的registerXXX注册、unRegisterXXX去注册函数 [注：1.使用register标识当前回调只注册，业务决定触发时机，区别当前函数中触发回调场景； 2.当前注册回调支持单个参数，不支持多个参数，回调形式：1) registerCallbackfunc(cb : (wid: number) => string); 2) fun17(tt: Function): string; Function类型按 ()=>void 处理； 3.所有回调方式默认为同步]</li><li>支持ts接口文件中namespace域中的addXXX注册、removeXXX去注册、onXXX定义回调的函数 [注：1.使用add标识当前函数为注册函数，且参数为接口类 2.当前注册回调支持单个参数，不支持多个参数；支持类型写法：addSayHelloListener(listener: NodeISayHelloListener); 3.注册的object回调函数写法不支持箭头函数写法, 只支持onSayHelloStart(info1: SayInfo, info2: string);这样的写法）</li><li>支持on注册类型为固定值,注册回调为箭头函数：如function on(type: 'onEvents', callback: (wid: number) => void): void; </li><li>支持文件中定义多个on注册函数</li><li>支持callback方法为箭头函数：如function fun1(cb: (wid: boolean) => string): string;</li><li>支持callback方法为Function关键字：如function fun2(tt: Function): void;</li><li>支持Promise类型是匿名interface的函数。如：sayHi(from: string): Promise<{result: number, errMsg: string, response: string}>; </li></ul>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418001">
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_on">on/off用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_callback">callback用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345716"><a name="p2142111345716"></a><a name="p2142111345716"></a><ul>V1.4</ul></p>
</td>
</tr>
<tr id="row834358143319"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p1818191195713"><a name="p1818191195713"></a><a name="p1818191195713"></a><ul>interface</ul></p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><a name="ul4367144411512"></a><a name="ul4367144411512"></a><ul id="ul4367144411512"><li>支持class/interface成员方法不声明返回值，默认void</li><li>支持后定义的class或interface被前面类引用</li></ul>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418001">
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_class">class用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_interface">interface用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345717"><a name="p2142111345717"></a><a name="p2142111345717"></a><ul>V1.4</ul></p>
</td>
</tr>
<tr id="row119944512385"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p919862210573"><a name="p919862210573"></a><a name="p919862210573"></a><ul>namespace</ul></p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><a name="ul12374158862"></a><a name="ul12374158862"></a><ul id="ul12374158862"><li>支持enum定义在interface后面</li><li>支持namespace域中的interface/type/class/enum兼容大括号换行</li></ul>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418001">
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_enum">enum用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_interface">interface用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_class">class用例</a>
</li>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_type">type用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345718"><a name="p2142111345718"></a><a name="p2142111345718"></a><ul>V1.4</ul></p>
</td>
</tr>
<tr id="row18711154213388"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 " rowspan="2"><p id="p111921822185713"><a name="p111921822185713"></a><a name="p111921822185713"></a><ul>文件</ul></p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><a name="ul94024441879"></a><a name="ul94024441879"></a><ul id="ul94024441879"><li>支持命令行方式ts接口文件转换时业务代码可配置</li></ul>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.3.1.3 ">
实现一键生成NAPI实现代码，目前仅支持命令行入口，DevEco Studio入口、VScode入口暂不支持
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345718"><a name="p2142111345718"></a><a name="p2142111345718"></a><ul>V1.4</ul></p>
</td>
</tr>
</tbody>
</table>





**表 2** 

[待开发特性](https://gitee.com/openharmony/napi_generator/blob/master/docs/ROADMAP_ZH.md)



