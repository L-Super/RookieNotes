`target_link_libraries()` 的可见性（PRIVATE/PUBLIC/INTERFACE）对依赖传递有重要影响。

- PRIVATE：依赖仅用于目标自身的实现，不会传递给依赖该目标的其他目标
- PUBLIC：依赖既用于目标自身，也会传递给依赖该目标的其他目标
- INTERFACE：依赖仅传递给依赖该目标的其他目标，自身不使用