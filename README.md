## How to compile
```
make aie
make hls
make xsa
make host
make package
```

## Simulation Result
| 1-parallelism | 2-parallelism |
| :---: | :---: |
| Time requirement: 90265 us | Time requirement: 70527 us |
| ![](./imp_result/AIE_util_1_paral.png) | ![](./imp_result/AIE_util_2_paral.png) | 
| ![](./imp_result/graph_1_paral.png) | ![](./imp_result/graph_2_paral.png) | 
| ![](./imp_result/array_1_paral.png) | ![](./imp_result/array_2_paral.png) | 
| ![](./imp_result/PL_util_1_paral.png) | ![](./imp_result/PL_util_2_paral.png) | 
* Time requirement of 1-parallelism: (92307+91215+94674+92912+90864+94735+78237+88278+84813+94617)/10=90265.2 us 
* Time requirement of 2-parallelism: (72680+72702+78955+52469+74924+71306+52863+79045+66018+84310)/10=70527.2 us