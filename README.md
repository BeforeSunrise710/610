# 610
我们把小程序和Arduino的代码分别放在两个文件夹里。
接收天气数据的代码和舵机控制代码被分开，以便分别引用。
这里的舵机控制代码没有依靠<Servo.h>头文件，而是自己改写了舵机控制代码。 所以，我们的舵机可以实现稳定的运动，而不会不稳定地抖动。
我们通过光照传感器实现开箱与否的识别。
