# DC-Motor-PID-Control

基于 STM32F103 的直流电机 PID 闭环调速系统，含编码器测速、OLED 显示与按键交互。

![MCU](https://img.shields.io/badge/MCU-STM32F103-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Status](https://img.shields.io/badge/Status-进行中-yellow)

## 硬件清单
- **主控**：STM32F103C8T6 最小系统板
- **驱动**：TB6612 电机驱动
- **电机**：JGA25-370 6V 减速电机（带霍尔编码器，减速比 1:192.4）
- **显示**：0.96 寸 OLED（I2C 接口）
- **电源**：18650 电池盒 7.4V（待到货，目前 USB 5V 供电）

## 项目进度
- [x] Day 1 (7.28)：开环 PWM，电机正反转正常
- [x] Day 2 (7.29)：编码器测速完成，电机联合运行正常
- [x] Day 3 (7.30)：OLED 显示 + 按键调速 + 转速计算完成
- [ ] Day 4 (7.31)：PID 闭环控制
- [ ] Day 5 (8.1)：整合优化 + 最终演示

## 接线图与测试媒体
- 📷 [查看接线图](Media/Day1_Wiring.jpg)
- 🎬 [Day1 电机正转测试](Media/Day1_Motor_Run.mp4)
- 📷 [Day2 编码器测速照片 1](Media/Day2_Encoder_Test1.jpg)
- 📷 [Day2 编码器测速照片 2](Media/Day2_Encoder_Test2.jpg)
- 🎬 [Day3 OLED 运行视频](Media/Day3_OLED_Run.mp4)
- 📷 [Day3 OLED 运行照片](Media/Day3_OLED_Running.jpg)

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

## 更新日志
- **2026-07-28**：USB 供电测试通过，电机开环运行正常
- **2026-07-28**：开环 PWM 占空比测试，四档转速正常
- **2026-07-29**：编码器模块完成，电机 + 编码器联合测试通过
- **2026-07-30**：OLED 显示 + 按键控制 + 转速计算完成