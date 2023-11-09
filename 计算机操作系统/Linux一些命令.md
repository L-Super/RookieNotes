
#### 查看端口占用
```shell
netstat -tunlp | grep 8000
```
#### 后台启动程序
```shell
nohup ./MonitorStorageServer &
```
#### 拷贝依赖库到 lib 目录

拷贝 Pmair 应用程序所有的依赖到当前目录下的 lib 子目录下。

```shell
ldd Pamir | awk '{print $3}' | xargs -i cp -L {} ./lib
```

### 查看系统配置

#### 查看 CPU 处理器信息

```sh
lscpu
```


X86：
```
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
Address sizes:       39 bits physical, 48 bits virtual
CPU(s):              16
On-line CPU(s) list: 0-15
Thread(s) per core:  2
Core(s) per socket:  8
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               165
Model name:          Intel(R) Core(TM) i7-10700 CPU @ 2.90GHz
Stepping:            5
CPU MHz:             4700.032
CPU max MHz:         4800.0000
CPU min MHz:         800.0000
BogoMIPS:            5800.00
Virtualization:      VT-x
L1d cache:           256 KiB
L1i cache:           256 KiB
L2 cache:            2 MiB
L3 cache:            16 MiB
NUMA node0 CPU(s):   0-15
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single ssbd ibrs ibpb stibp ibrs_enhanced tpr_shadow vnmi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp pku ospke md_clear flush_l1d arch_capabilities
```
龙芯：
```sh
$ lscpu
Architecture:        loongarch64
Byte Order:          Little Endian
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  1
Core(s) per socket:  4
Socket(s):           1
NUMA node(s):        1
CPU family:          Loongson-64bit
Model name:          Loongson-3A5000
BogoMIPS:            5000.00
L1d cache:           256 KiB
L1i cache:           256 KiB
L2 cache:            1 MiB
L3 cache:            16 MiB
NUMA node0 CPU(s):   0-3
Flags:               cpucfg lam ual fpu lsx lasx complex crypto lvz lbt_x86 lbt_arm lbt_mips
```



#### 查看内存信息

```sh
lsmem
```

#### 查看磁盘信息

```sh
df -h
sudo fdisk -l
```