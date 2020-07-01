# AEC-ECC 混合加密系统

采用AES和ECC和混合加密

分模块编写，方便二次开发使用

SHA-3采用元编程实现

AES目前实现了AES-128

ECC使用了c++和py交互编程

由于大数原因，使用python增加系统稳定性，使用c++调用python

待后续更新

下面演示了一次使用此混合密码的流程

首先Bob生成密钥和公共点

再由Alice根据公共点生成密钥对

接着Alice使用Bob的公钥和自己的私钥对信息进行加密

接着Bob使用Alice的公钥的自己的私钥进行解密

相关信息皆存于文件中

项目演示

![show](./image/show.gif)