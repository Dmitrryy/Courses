# LLVM course 2022/2023

## Task 1
As the first task, it was set to implement in c any task that can be visualized in real time using the interface: put_pixel(y, x, argb) and flush().

### Solution
The rendering driver is implemented in SFML. (see sEngine.cpp) \
The implementation of shading is chosen as the task. The source of the idea [shadertoy](https://www.shadertoy.com/view/WlcXR4#). \
If you think I just did ctrl+C, ctrl+V, then just take a look at the integer cosine in main.cpp.

![fract](captures/fract.gif)

