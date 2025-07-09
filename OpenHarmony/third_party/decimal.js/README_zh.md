# decimal.js

API官方文档：[https://mikemcl.github.io/decimal.js/](https://mikemcl.github.io/decimal.js/)
版本：10.4.3

decimal.js是JavaScript的一个高精度数学库，它具有以下特性

- 可以进行128位的高精度计算和数据表示

- 简单且功能齐全的API

- 复用了许多JavaScript的 `Number.prototype` 和 `Math` 对象的方法

- 可以转换为二进制、八进制和十六进制值

- 比Java的BigDecimal的JavaScript版本更快、更小，也更容易使用

- 广泛的平台兼容性：仅使用JavaScript 1.5（ECMAScript 3）特性

- 添加了三角函数，非整数幂等数学计算方法

  

OpenHarmony上引入decimal.js主要用于提供高精度浮点运算能力。

## 主要目录结构

```
doc/             #文档
test/            #测试代码
decimal.mjs      #decimal源码
LICENSE          #版权补充说明
README.md        #软件说明
```

## OpenHarmony如何集成decimal.js

decimal.js被引入在OpenHarmony的third_party目录下，通过OpenHarmony中部件依赖的方式进行编译。

1. 主干代码下载

   ```
   repo init -u https://gitee.com/openharmony/manifest.git -b master --no-repo-verify
   repo sync -c
   repo forall -c 'git lfs pull'
   ```

2. 在需要使用该库的模块中添加依赖

   ```
   deps = [ "//third_party/decimal.js:decimal" ]
   ```

3. 预处理

   ```
   ./build/prebuilts_download.sh
   ```

4. 编译

   ```
   ./build.sh --product-name rk3568 --ccache
   ```

   编译生成的文件对应路径：`out/rk3568/thirdparty/decimal.js/libdecimal.z.so`。

5. 运行

   ```ts
   // Decimal功能需要在ArkTs中使用
   // Index.ets
   import { Decimal } from '@kit.ArkTS';
   @Entry
   @Component
   struct Index {
     build() {
       Row() {
         Column() {
           Text("Test")
             .fontSize(50)
             .fontWeight(FontWeight.Bold)
             .onClick(() => {
               let a0 : Decimal = new Decimal(1.2345678912345678)  // 可以使用Decimal表示数值
               console.log("test Decimal :" + a0.toString());      // 可以通过toString获取Decimal表示的数值
                                                                   // '1.2345678912345678'
               Decimal.set({ precision : 10 })                     // 可以通过Decimal.set设置精度等"global"配置
               let a1 : Decimal = a0.add(0.5)                      // 进行加法操作
               console.log("test Decimal set:" + a1.toString());   // 当前全局精度为10，结果为'1.734567891'

               Decimal.set({ defaults : true })                    // 设置回默认值配置
               let dCos = Decimal.cos(0.5)                         // 可以使用Decimal中的三角函数等数学方法输出高精度浮点数
               console.log("test Decimal cos:" + dCos.toString()); // '0.87758256189037271612'
               console.log("test Math cos:" + Math.cos(0.5));      // '0.8775825618903728'

               let a2 = Decimal.add(0.1, 0.2)                      // 此外, Decimal可以解决一些低精度计算导致的bug
               console.log("test Decimal add:" + a2.toString());   // '0.3'
               console.log("test Decimal add:" + (0.1 + 0.2));     // '0.30000000000000004'
             })
         }
         .width('100%')
       }
       .height('100%')
     }
   }
   
   ```


## 许可证

本项目遵从[LICENSE](https://gitee.com/openharmony-sig/third_party_decimal.js/blob/master/LICENCE.md)中所描述的许可证。