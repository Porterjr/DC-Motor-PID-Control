# DC-Motor-PID-Control

基于 STM32F103 的直流电机 PID 闭环调速系统，含编码器测速、OLED 显示与按键交互。

![MCU](https://img.shields.io/badge/MCU-STM32F103-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Status](https://img.shields.io/badge/Status-进行中-yellow)

## 硬件清单
| 模块 | 型号 | 说明 |
|------|------|------|
| 主控 | STM32F103C8T6 | 最小系统板 |
| 电机驱动 | TB6612FNG | 双路H桥 |
| 电机 | JGA25-370 6V | 减速比370:1，带霍尔编码器 |
| 编码器 | 霍尔AB相 | 13 PPR（电机轴），四倍频 |
| 显示 | 0.96寸OLED | I2C接口，4行显示 |
| 按键 | 轻触开关×3 | KEY1加速，KEY2减速，KEY3停止 |
| 电源 | 2×18650电池盒 | 7.4V串联，接TB6612 VM |

## 接线图
| STM32引脚 | 接哪里 | 功能 |
|-----------|--------|------|
| PB12 | TB6612 AIN1 | 方向控制 |
| PB13 | TB6612 AIN2 | 方向控制 |
| PB14 | TB6612 PWMA | PWM调速 |
| PA6 | 编码器绿线(A相) | TIM3_CH1 |
| PA7 | 编码器黄线(B相) | TIM3_CH2 |
| PB6 | OLED SCL | I2C时钟 |
| PB7 | OLED SDA | I2C数据 |
| 3.3V | TB6612 STBY | 使能 |
| 5V | 编码器蓝线 | 编码器供电 |
| GND | 共地 | 电池盒GND+STM32GND+TB6612GND |

## 项目进度
- [x] Day 1 (7.28)：开环 PWM，电机正反转正常
- [x] Day 2 (7.29)：编码器测速完成，电机联合运行正常
- [x] Day 3 (7.30)：OLED 显示 + 按键调速 + 转速计算完成
- [x] Day 4 (7.31): PID闭环稳速完成
- [ ] Day 5 (8.1): 代码重构+最终演示

## 核心参数
| 参数 | 数值 |
|------|------|
| 减速比 | 370:1 |
| 编码器分辨率 | 13 PPR（电机轴） |
| 四倍频后 | 52 脉冲/电机轴转 |
| 输出轴每转脉冲数 | 19240 |
| RPM计算公式 | Delta × 600 / 19240 |
| PID参数(Kp/Ki/Kd) | 3.0 / 0.3 / 0 |
| 采样周期 | 100ms |
| 电机极限转速(7.4V) | ~25 RPM（输出轴） |

## 测试记录

### Day 1 开环 PWM 占空比测试（USB 5V 供电）

- 占空比测试数据：

  | 占空比 | 转速 | 现象 |
  |--------|------|------|
  | 20% | 慢 | 可稳定启动 |
  | 50% | 中 | 正常 |
  | 80% | 快 | 明显加速 |
  | 100% | 最快 | 声音变大 |

- **结论**：20% 以上可稳定启动，线性度良好，适合后续 PID 闭环。

### Day 2 编码器测速
- 手转电机轴：Count 正常变化，编码器接线正确
- 电机 30% 占空比自转：Delta 稳定在 1565~1588
- 输出轴转速约 4.9 RPM
- **转速计算公式：** `RPM = Delta × 600 / 19240`

### Day 3 OLED + 按键调速
- OLED 实时显示 Target / Actual / PWM / Delta
- KEY1 加速（+20），KEY2 减速（-20），KEY3 停止

### Day4 PID闭环
- 目标20 RPM，实际稳在19~20，稳态误差<5%
- PWM自动调节（58→92），积分项消除静差
- 电机物理极限约25 RPM（7.4V电池供电）

## 文件结构
Project/
├── User/
│   └── main.c              # 主循环：按键+PID+显示
├── Hardware/
│   ├── Motor.c/h           # 电机驱动
│   ├── PWM.c/h             # 定时器PWM
│   ├── Encoder.c/h         # 编码器接口(TIM3)
│   ├── PID.c/h             # PID算法
│   ├── OLED.c/h            # OLED显示
│   ├── Key.c/h             # 按键扫描
│   ├── Serial.c/h          # 串口调试
│   └── Delay.c/h           # 延时
├── Library/                # 标准库
├── Start/                  # 启动文件
└── Media/                  # 测试截图/视频

## 接线图与测试媒体
- 📷 [查看接线图](Media/Day1_Wiring.jpg)
- 🎬 [Day1 电机正转测试](Media/Day1_Motor_Run.mp4)
- 📷 [Day2 编码器测速照片 1](Media/Day2_Encoder_Test1.jpg)
- 📷 [Day2 编码器测速照片 2](Media/Day2_Encoder_Test2.jpg)
- 🎬 [Day3 OLED 运行视频](Media/Day3_OLED_Run.mp4)
- 📷 [Day3 OLED 运行照片](Media/Day3_OLED_Running.jpg)
- 🎬 [Day4 PID 闭环测试视频](Media/Day4_PID_Stable.mp4)

## 更新日志
- **2026-07-28**：USB 供电测试通过，电机开环运行正常
- **2026-07-28**：开环 PWM 占空比测试，四档转速正常
- **2026-07-29**：编码器模块完成，电机 + 编码器联合测试通过
- **2026-07-30**：OLED 显示 + 按键控制 + 转速计算完成
- **2026-07-30**：PID 闭环稳速完成

## 待优化
- [ ] 目标值上限限制（防止超过电机物理极限）
- [ ] 按键步进改为±5，精细调节
- [ ] 电池供电最终演示视频