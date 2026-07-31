# DC-Motor-PID-Control

基于 STM32F103 的直流电机 PID 闭环调速系统，实现编码器测速、OLED 实时显示与按键交互控制，稳态误差 ≤ 1 RPM。

![MCU](https://img.shields.io/badge/MCU-STM32F103-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Status](https://img.shields.io/badge/Status-已完成-brightgreen)

## 项目亮点
- 开环 PWM 驱动：电机正反转、四档占空比调速
- 霍尔编码器测速：TIM3 编码器接口，四倍频（52 PPR）
- PID 闭环稳速：Kp=3.0, Ki=0.3，采样周期 100ms，稳态误差 ≤ 1 RPM
- OLED 实时显示：Target / Actual / PWM / Err
- 按键人机交互：KEY1 加速(+5)，KEY2 减速(-5)，KEY3 停止

## 硬件清单
| 模块 | 型号 | 说明 |
|------|------|------|
| 主控 | STM32F103C8T6 | 最小系统板 |
| 电机驱动 | TB6612FNG | 双路 H 桥 |
| 电机 | JGA25-370 6V | 减速比 370:1，带霍尔编码器 |
| 编码器 | 霍尔 AB 相 | 13 PPR（电机轴），四倍频后 52 脉冲/转 |
| 显示 | 0.96 寸 OLED | I2C 接口，4 行显示 |
| 按键 | 轻触开关 ×3 | KEY1 加速，KEY2 减速，KEY3 停止 |
| 电源 | 2×18650 电池盒 | 7.4V 串联，接 TB6612 VM |

## 接线图
| STM32 引脚 | 连接设备 | 功能 |
|-----------|---------|------|
| PB12 | TB6612 AIN1 | 方向控制 |
| PB13 | TB6612 AIN2 | 方向控制 |
| PB14 | TB6612 PWMA | PWM 调速 |
| PA6 | 编码器 A 相（绿线） | TIM3_CH1 |
| PA7 | 编码器 B 相（黄线） | TIM3_CH2 |
| PB6 | OLED SCL | I2C 时钟 |
| PB7 | OLED SDA | I2C 数据 |
| 3.3V | TB6612 STBY | 使能 |
| 5V | 编码器供电（蓝线） | 编码器电源 |
| GND | 共地 | 电池盒 GND + STM32 GND + TB6612 GND |

## 核心参数
| 参数 | 数值 |
|------|------|
| 减速比 | 370:1 |
| 编码器分辨率 | 13 PPR（电机轴） |
| 四倍频后 | 52 脉冲/电机轴转 |
| 输出轴每转脉冲数 | 19240 |
| PID 参数 (Kp / Ki / Kd) | 3.0 / 0.3 / 0 |
| 采样周期 | 100 ms |
| 电机极限转速 (7.4V) | ~25 RPM（输出轴） |

**转速计算公式：** `RPM = Delta × 600 / 19240`

## 项目进度
- [x] Day 1 (7.28)：开环 PWM，电机正反转正常
- [x] Day 2 (7.29)：编码器测速完成，电机联合运行正常
- [x] Day 3 (7.30)：OLED 显示 + 按键调速 + 转速计算完成
- [x] Day 4 (7.31)：PID 闭环稳速完成
- [x] Day 5 (8.1)：代码重构 + 电池供电最终演示

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
- 开环下转速与占空比基本线性

### Day 4 PID 闭环
- 目标 20 RPM，实际稳在 19~20，稳态误差 &lt; 5%
- PWM 自动调节（58→92），积分项消除静差
- 电机物理极限约 25 RPM（7.4V 电池供电）

### Day 5 最终演示（电池供电）

| 目标转速 | 实际转速 | 稳态误差 | PWM |
|----------|----------|----------|-----|
| 5 RPM | 4~5 RPM | ±1 | 15~22 |
| 20 RPM | 19~20 RPM | ±1 | 75~85 |
| 25 RPM | 24~25 RPM | ±1 | 100（饱和）|

## 文件结构
```c
Project/
├── User/
│   └── main.c              # 主循环：按键 + PID + 显示
├── Hardware/
│   ├── Motor.c/h           # 电机驱动
│   ├── PWM.c/h             # 定时器 PWM
│   ├── Encoder.c/h         # 编码器接口 (TIM3)
│   ├── PID.c/h             # PID 算法
│   ├── OLED.c/h            # OLED 显示
│   ├── Key.c/h             # 按键扫描
│   ├── Serial.c/h          # 串口调试
│   └── Delay.c/h           # 延时
├── Library/                # 标准库
├── Start/                  # 启动文件
└── Media/                  # 测试截图 / 视频
```

## 接线图与测试媒体
- 📷 [查看接线图](Media/Day1_Wiring.jpg)
- 🎬 [Day 1 电机正转测试](Media/Day1_Motor_Run.mp4)
- 📷 [Day 2 编码器测速照片 1](Media/Day2_Encoder_Test1.jpg)
- 📷 [Day 2 编码器测速照片 2](Media/Day2_Encoder_Test2.jpg)
- 🎬 [Day 3 OLED 运行视频](Media/Day3_OLED_Run.mp4)
- 📷 [Day 3 OLED 运行照片](Media/Day3_OLED_Running.jpg)
- 🎬 [Day 4 PID 闭环测试视频](Media/Day4_PID_Stable.mp4)
- 📷 [Day 5 项目运行照片 (5 RPM)](Media/Day5_Target05.jpg)
- 📷 [Day 5 项目运行照片 (20 RPM)](Media/Day5_Target20.jpg)
- 📷 [Day 5 项目运行照片 (25 RPM)](Media/Day5_Target25.jpg)

## 演示视频
- [Bilibili: STM32 直流电机 PID 闭环调速系统演示](https://b23.tv/Cbqxt7f)

## 更新日志
- **2026-08-01**：代码重构，电池供电最终演示完成
- **2026-07-31**：PID 闭环稳速完成，稳态误差 ≤ 1 RPM
- **2026-07-30**：OLED 显示 + 按键控制 + 转速计算完成
- **2026-07-29**：编码器模块完成，电机 + 编码器联合测试通过
- **2026-07-28**：开环 PWM 占空比测试，四档转速正常
- **2026-07-28**：USB 供电测试通过，电机开环运行正常

## 已完成优化
- [x] 目标值上限限制 ±25（防止超过电机物理极限）
- [x] 按键步进改为 ±5，精细调节
- [x] 电池供电最终演示
