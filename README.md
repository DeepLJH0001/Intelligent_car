# Intelligent_car
# 智能小车 <br />
主要功能：使用IntelGalileo Gen2开发板组装小车，实现基本的gpio控制（电机、舵机）、数据采集（摄像头和超声波测距）、手机端小车端的通讯与控制等。
开发板本身支持ardunio，但这次所做主要使用busybox裁剪的linux系统完成、这样主要的传感器驱动并不需要自己手动去写。<br />
<br />
开发环境详见：https://www-ssl.intel.com/content/www/us/en/do-it-yourself/get-help.html<br />
&emsp; &emsp; &emsp; &emsp; &emsp;&emsp;http://www.intel.com/support/galileo/sb/CS-035168.htm<br />
<br />
最终目标：使用摄像头、超声波采集的数据，控制电机，完成壁障赛道自动行走、使用手机完成外网穿透控制小车监控家内情况<br />
<br />
模块介绍：1.IntelGalileo Gen2（一块）、网卡（一块）、天线（一根）2.小车底座（一辆）3.12V-3A电池（一个）
&emsp;4.电机驱动模块（一个）
&emsp;5.电源稳压模块（一个）
&emsp;6.测速传感器（两个）
&emsp;7.超声波模块（一个）
&emsp;8.舵机（一个）
&emsp;9.摄像头（一个）
&emsp;10:usb转串口模块
&emsp;11.零件
<br />
开发板电路图及其参数<br />
<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/QQ%E5%9B%BE%E7%89%8720170811103530.jpg?raw=true)
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/QQ%E6%88%AA%E5%9B%BE20170811103617.png?raw=true)<br />
手机端（安卓实现）<br />
<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/Screenshot_2017-08-11-10-59-54-721_com.atr.Car_v2.png?raw=true)
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/Screenshot_2017-08-11-11-00-06-983_com.atr.Car_v2.png?raw=true)
