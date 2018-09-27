## QtTsTranslation
QtTsTranslation 用于帮助 Qt 开发者翻译 Qt [翻译文件](http://doc.qt.io/qt-5/linguist-manager.html),其中翻译工作通过网络接口实现,目前支持:
- [x] 百度
- [ ] 搜狗
- [ ] 腾讯

## Build
``` shell
qmake
make
```

## Usage 

### step1

请前往您要使用的翻译接口网站申请相应的 appid 与appkey,入口如下

- [百度](http://fanyi-api.baidu.com/api/trans/product/index)
- [搜狗](http://ai.sogou.com)
- [腾讯](https://ai.qq.com/doc/nlptrans.shtml)

### step2

软件设置 step1 获取的 appid 和 appkey

### step3

打开 Qt ts 文件,点击开机即可进行翻译

## ScreenShot
![ScreenShot](https://github.com/hejianglin/QtTsTranslator/blob/master/screenshot.png)


## License
[MIT](https://github.com/hejianglin/QtTsTranslator/blob/master/LICENSE)

