7月28号-增加在解码频率测试最后统一关闭PWM捕获检测功能，避免出现当检测不到波形时，一直进入溢出中断，但是任然在检测没有关闭检测功能的情况。
7月28号——由于调试和实际生产硬件存在差异，修改pmu-FI功能的DAC转换公式为2048。
7月30号——将关闭定时器的函数放入了结束函数void GPIO_Init_ExcBin(void)中。
