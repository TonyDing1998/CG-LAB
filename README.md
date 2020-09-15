# CG-LAB
CG-LAB in 2019
华南理工大学软件学院2019年计算机图形学课程实验
Computer graphics course experiment in school of software, SCUT in 2019.

*说明：
* 1.实验环境:Windows 10,Visual Studio 2017 Community.
* 2.实验基础库：GLFW,GLAD,stb_image.h.
* 3.实验中通过读取obj及对应的mtl文件导入模型的部分参考了https://blog.csdn.net/tanshengjunjun/article/details/78746019 ，并在其基础上实现了多Mesh模型的导入。详见头文件Mesh.h和Model.h
* 4.纹理、光照相关的着色器参考了https://learnopengl-cn.github.io/ 中的相关内容。
* 5.obj、mtl文件说明：obj需要包含v（顶点数据）、vt（贴图数据）、vn（顶点法线数据）、f（面数据）<从win10自带的3D builder导出的数据会丢失vn、本实验采用的是用blender中导出的obj和mtl文件>
