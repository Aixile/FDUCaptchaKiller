#FudanCaptchaKiller

##Overview
A program simpily recoginize CAPTCHAs on serveral sites of Fudan University.

It support 

- [xk.fudan.edu.cn](http://xk.fudan.edu.cn)	
- [mail.fudan.edu.cn](http://mail.fudan.edu.cn)*
- [www.freshman.fudan.edu.cn](http://www.freshman.fudan.edu.cn)	

*If you send more than 200 mails with similar content in a short time, then you will see CAPTCHA authentication.

##Usage
**Usage: FDUCaptchaKiller xk|mail|freshman ImageFilePath  [libsvm_model]**

The first parameter is the working mode of program, please specify it manually.

And the last parameter is libsvm trained model, there is no need for you to specify it under common situation.

##Accuracy
- [xk.fudan.edu.cn](http://xk.fudan.edu.cn)	>99%(1000/1000)
- [mail.fudan.edu.cn](http://mail.fudan.edu.cn) >99%(1043/1053)
- [www.freshman.fudan.edu.cn](http://www.freshman.fudan.edu.cn)	>99%(1000/1000)

##Licence
This program is under the BSD license.