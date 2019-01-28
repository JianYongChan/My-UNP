# Exercise-06

## 6.1

把数组放在一个结构体中，C是允许struct使用等号赋值的。

## 6.2

## 6.3

如果在19行的`if`前加上`else`，那么只有在套接字不可读的时候才会检查标准输入，那么套接字就变成单工的了。

## 6.4

使用`getrlimit`获得`RLIMIT_NOFILE`资源的当前值，然后调用`setrlimit`把软限制设置成硬限制。

## 6.5

。。。不知道怎么开启chargen服务。。。

## 6.6

以参数`SHUT_WR`和`SHUT_RDWR`调用`shutdown`函数时，总会发送FIN；而`close`只有在引用计数为1时才会发送FIN。

## 6.7

## 6.8
