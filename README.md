# gpspubsub
simple DDS position service


note: line 47 of auto-generated makefile needs to be modified to include -lCppLinuxSerial:
SYSLIBS = -ldl -lm -lpthread -lrt -lCppLinuxSerial -no-pie -rdynamic
