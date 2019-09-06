# 说明
+ 在002的基础上修改用lds文件指定data段存放位置
# 输出
```
-rwxr-xr-x 1 root root   2049 Sep  1 17:55 main.bin
```
因为指定`AT(0x800)`并且变量为1个字节所以bin文件为0x801