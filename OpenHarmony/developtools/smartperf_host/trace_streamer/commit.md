## 关于提交代码的说明
为了方便代码格式化的维护，提交代码前，请使用脚本对代码使用脚本格式化。  
format-code.sh文件会对如下的文件夹进行格式化：
```
src
sdk/demo_sdk
test
```
注意，该脚本不会对所有的#include格式化，因为#include涉及头文件字母表排序，请开发者自行排序。  
如果某一段代码不想被格式化，请在代码前后增加标识如下：  
```
// clang-format off
you code does not want to be formatted
// clang-format on
```
上述标识会控制clang-format自动跳过标识内的代码块。  
目前使用的clang-format是：  
```clang-format version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)```  
如果格式化代码后发现代码格式变化很大，请检查clang-format的版本。
