TARGET = base stage1 stage2 stage3

all: $(TARGET)

$(TARGET):
	gcc -no-pie -static $@.c util.c -o $@ -masm=intel -pthread
	scp -i bullseye.id_rsa -P 10021 ./$@ root@localhost:/root/
	rm $@
