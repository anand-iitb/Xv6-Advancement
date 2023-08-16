rm disk*
gcc testcase1.c emufs-disk.c emufs-ops.c
./a.out > output1

gcc testcase2.c emufs-disk.c emufs-ops.c
./a.out > output2
#input a key for encryption

gcc testcase3.c emufs-disk.c emufs-ops.c
./a.out > output3

gcc testcase4.c emufs-disk.c emufs-ops.c
./a.out > output4
# input 2 keys for 2 encrypted devices
