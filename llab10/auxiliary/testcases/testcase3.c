#include "emufs.h"

int main(){
    int mnt1 = opendevice("disk1", 64);
    if(mnt1){
        printf("error!\n");
        return 0;
    }
    if(create_file_system(mnt1, 0)==-1){
        printf("error!\n");
        return 0;
    }

    int dir1 = open_root(mnt1);
    if(dir1==-1){
        printf("error!\n");
        return 0;
    }
    emufs_create(dir1, "dir1", 1);
    change_dir(dir1,"dir1");
    emufs_create(dir1,"dir2", 1);
    emufs_create(dir1, "file1", 0);

    change_dir(dir1,"..");
    emufs_create(dir1,"dir3",1);

    change_dir(dir1,"dir3");
    emufs_create(dir1,"dir4",1);
    emufs_create(dir1,"file2",0);

    change_dir(dir1,"../dir1/dir2");
    emufs_create(dir1,"dir5",1);
    emufs_create(dir1,"file3",0);

    fsdump(mnt1);

    change_dir(dir1, "../../dir3/.///dir4");
    emufs_create(dir1, "dir6", 1);
    emufs_create(dir1, "file4", 0);
    emufs_create(dir1, "dir7", 1);
    emufs_create(dir1, "file5", 0);

    fsdump(mnt1);

    change_dir(dir1, "/dir3/../dir1/dir2/dir5/");
    emufs_create(dir1, "file6", 0);
    emufs_create(dir1, "dir8", 1);
    change_dir(dir1, "dir8");
    emufs_create(dir1, "file7", 0);
    emufs_create(dir1, "file8", 0);
    change_dir(dir1,"../..");
    fsdump(mnt1);

    emufs_delete(dir1, "dir5");
    fsdump(mnt1);

    printf("\n\n");


    int mnt2 = opendevice("disk2", 60);
    if(mnt2==-1){
        printf("error!\n");
        return 0;
    }
    if(create_file_system(mnt2, 0)==-1){
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

    printf("\n\n");

    int dir3 = open_root(mnt1);
    int fd3 = open_file(dir3, "dir3/dir4/file4");
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
    fsdump(mnt1);

    change_dir(dir3, "dir3/dir4/dir7");
    for(int i=0; i<6; i++){
        char digit = i + '0';
        char fName[8];
        fName[8] = '\0';
        fName[0] = digit;
        strcpy(fName + 1, "_.txt");
        printf("Creating file %d_.txt: %d\n", i, emufs_create(dir3, fName, 0));
        int fd = open_file(dir3, fName);
        if(fd==-1)
            continue;
        char str[] = "!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!!-----------------------64 Bytes of Data-----------------------!";
        emufs_write(fd, str, 64*4);
        emufs_write(fd, str, 64*4);
        emufs_write(fd, str, 64*4);
        emufs_write(fd, str, 64*4);
    }
    fsdump(mnt1);

    change_dir(dir3,"/");
    emufs_delete(dir3,"dir1");
    emufs_delete(dir3,"dir3");
    fsdump(mnt1);

    return 0;
}