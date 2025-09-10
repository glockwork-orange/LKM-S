#
# Packet Logger Kernel Module – Execution Log

**Author**: Kevin Abraham Wilson
**Module Name**: `p_logger.ko`  
**Kernel Version**: 6.11.0-28-generic  
**Build Tool**: `make`  
**System**: Ubuntu (GCC 13.3.0)

---

## Module Compilation

```bash
$ sudo make
make -C /lib/modules/6.11.0-28-generic/build M=/home/kevin_abraham/Desktop/Kernel_MODS/packet_logger modules
make[1]: Entering directory '/usr/src/linux-headers-6.11.0-28-generic'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-13
  You are using:           gcc-13
  ...
  LD [M]  p_logger.ko
  BTF [M] p_logger.ko
Skipping BTF generation due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-6.11.0-28-generic'
````

---

## Module Insertion

```bash
$ sudo insmod p_logger.ko
```

---

## Recent Kernel Log Output (Last 10 entries from `[p_logger]`)

```bash
$ sudo dmesg | grep "\[p_logger\]" | tail -n 10
```

```text
[ 9305.502342] [p_logger] UDP SRC=8.8.8.8:53 DST=192.168.183.212:59513
[ 9305.502779] [p_logger] UDP SRC=127.0.0.53:53 DST=127.0.0.1:42224
[ 9305.526392] [p_logger] Packet logger module unloaded
[ 9336.704106] [p_logger] Packet logger module loaded
[ 9337.868272] [p_logger] TCP SRC=127.0.0.1:49006 DST=127.0.0.1:6379
[ 9337.868316] [p_logger] TCP SRC=127.0.0.1:6379 DST=127.0.0.1:49006
[ 9338.271649] [p_logger] TCP SRC=140.82.112.25:443 DST=192.168.183.212:43904
[ 9338.891007] [p_logger] TCP SRC=140.82.112.25:443 DST=192.168.183.212:43904
[ 9339.870293] [p_logger] TCP SRC=127.0.0.1:49020 DST=127.0.0.1:6379
[ 9339.870334] [p_logger] TCP SRC=127.0.0.1:6379 DST=127.0.0.1:49020
```

---

## Module Removal

```bash
$ sudo rmmod p_logger.ko
```

---

### Summary

* Netfilter hooked to `NF_INET_PRE_ROUTING`
* Logged both TCP and UDP packets with IP and port info
* Worked on loopback and external traffic
* Kernel module safely loaded/unloaded without errors

---

