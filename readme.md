# OpenGL Grass Render

# 介绍 | introduction

使用 OpenGL 实现的草地渲染效果

- **草叶建模**：每片草叶采用简化几何结构，通过顶点着色器进行位置与形态控制，兼顾性能与视觉质量。

- **实例化渲染（Instancing）** ：使用 glDrawArraysInstanced 批量绘制成百万片草叶，大幅提升渲染效率。

- **风动动画**：通过顶点着色器中引入时间与噪声函数，模拟草叶随风摆动的动态效果，增强自然感。

- **地形适配**：使用Perlin噪声生成连续地形高度，实现草地随地形起伏自然分布。

- **颜色渐变**：草叶采用渐变色或按高度混合不同紫色调，模拟草地的层次感。

#  渲染结果 | render result | [B站视频](https://www.bilibili.com/video/BV1MMMtzcE2R/?vd_source=0bc24278cd7a0c76a9cea9d12245ddd8)

![img](./img/pic1.png)

![img](./img/windBlow.gif)

![img](./img/grass.gif)