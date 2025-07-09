#build ninja

#获取代码
git clone https://gitee.com/openharmony/third_party_ninja.git

#通过python脚本构建生成二进制文件
cd third_party_ninja
./configure.py --bootstrap

#通过CMake编译生成二进制文件
cd third_party_ninja
cmake -Bbuild-cmake -H.
cmake --build build-cmake
