#include "emufs.h"

int main(){

    int mnt2 = opendevice("disk6", 60);
    if(mnt2==-1){
        printf("error!\n");
        return 0;
    }
    if(create_file_system(mnt2, 1)==-1){
        printf("error!\n");
        return 0;
    }

    int dir2 = open_root(mnt2);
    emufs_create(dir2, "file1", 0);
    emufs_create(dir2, "file2", 0);
    emufs_create(dir2, "file3", 0);
    emufs_create(dir2, "file4", 0);

    int fd1 = open_file(dir2,"file1");
    char str1[] = "!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!";
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd1, str1, 64*4);
    char buf1[256 * 2 + 1];
    buf1[256 * 2] = '\0';
    emufs_seek(fd1, -256*2);
    emufs_read(fd1, buf1, 256*2);
    printf("Data: %s\n", buf1);
    emufs_close(fd1, 0);
    fsdump(mnt2);

    fd1 = open_file(dir2, "file2");
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd1, str1, 64*4);
    char buf2[256 * 4 + 1];
    buf2[256 * 4] = '\0';
    emufs_seek(fd1, -256*4);
    emufs_read(fd1, buf2, 256*4);
    printf("Data: %s\n", buf2);
    emufs_close(fd1, 0);
    fsdump(mnt2);

    fd1 = open_file(dir2, "file3");
    int fd2 = open_file(dir2, "file4");
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd2, str1, 64*4);
    emufs_write(fd1, str1, 64*4);
    emufs_write(fd2, str1, 64*4);
    fsdump(mnt2);
    emufs_delete(dir2, "file2");
    fsdump(mnt2);
    emufs_delete(dir2, "file3");
    fsdump(mnt2);
    emufs_close(fd2, 0);

    emufs_create(dir2, "file3", 0);
    int fd3 = open_file(dir2, "file3");
    char str2[] = "!-----------------------70 Bytes of Data-----------------------------!!-----------------------70 Bytes of Data-----------------------------!!-----------------------70 Bytes of Data-----------------------------!!-----------------------70 Bytes of Data-----------------------------!";
    char str3[] = "!-------------46 Bytes of Data---------------!!-------------46 Bytes of Data---------------!!-------------46 Bytes of Data---------------!!-------------46 Bytes of Data---------------!";
    emufs_write(fd3, str2, 70*4);
    emufs_write(fd3, str2, 70*4);
    emufs_write(fd3, str2, 70*4);
    emufs_write(fd3, str3, 46*4);

    char buf3[70*4 + 1];
    buf3[70*4] = '\0';
    char buf4[46*4+1];
    buf4[46*4] = '\0';

    emufs_seek(fd3, -256*4);
    emufs_read(fd3, buf3, 70*4);
    printf("Data: %s\n", buf3);
    emufs_read(fd3, buf3, 70*4);
    printf("Data: %s\n", buf3);
    emufs_read(fd3, buf3, 70*4);
    printf("Data: %s\n", buf3);
    emufs_read(fd3, buf4, 46*4);
    printf("Data: %s\n", buf4);
    fsdump(mnt2);

    return 0;
}