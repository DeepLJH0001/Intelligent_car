# Intelligent_car
# 智能小车 <br />
主要功能：使用IntelGalileo Gen2开发板组装小车，实现基本的gpio控制（电机、舵机）、数据采集（摄像头和超声波测距）、手机端小车端的通讯与控制等。
开发板本身支持ardunio，但这次所做主要使用busybox裁剪的linux系统完成、这样减少了传感器驱动程序的编写。<br />
<br />
开发环境详见：https://www-ssl.intel.com/content/www/us/en/do-it-yourself/get-help.html<br />
&emsp; &emsp; &emsp; &emsp; &emsp;&emsp;http://www.intel.com/support/galileo/sb/CS-035168.htm<br />
<br />
最终目标：使用摄像头、超声波采集的数据，控制电机，完成壁障赛道自动行走、使用手机完成外网穿透控制小车监控家内情况（openWRT、完成一半）、小车自主拍照得到的图片自主发微博<br />
<br />
模块介绍：1.IntelGalileo Gen2（一块）、网卡（一块）、天线（一根）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;2.小车底座（一辆）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;3.12V-3A电池（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;4.电机驱动模块（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;5.电源稳压模块（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;6.测速传感器（两个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;7.超声波模块（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;8.舵机（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;9.摄像头（一个）<br />
&emsp;&emsp;&emsp;&emsp;&emsp;10:usb转串口模块<br />
&emsp;&emsp;&emsp;&emsp;&emsp;11.零件<br />
<br />
模块图片<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/%E6%97%A0%E6%A0%87%E9%A2%98.png?raw=true)
<br />
<br />
开发板电路图及其参数<br />
<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/QQ%E5%9B%BE%E7%89%8720170811103530.jpg?raw=true)
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/QQ%E6%88%AA%E5%9B%BE20170811103617.png?raw=true)<br />
手机端（安卓实现）<br />
<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/Screenshot_2017-08-11-10-59-54-721_com.atr.Car_v2.png?raw=true)
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/Screenshot_2017-08-11-11-00-06-983_com.atr.Car_v2.png?raw=true)
微博API介绍<br />
1、去新浪微博开放平台：http://open.weibo.com/ 申请<br />
2、选择移动应用场景、获取Access Token<br />
3、创建申请信息、获取App Key、App Secret<br />
值得注意的地方：<br />
1、Oauth2.0授权设置<br />
应用创建完后可以在“管理中心”-“我的应用”中查看信息，在“应用信息”--“高级信息”中可以设置网站的授权回调页和取消授权回调页。
授权回调页非常重要，一定要填写正确，当用户授权成功后会回调到此页面，传回一个“code”参数，开发者可以用code换取Access Token值。<br />
2、引导用户授权<br />
引导需要授权的用户到如下页面：
https://api.weibo.com/oauth2/authorize?client_id=YOUR_CLIENT_ID&response_type=code&redirect_uri=YOUR_REGISTERED_REDIRECT_URI
YOUR_CLIENT_ID：即应用的AppKey，可以在应用基本信息里查看到。
YOUR_REGISTERED_REDIRECT_URI：即之前填写的授权回调页，注意一定要完全相同。
如果用户授权成功后，会跳转到回调页，开发者此时需要得到url参数中的code值，注意code只能使用一次。<br />
3、换取Access Token<br />
在Ubuntu里的火狐浏览器安装一个post插件，在post的URL中输入：
https://api.weibo.com/oauth2/access_token?client_id=YOUR_CLIENT_ID&client_secret=YOUR_CLIENT_SECRET&grant_type=authorization_code&redirect_uri=YOUR_REGISTERED_REDIRECT_URI&code=CODE
点击post就可以获得：
"access_token": "xxxxxx",
"expires_in": xxxxx,
"remind_in":"xxxxxx",
"uid":"xxxxxxxx"
{"access_token":"2.00XcgbcCnUKPbB3d6778b56fHHZomD","remind_in":"157679999","expires_in":157679999,"uid":"2402748405"}<br />
4、调用API<br />
获取到Access Token后，开发者可以保存它的值，调用API的时候直接用就可以了。Access Token有一定的有效期，过期后需要重新授权。<br />
申请审核渣浪要等好几天,耐心等待<br />
![](https://github.com/DeepLJH0001/Intelligent_car/blob/master/image/QQ%E5%9B%BE%E7%89%8720170811113805.jpg?raw=true)
