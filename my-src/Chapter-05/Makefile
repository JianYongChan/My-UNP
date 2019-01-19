cli_objects = str_cli.o

srv_objects = str_echo.o sigchld.o

exe_objects = cli01 cli04 srv01 srv02 srv04 tcpcli01.o tcpcli04.o tcpsrv01.o tcpsrv02.o tcpsrv04.o

cli01: tcpcli01.o $(cli_objects)
	cc -o cli01 tcpcli01.o $(cli_objects) -lunp

cli04: tcpcli04.o $(cli_objects)
	cc -o cli04 tcpcli04.o $(cli_objects) -lunp

srv01: tcpsrv01.o str_echo.o
	cc -o srv01 tcpsrv01.o str_echo.o -lunp

srv02: tcpsrv02.o $(srv_objects)
	cc -o srv02 tcpsrv02.o $(srv_objects) -lunp

srv04: tcpsrv04.o $(srv_objects)
	cc -o srv04 tcpsrv04.o $(srv_objects) -lunp

srv05: tcpsrv04.o str_echo2.o sigchld.o
	cc -o srv05 tcpsrv04.o str_echo2.o sigchld.o -lunp

str_cli.o: str_cli.c
	cc -c str_cli.c

tcpcli01.o: tcpcli01.c str_cli.c
	cc -c tcpcli01.c

tcpcli04.o: tcpcli04.c str_cli.c
	cc -c tcpcli04.c


str_echo.o: str_echo.c
	cc -c str_echo.c

str_echo2.o: str_echo2.c
	cc -c str_echo2.c

sigchld.o: sigchld.c
	cc -c sigchld.c

tcpsrv01.o: tcpsrv01.c str_echo.c sigchld.c
	cc -c tcpsrv01.c

tcpsrv02.o: tcpsrv02.c str_echo.c sigchld.c
	cc -c tcpsrv02.c

tcpsrv04.o: tcpsrv04.c str_echo.c sigchld.c
	cc -c tcpsrv04.c


# .PHONY表示clean是一个伪目标
.PHONY: clean
# rm前的-表示某些文件删除时出错就忽略
clean:
	-rm $(cli_objects) $(srv_objects) $(exe_objects)
