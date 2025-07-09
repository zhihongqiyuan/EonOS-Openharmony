# napi_generator-1.3（2022-01-30）

## 版本概述<a name="section249611124916"></a>

发布napi_generator-1.3。

## 版本特性<a name="section249611124917"></a>
**表 1**  已支持特性

[当前已知不支持的推荐方案](https://gitee.com/openharmony/napi_generator/blob/master/docs/SOLUTION.md)

<a name="table143385853320"></a>

<table><thead align="center"><tr id="row53375863312"><th class="cellrowborder" valign="top" width="18%" id="mcps1.2.3.1.1"><p id="p20331858193317"><a name="p20331858193317"></a><a name="p20331858193317"></a><ul>类别</ul></p>
</th>
<th class="cellrowborder" valign="top" width="45%" id="mcps1.2.3.1.2"><p id="p1133115820331"><a name="p1133115820331"></a><a name="p1133115820331"></a><ul>支持特性</ul></p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.3.1.3"><p id="p1133115820331"><a name="p1133115820333"></a><a name="p1133115820333"></a><ul>用例</ul></p>
</th>
<th class="cellrowborder" valign="top" width="12%" id="mcps1.2.3.1.4"><p id="p1133115820332"><a name="p1133115820332"></a><a name="p1133115820332"></a><ul>版本号</ul></p>
</th>
</tr>
</thead>
<tbody><tr id="row333115812331"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p2142111345714"><a name="p2142111345714"></a><a name="p2142111345714"></a><ul>变量/返回值</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul9264132010"></a><a name="ul9264132010"></a><ul id="ul9264132010"><li>支持ts语言中string、number、boolean基础类型的变量/函数入参自动转换为C++类型</li><li>支持ts语言中interface及匿名interface复合类型的变量/函数入参自动转换为C++类型</li><li>支持ts语言中enum复合类型变量/函数入参自动转换为C++类型 [注：enum基本类型支持string、number类型]</li><li>支持ts语言中string、number、boolean、enum、interface、map数组类型的变量/函数入参自动转换为C++类型，且数组书写方式可以为array<>或[]两种</li><li>支持ts语言中string、number、boolean、array、map、interface的map类型的变量/函数入参自动转换为C++类型，且map书写方式可以为{[key:string]:any}或Map<>两种 [注：map的key仅支持string类型，value支持string/number/boolean/map/array类型]</li>  <li>支持ts接口文件中namespace域的any类型变量或函数参数转换为对应C++类型变量 [注：any的范围为string/number/boolean/map(string/number/boolean/array类型map)/array类型(string/number/boolean/map类型array)]</li><li>支持ts接口文件中namespace域的object变量 转换为对应C++变量[注：object_value类型为string/number/boolean/object]</li><li>支持ts接口文件中namespace域的可选变量 转换为对应的C++变量</li><li>支持ts接口文件中namespace域的多类型合并成新类型的变量转换为对应C++类型变量 [注：类型范围为：number/string/boolean]</li></ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418001">
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_string">string用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_number">number用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_bool">boolean用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_interface">interface用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_interface_no_name">匿名interface用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_enum">enum用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_enum_interface">enum_interface用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_enum_js">enum_JS用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_[]">[]数组用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_array">array数组用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_map">map用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_array_map">array map用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_any">any用例</a>、<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_any[]">any[]用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_object">object用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_optional">可选变量用例</a>
</li>
<li><a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_union">union用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345715"><a name="p2142111345715"></a><a name="p2142111345715"></a><ul>V1.2</ul></p>
</td>
</tr>
<tr id="row334175803317"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p382391145710"><a name="p382391145710"></a><a name="p382391145710"></a><ul>函数类型</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul334485413318"></a><a name="ul334485413318"></a><ul id="ul334485413318"><li>支持ts语言中返回值、callback类型的同步函数自动转换为C++类型 [注：同步callback类型特指模板中定义的export interface Callback<T> {
    (data: T): void;
}]</li><li>支持ts语言中callback、promise类型的异步函数自动转换为C++类型 [注：异步callback类型特指模板中定义的export interface AsyncCallback<T> {
    (err: BusinessError, data: T): void;
}]</li><li>支持ts接口文件中namespace域的static函数自动转换为对应C++的函数</li><li>支持ts接口文件中namespace域的以$开头的函数自动转换为对应C++的自动函数</li><li>支持ts接口文件中namespace域的on注册、off去注册函数 [注：on/off事件类型支持string类型泛值或string类型固定值，回调函数支持AsyncCallback、Callback模板]</li><li>支持ts接口文件中namespace域的箭头函数转换为对应C++的函数</li></ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418002">
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_callback">callback用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_promise">promise用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_static">static用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_$">$用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_on">on/off用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_class">arrow func用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345716"><a name="p2142111345716"></a><a name="p2142111345716"></a><ul>V1.2</ul></p>
</td>
</tr>
<tr id="row834358143319"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p1818191195713"><a name="p1818191195713"></a><a name="p1818191195713"></a><ul>interface</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul4367144411512"></a><a name="ul4367144411512"></a><ul id="ul4367144411512"><li>支持ts语言中interface域的变量、函数自动转换为C++类型</li><li>支持ts接口文件中interface继承class方式的声明 对应的C++实现</li><li>支持ts接口文件中interface继承interface方式的声明 对应的C++实现</li></ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418003">
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_interface">interface用例</a>
</li>
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_extends">extends用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345717"><a name="p2142111345717"></a><a name="p2142111345717"></a><ul>V1.2</ul></p>
</td>
</tr>
<tr id="row119944512385"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p919862210573"><a name="p919862210573"></a><a name="p919862210573"></a><ul>namespace</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul12374158862"></a><a name="ul12374158862"></a><ul id="ul12374158862"><li>支持ts语言中namespace域的变量、函数、interface自动转换为C++类型</li><li>支持ts接口文件中namespace域class转换为C++的class</li><li>支持ts接口文件中namespace域class继承转换为C++中继承的class</li> </ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418004">
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_namespace">namespace用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345718"><a name="p2142111345718"></a><a name="p2142111345718"></a><ul>V1.2</ul></p>
</td>
</tr>
<tr id="row18711154213388"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 " rowspan="2"><p id="p111921822185713"><a name="p111921822185713"></a><a name="p111921822185713"></a><ul>文件</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul94024441879"></a><a name="ul94024441879"></a><ul id="ul94024441879"><li>支持ts文件中namespace数据类型转换为C++类型</li><li>支持ts接口文件名格式如下ohos.A.B.C.d.ts的文件转换</li><li>支持命令行方式转换时参数为多个文件路径，相互之间用英文逗号分开</li><li>支持命令行方式转换时指定文件夹路径方式转换</li><li>支持ts文件中import自定义文件</li></ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
<ul id="ult940244418005">
<li>
<a href="https://gitee.com/openharmony/napi_generator/tree/master/test/storytest/test_import">import用例</a>
</li>
</ul>
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345718"><a name="p2142111345718"></a><a name="p2142111345718"></a><ul>V1.2</ul></p>
</td>
</tr>
<tr id="row18711154213389">
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul94024441880"></a><a name="ul94024441880"></a><ul id="ul94024441880"><li>DevEco Studio 上的IntelliJ插件支持如下文件名格式文件ohos.A.B.C.d.ts</li><li>VSCode插件支持如下文件名格式文件ohos.A.B.C.d.ts</li><li>命令行方式支持同时转换多个文件，相互之间用英文逗号分开</li><li>命令行方式支持文件夹转换</li></ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
插件界面适配暂无用例
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345719"><a name="p2142111345719"></a><a name="p2142111345719"></a><ul>V1.3</ul></p>
</td>
</tr>
<tr id="row119944512386"><td class="cellrowborder" valign="top" width="18%" headers="mcps1.2.3.1.1 "><p id="p919862210574"><a name="p919862210574"></a><a name="p919862210574"></a><ul>可靠性</ul></p>
</td>
<td class="cellrowborder" valign="top" width="45%" headers="mcps1.2.3.1.2 "><a name="ul12374158863"></a><a name="ul12374158863"></a><ul id="ul12374158863"><li>整改生成代码规范</li> </ul>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.3.1.3 ">
代码规范暂无用例
</td>
<td class="cellrowborder" valign="top" width="12%" headers="mcps1.2.3.1.4 "><p id="p2142111345720"><a name="p2142111345720"></a><a name="p2142111345720"></a><ul>V1.3</ul></p>
</td>
</tr>
</tbody>
</table>

**表 2** 

[待开发特性](https://gitee.com/openharmony/napi_generator/blob/master/docs/ROADMAP_ZH.md)

