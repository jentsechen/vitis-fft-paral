## How to compile
```
make aie
make hls
make xsa
make host
make package
```

## Analysis of Parallelism
| 1-parallelism | 2-parallelism |
| :---: | :---: |
| Time requirement: 90265 us | Time requirement: 70527 us |
| ![](./imp_result/paral_analysis/AIE_util_1_paral.png) | ![](./imp_result/paral_analysis/AIE_util_2_paral.png) | 
| ![](./imp_result/paral_analysis/graph_1_paral.png) | ![](./imp_result/paral_analysis/graph_2_paral.png) | 
| ![](./imp_result/paral_analysis/array_1_paral.png) | ![](./imp_result/paral_analysis/array_2_paral.png) | 
| ![](./imp_result/paral_analysis/PL_util_1_paral.png) | ![](./imp_result/paral_analysis/PL_util_2_paral.png) | 
* Time requirement of 1-parallelism: (92307+91215+94674+92912+90864+94735+78237+88278+84813+94617)/10=90265.2 us 
* Time requirement of 2-parallelism: (72680+72702+78955+52469+74924+71306+52863+79045+66018+84310)/10=70527.2 us

## Analysis of Distribution
* `TP_WINDOW_VSIZE`: 1024 x 2

| 2 kernels | 4 kernels |
| :---: | :---: |
| Time requirement: 28910 us | Time requirement: 17839 us |
| ![](./imp_result/dist_analysis/AIE_util_2_kn.png) | ![](./imp_result/dist_analysis/AIE_util_4_kn.png) |
| ![](./imp_result/dist_analysis/graph_2_kn.png) | ![](./imp_result/dist_analysis/graph_4_kn.png) |
| ![](./imp_result/dist_analysis/array_2_kn.png) | ![](./imp_result/dist_analysis/array_4_kn.png) |
* Time requirement of 2 kernels: (30699+27543+30708+30437+29609+29585+30782+27020+27162+25551)/10 = 28909.6 us
* Time requirement of 4 kernels: (18104+18672+16816+18693+17410+16946+16979+17149+18699+18923)/10 = 17839.1 us

| 4 kernels | 8 kernels |
| :---: | :---: |
| Time requirement: 15947 us | Time requirement: 13116 us |
| ![](./imp_result/dist_analysis/aie_util_4_kn_acc.png) | ![](./imp_result/dist_analysis/aie_util_8_kn_acc.png) |
| ![](./imp_result/dist_analysis/graph_4_kn_acc.png) | ![](./imp_result/dist_analysis/graph_8_kn_acc.png) |
| ![](./imp_result/dist_analysis/array_4_kn_acc.png) | ![](./imp_result/dist_analysis/array_8_kn_acc.png) |
| ![](./imp_result/dist_acc_blk_dgm.png) |  |
* Time requirement of 4 kernels: (15844+15811+15907+15795+15852+16384+16066+15930+15916+15967)/10 = 15947.2 us
* Time requirement of 8 kernels: (13118+13186+13100+13131+13126+13126+13145+13089+13073+13067)/10 = 13116.1 us

## Analysis of Mapper/Router
* 4-kernels, 4-parallelism
### Auto-Routing
![](./imp_result/mp_rt_analysis/four_paral_auto.png)
### Routing with Constraint
* 33 min for routing
* `aie_constraints.json`
```json
{
    "GlobalConstraints": {
        "areaGroup": {
            "name": "fft_graph",
            "nodeGroup": [
                "fft_graph.*"
            ],
            "tileGroup": [
                "(22,0):(22,7)",
                "(23,0):(23,7)",
                "(24,0):(24,7)",
                "(25,0):(25,7)",
                "(26,0):(26,7)",
                "(27,0):(27,7)",
                "(28,0):(28,7)",
                "(29,0):(29,7)"
            ]
        }
    }
}
```
![](./imp_result/mp_rt_analysis/four_paral_with_constraint.png)