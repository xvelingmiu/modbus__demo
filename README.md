# Modbus-TCP 通信练习项目（Python 仿真器 + C 客户端）

学习开源库 [libmodbus](https://github.com/stephane/libmodbus) 的练习项目：
用 Python（pymodbus）搭一个 Modbus-TCP 从站（模拟 PLC），
再分别用 Python 和 C 语言写客户端去读写它的寄存器。

## 文件说明

|文件|作用|
|-|-|
|`slave\_sim.py`|Modbus-TCP 从站仿真器（服务器），监听 127.0.0.1:502|
|`modbus\_demo.py`|Python 客户端（pymodbus），循环读写寄存器|
|`main.c`|C 语言客户端（libmodbus），功能同上|
|`modbus\_client\_demo.sln` 等|C 客户端的 Visual Studio 2022 工程|

## 运行方法

### 1\. 启动仿真器（窗口保持开着）

```
pip install pymodbus
python slave\_sim.py
```

### 2\. 运行客户端（二选一）

* Python 版：`python modbus\_demo.py`
* C 版：用 Visual Studio 2022 打开 `modbus\_client\_demo.sln`，选 Release + x64 编译运行

成功后会看到寄存器值每秒刷新，例如 `loop 0: R0=0, R1=22, R2=33`。

## 环境要求

* Python 3.11 + pymodbus
* Visual Studio 2022（"使用 C++ 的桌面开发"工作负载）
* libmodbus：请到官方仓库 https://github.com/stephane/libmodbus 下载源码自行编译，
编译方法见下文"踩坑记录"第 2 条

## 踩坑记录（本项目最有价值的部分）

1. **libmodbus master 版在 Windows 下 connect 必失败（报 Invalid argument）**
原因：源码里多处 `FD\_SETSIZE` 检查把 Windows 的 socket 句柄（通常是几百的值）
误判为超过 64 的非法描述符。修复：删除 modbus-tcp.c / modbus.c / modbus-rtu.c 中
所有 `>= FD\_SETSIZE` 的检查块（本项目 patch 说明见【在此补充你的补丁文件路径或说明】）。
2. **libmodbus 用 Visual Studio 编译前**，需先在 `src\\win32` 目录运行
`cscript configure.js` 生成 `config.h` 和 `modbus-version.h`，
否则报 "无法打开包括文件 config.h"。编译时需链接 `ws2\_32.lib`，
运行时需把生成的 `modbus.dll` 放到 exe 旁边。
3. **pymodbus 版本兼容**：新版 pymodbus 移除了 `ModbusSlaveContext`
（改名 `ModbusDeviceContext`，参数 `slaves=` 改为 `devices=`）。
旧版写法见 `slave\_sim.py`，新版写法见 `slave\_sim\_新版pymodbus.py`。

## 许可证

* libmodbus 遵循 LGPL-2.1，版权归其作者所有；本仓库不包含其源码，仅记录使用方法。
* 本仓库自有代码采用 MIT 许可证。

