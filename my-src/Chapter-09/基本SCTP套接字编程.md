# 基本SCTP套接字编程

## 接口模型

SCTP套接字分为：一到一套接字和一到多套接字。

* **一到一套接字**: 对应一个单独的SCTP关联(一个SCTP关联是两个系统之间的一个连接，不过由于多宿原因而在每个端点涉及不止一个IP地址)这种映射类似于TCP套接字和TCP连接的对应关系。
* **一到多套接字**: 一个给定的套接字上可以同时有多个活跃的SCTP关联。这种映射类似于绑定了某个特定端口的UDP套接字能够从若干个同时在发送数据的远程UDP端点接收彼此交错的数据报。

在考虑使用何种模型时，需要考虑应用程序的多个因素：

* 所编写的服务器程序是迭代的还是并发的？
* 服务器希望管理多少套接字？
* 优化关联建立的四路握手过程，使得能够在其中第三个(或者第四个)分组交换用户数据，这一点很重要吗？
* 应用进程希望维护多少个连接状态？

### 一到一形式

开发一到一形式的目的是方便现有TCP应用程序移植到SCTP上，但是二者之间仍有差异：

1. 任何TCP套接字选项必须转换成等效的SCTP套接字选项。比如`TC_NODELAY`和`TCP_MAXSEG`应该映射成`SCTP_NODELAY`和`SCTP_MAXSEG`。
2. SCTP保存消息边界，因而应用层消息边界并非必须。(比如说基于TCP的某个应用程序首先使用`write`写两个字节，给出消息的长度；然后再使用`write`写x字节给出消息的内容；改用SCTP后，接收端SCTP将接收到两个独立的消息，也就是需要两个独立的`read`才能返回所有数据，一个2字节的消息长度，一个x字节的消息内容)
3. 有些TCP应用进程使用半关闭来告知对端去往它的数据流已经结束。
4. `send`函数

服务器启动后，打开一个套接字，`bind`一个地址，然后就等着`accept`客户关联。一段时间后客户启动，它也打开一个套接字，并初始化与服务器的一个关联。

``` C
sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
```

### 一到多形式

一到多形式给应用程序开发人员提供这样的能力：编写的服务器无需管理大量套接字描述符。单个套接字描述符将代表多个关联，就像一个UDP套接字能够从多个客户接收消息那样。

在一到多式套接字上，用于标识单个关联的是一个**关联标识**(association identifier)。关联标识是一个类型为`sctp_assoc_t`的值，通常是一个整数。它通常是一个不透明的值，应用进程不应该使用任何不是由内核给予的关联标识。

使用一到多套接字时我们应该注意以下几点：

1. 当一个客户关闭其关联时，其服务器也将自动关闭同一个关联，服务器主机内核不再有该关联的状态。
2. 可用于致使在四路握手中的第三个或者第四个分组中捎带用户数据的唯一办法就是使用一到多形式。
3. 对于一个与它还没有关联存在的IP地址，任何以它为目的地址的`sendto`, `sendmsg`和`sctp_sendmsg`将导致对主动打开的尝试，如果成功的话，将会建立一个与该地址的新关联。这种行为的发生与执行分组发送的这个应用进程是否曾调用过`listen`以请求被动打开无关。
4. 用户必须使用`sendto`, `sendmsg`或者`sctp_sendmsg`这三个分组发送函数，而不能使用`send`或者`write`函数来进行数据的发送，除非已经使用`sctp_peeloff`函数从一个一到多套接字中剥离出一个一到一式套接字。
5. 任何时候调用其中任何一个分组发送函数时，所用的目的地址是由系统在关联建立阶段选定的**主目的地址**，除非调用者在所提供的`sctp_sndrcvinfo`结构中设置了`MSG_ADDR_OVER`标志。为了提供这个结构，调用者必须使用伴随辅助数据的`sendmsg`或者`sctp_sendmsg`函数。
6. 关联事件可能被启用，因此要是应用进程不希望收到这些时间，就得用`SCTP_EVENTS`套接字选项来显式禁止它们。默认情况下唯一启用的事件是`sctp_data_io_event`，它给`recvmsg`和`sctp_recvmsg`调用提供辅助数据。这个默认设置同时适用于一到一形式和一到多形式。

