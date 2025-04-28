<p align="center">
 <h1 align="center">HiL-protocols</h1>
 <p align="center">完全的HIL简单应用层协议，以适应不同传输层接口</p>
</p>

<p align="center">
    <a href="https://github.com/anuraghazra/github-readme-stats/graphs/contributors"><img alt="C" src="https://img.shields.io/badge/C-00599C.svg?logo=c&logoColor=white" /></a>
    <a href="https://mit-license.org"><img alt="Static Badge" src="https://img.shields.io/badge/license%20-%20MIT%20-%20blue" /></a>
</p>

<p align="center">
    <a href="https://wiki.yono233.cn/HiLBase/zh_hans/">文档</a>
</p>

## ✨特性

仅适用于从机部分解析。

# 📚功能计划(按优先级排序)

- [ ] 接收解析的分流
- [ ] **系统指令部分**及**矩阵配置部分**的实现
- [ ] 分帧断续审查
- [ ] 实际的矩阵、控制参数流出(适用AXI总线至矩阵计算模块)
- [ ] 协助排查错误的错误栈机制
- [ ] 协议其他部分的实现
- [ ] 时间窗事务号连续性检查(非必要)

## 接口设计

物理层驱动应有自己的收发buffer，以便于接收异步解析、发送同样是异步。

所以这里的接收port是从物理层buffer中直接取值，发送port是直接推入物理层buffer。

在这里有一个浪费，由于物理层不应有任何解析功能，协议解析类型定义中有大的接收buffer，是避免单例的矩阵对象被错误帧意外变化而无法恢复，所以需要寄存与验证。

另外存在主动上报这个动作，可能存在多线程填写数据的问题，需要做临界段保护。
