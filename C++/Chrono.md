Chrono程序库的设计，是希望能够处理“timer和clock在不同系统中可能不同”的事实，同时也是为了强化时间精准度。为避免像POSIX的time程序库那样每十年就引入一个新的时间类型（time type），C++标准库的目标是提供一个精度中立（precision-neutral）概念，把duration（时间段）和timepoint（时间点）从特定clock（时钟）区分开来。最终结果就是chrono程序库核心由以下类型或概念组成，形成一个抽象层，用来叙述及处理timepoint和duration：

+ 所谓duration（时间段）指的是在某时间单位上的一个明确的tick（片刻数）。

  举个例子，“3分钟”时间段意指“3个一分钟”。再举个例子，“42毫秒”或“86 400秒”。这样的概念也允许你指定“1.5个1/3秒”，其中1.5是片刻数而“1/3秒”是时间单位。

+ 所谓timepoint （时间点）的意思是一个duration 和一个epoch （起始点）的组合。

  典型例子是“2000 年新年午夜”时间点，它被描述为“自 1970 年 1 月 1日开始的第1 262 300 400秒”

  1970年1月1日是UNIX和POSIX的system clock（系统时钟）起始点。

+ Timepoint的概念其实是以某个clock（时钟）为参数。Clock是个对象，定义timepoint的epoch（起点；重要纪元）。不同的clock有着不同的epoch。一般而言，当操作函数处理多个timepoint，例如处理两个timepoint之间的差距时，必须采用相同的epoch/clock。任何clock都提供便捷函数用来产出现下（当前）的timepoint。
![](../images/Pasted%20image%2020220901105717.png)