``` C
sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
```

## 基本函数

### `sctp_bindx`函数

``` C
#include <netinet/sctp.h>

int sctp_bindx(int sockfd, const struct sockaddr *addrs, int addrcnt, int flags);
```

其中`flags`指导该函数的行为：

``` C
SCTP_BINDX_ADD_ADDR
SCTP_BINDX_REM_ADDR
```

`sctp_bindx`函数既可以用于已绑定的套接字，也可以用于未绑定的套接字：

1. 对于未绑定的套接字，`sctp_bindx`调用将把给定的地址集合捆绑到其上。
2. 对于已绑定的套接字，若指定`SCTP_BINDX_ADD_ADDR`则把额外的地址加入套接字描述符，若指定`SCTP_BINDX_REM_ADDR`则从套接字描述符已加入地址中移除给定的地址。

有几个注意点：

1. 如果在一个**监听套接字**上执行`sctp_bindx`调用，那么将来产生的关联将使用新的地址配置，已经存在的关联则不受影响。
2. 传递给`sctp_bindx`的两个flag是互斥的，同时制定会返回`EINVAL`错误。所有套接字地址结构的端口号必须相同，而且必须与已经绑定的端口号相匹配，否则返回`EINVAL`错误。

### `sctp_sendmsg`函数

``` C
#include <netinet/sctp.h>

ssize_t sctp_sendmsg(int sockfd, const void *msg, size_t msgsz,
	const struct sockaddr *to, socklen_t tolen,
    uint32_t ppid,
    uint32_t flags, uint16_t stream,
    uint32_t timetolive, uint32_t context);
```

解释一些参数：

* `ppid`参数指定将随数据块传递的**净荷协议标识符**
* `flags`参数将传递给SCTP栈，用以标识任何SCTP选项
* `stream`指定一个SCTP流号
* `lifetime`参数以毫秒为单位指定消息的生命期，其中0表示无限生命期
* `context`参数用于指定可能有的用户上下文(用户上下文把通过消息通知机制收到的某次失败的消息发送与某个特定于应用的本地上下文关联起来)

### `sctp_recvmsg`函数

``` C
#include <netinet/sctp.h>

ssize_t sctp_recvmsg(int sockfd, void *buf, size_t msgsz,
	struct sockaddr *from, socklen_t fromlen,
    struct sctp_sndrcvinfo *sinfo,
    int *msg_flags);
```

### `shutdown`函数

``` C
#include <sys/socket.h>

int shutdown(int sockfd, int how);
```

`shutdown`函数可用于一到一式的SCTP端点。由于SCTP设计成**不提供半关闭状态**，SCTP端点对`shutdown`的反应不同于TCP端点。当相互通信的两个SCTP端点中的任何一个发起关联终止序列时，这两个端点都得把已排队的任何数据发送掉，然后关闭关联。
关联主动打开的发起端点调用`shutdown`而不是`close`的原因可能是：同一个端点可用于连接到一个新的对端端点。与TCP不同，新的套接字打开之前不必调用`close`。SCTP允许一个端点调用`shutdown`，`shutdown`结束之后，这个端点就可以重用原始套接字连接到一个新的对端。但是需要注意的是，如果这个端点没有等到SCTP关联终止序列结束，新的连接就会失败。

对SCTP，`shutdown`的`how`参数语义如下：

* `SHUT_RD`：没有任何SCTP协议行为发生
* `SHUT_WR`：禁止后续发送操作，激活SCTP关联终止过程，以此终止当前关联。注意，这个操作不提供半关闭状态，不过允许本地端点读取已经排队的数据，这些数据时对端在收到SCTP的`SHUTDOWN`消息之前就已经发送给了本端的。
* `SHUT_RDWR`：禁止所有的`read`操作和`write`操作，激活SCTP关联终止过程。传送到本地端点的任何已经排队的数据都得到确认，然后悄然丢弃。
