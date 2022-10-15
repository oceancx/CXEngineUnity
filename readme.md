# 简介：

CXEngine是一个方便大家使用lua+imgui做游戏的游戏引擎，基本介绍参考：https://github.com/oceancx/CXEngine#readme。本项目为CXEngine的Unity移植版，目前还在开发中。仅仅实现了人物可以在地图上跑的功能。

# 编译运行（只尝试过windows）：

1. git clone --recursive https://github.com/oceancx/CXEngineUnity

2. 构建NativeProject工程。用cmake.exe打开CXEngineUnity/CMakeLists.txt，配置构建的binaries目录为CXEngineUnity/build, vs版本请选择Visual Studio 15 2017 之后的64位版本，之后全部采用默认配置，点击生成，直到vs工程生成。然后用IDE打开工程，构建CXEngine目标，这个构建完成后会自动拷贝CXEngine.dll, lua.dll到CXEngineUnity/UnityProject/Assets/Plugins目录下。

3. 把data目录拷贝在CXEngineUnity下。

4. 用Unity2021.2.3f1（未来会支持最新版Unity）打开CXEngineUnity/UnityProject目录。顺利的话，Unity可以正常打开项目。如果遇到报错，点击SLua->All->Make, SLua->Custom->Make。顺利的话，点击play，游戏就能跑起来。

有问题可以到Q群提问：983630090