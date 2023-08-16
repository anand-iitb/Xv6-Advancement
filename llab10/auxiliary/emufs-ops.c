#include "emufs-disk.h"
#include "emufs.h"

/* ------------------- In-Memory objects ------------------- */

int init = 0; // if the file/directory handles arrays are initialized or not

struct file_t
{
    int offset;       // offset of the file
    int inode_number; // inode number of the file in the disk
    int mount_point;  // reference to mount point
                      // -1: Free
                      // >0: In Use
};

struct directory_t
{
    int inode_number; // inode number of the directory in the disk
    int mount_point;  // reference to mount point
                      // -1: Free
                      // >0: In Use
};

struct directory_t dir[MAX_DIR_HANDLES]; // array of directory handles
struct file_t files[MAX_FILE_HANDLES];   // array of file handles

int closedevice(int mount_point)
{
    /*
        * Close all the associated handles
        * Unmount the device

        * Return value: -1,     error
                         1,     success
    */

    for (int i = 0; i < MAX_DIR_HANDLES; i++)
        dir[i].mount_point = (dir[i].mount_point == mount_point ? -1 : dir[i].mount_point);
    for (int i = 0; i < MAX_FILE_HANDLES; i++)
        files[i].mount_point = (files[i].mount_point == mount_point ? -1 : files[i].mount_point);

    return closedevice_(mount_point);
}

int create_file_system(int mount_point, int fs_number)
{
    /*
        * Read the superblock.
        * Update the mount point with the file system number
        * Set file system number on superblock
        * Clear the bitmaps.  values on the bitmap will be either '0', or '1'.
        * Update the used inodes and blocks
        * Create Inode 0 (root) in metadata block in disk
        * Write superblock and metadata block back to disk.

        * Return value: -1,		error
                         1, 	success
    */
    struct superblock_t superblock;
    read_superblock(mount_point, &superblock);

    update_mount(mount_point, fs_number);

    superblock.fs_number = fs_number;
    for (int i = 3; i < MAX_BLOCKS; i++)
        superblock.block_bitmap[i] = 0;
    for (int i = 0; i < 3; i++)
        superblock.block_bitmap[i] = 1;
    for (int i = 1; i < MAX_INODES; i++)
        superblock.inode_bitmap[i] = 0;
    superblock.inode_bitmap[0] = 1;
    superblock.used_blocks = 3;
    superblock.used_inodes = 1;
    write_superblock(mount_point, &superblock);

    struct inode_t inode;
    memset(&inode, 0, sizeof(struct inode_t));
    inode.name[0] = '/';
    inode.parent = 255;
    inode.type = 1;
    write_inode(mount_point, 0, &inode);
}

int alloc_dir_handle()
{
    /*
        * Initialize the arrays if not already done
        * check and return if there is any free entry

        * Return value: -1,		error
                         1, 	success
    */
    if (init == 0)
    {
        for (int i = 0; i < MAX_DIR_HANDLES; i++)
            dir[i].mount_point = -1;
        for (int i = 0; i < MAX_FILE_HANDLES; i++)
            files[i].mount_point = -1;
        init = 1;
    }
    for (int i = 0; i < MAX_DIR_HANDLES; i++)
        if (dir[i].mount_point == -1)
            return i;
    return -1;
}

int alloc_file_handle()
{
    for (int i = 0; i < MAX_FILE_HANDLES; i++)
        if (files[i].mount_point == -1)
            return i;
    return -1;
}

int goto_parent(int dir_handle)
{
    /*
        * Update the dir_handle to point to the parent directory

        * Return value: -1,		error   (If the current directory is root)
                         1, 	success
    */

    struct inode_t inode;
    read_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, &inode);
    if (inode.parent == 255)
        return -1;
    dir[dir_handle].inode_number = inode.parent;
    return 1;
}

int open_root(int mount_point)
{
    /*
        * Open a directory handle pointing to the root directory of the mount

        * Return value: -1,		            error (no free handles)
                         directory handle, 	success
    */
    int dir_handle = alloc_dir_handle();
    if (dir_handle == -1)
        return -1;
    dir[dir_handle].mount_point = mount_point;
    dir[dir_handle].inode_number = 0;
    return dir_handle;
}

int return_inode(int mount_point, int inodenum, char *path)
{
    /*
        * Parse the path
        * Search the directory to find the matching entity

        * Return value: -1,		        error
                         inode number, 	success
    */

    // start from root directory
    if (path[0] == '/')
        inodenum = 0;

    struct inode_t inode;
    read_inode(mount_point, inodenum, &inode);

    // the directory to start with is not a directory
    if (inode.type == 0)
        return -1;

    int ptr1 = 0, ptr2 = 0;
    char buf[MAX_ENTITY_NAME];
    memset(buf, 0, MAX_ENTITY_NAME);

    while (path[ptr1])
    {
        if (path[ptr1] == '/')
        {
            ptr1++;
            continue;
        }
        if (path[ptr1] == '.')
        {
            ptr1++;
            if (path[ptr1] == '/' || path[ptr1] == 0)
                continue;
            if (path[ptr1] == '.')
            {
                ptr1++;
                if (path[ptr1] == '/' || path[ptr1] == 0)
                {
                    if (inodenum == 0)
                        return -1;
                    inodenum = inode.parent;
                    read_inode(mount_point, inodenum, &inode);
                    continue;
                }
            }
            return -1;
        }
        while (1)
        {
            int found = 0;
            buf[ptr2++] = path[ptr1++];
            if (path[ptr1] == 0 || path[ptr1] == '/')
            {
                for (int i = 0; i < inode.size; i++)
                {
                    struct inode_t entry;
                    read_inode(mount_point, inode.mappings[i], &entry);
                    if (memcmp(buf, entry.name, MAX_ENTITY_NAME) == 0)
                    {
                        inodenum = inode.mappings[i];
                        inode = entry;
                        if (path[ptr1] == '/')
                            if (entry.type == 0)
                                return -1;
                        ptr2 = 0;
                        memset(buf, 0, MAX_ENTITY_NAME);
                        found = 1;
                        break;
                    }
                }
                if (found)
                    break;
                return -1;
            }
            if (ptr2 == MAX_ENTITY_NAME)
                return -1;
        }
    }
    return inodenum;
}

int change_dir(int dir_handle, char *path)
{
    /*
        * Update the handle to point to the directory denoted by path
        * You should use return_inode function to get the required inode

        * Return value: -1,		error
                         1, 	success
    */
    int inodenum = return_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, path);
    if (inodenum == -1)
        return -1;
    dir[dir_handle].inode_number = inodenum;
    return 1;
}

void emufs_close(int handle, int type)
{
    /*
     * type = 1 : Directory handle and 0 : File Handle
     * Close the file/directory handle
     */
    if (type == 1)
        dir[handle].mount_point = -1;
    else
        files[handle].mount_point = -1;
}

int delete_entity(int mount_point, int inodenum)
{
    /*
        * Delete the entity denoted by inodenum (inode number)
        * Close all the handles associated
        * If its a file then free all the allocated blocks
        * If its a directory call delete_entity on all the entities present
        * Free the inode

        * Return value : inode number of the parent directory
    */

    struct inode_t inode;
    read_inode(mount_point, inodenum, &inode);
    if (inode.type == 0)
    {
        for (int i = 0; i < MAX_FILE_HANDLES; i++)
            if (files[i].inode_number == inodenum)
                files[i].mount_point = -1;
        int num_blocks = inode.size / BLOCKSIZE;
        if (num_blocks * BLOCKSIZE < inode.size)
            num_blocks++;
        for (int i = 0; i < num_blocks; i++)
            free_datablock(mount_point, inode.mappings[i]);
        free_inode(mount_point, inodenum);
        return inode.parent;
    }

    for (int i = 0; i < MAX_DIR_HANDLES; i++)
        if (dir[i].inode_number == inodenum)
            dir[i].mount_point = -1;

    for (int i = 0; i < inode.size; i++)
        delete_entity(mount_point, inode.mappings[i]);
    free_inode(mount_point, inodenum);
    return inode.parent;
}

int emufs_delete(int dir_handle, char *path)
{
    /*
        * Delete the entity at the path
        * Use return_inode and delete_entry functions for searching and deleting entities
        * Update the parent root of the entity to mark the deletion
        * Remove the entity's inode number from the mappings array and decrease the size of the directory
        * For removing entity do the following :
            * Lets say we have to remove ith entry in mappings
            * We'll make mappings[j-1] = mappings[j] for all i < j < size
        * Then write the inode back to the disk

        * Return value: -1, error
                         1, success
    */
    int inodenum = return_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, path);
    if (inodenum == -1)
        return -1;
    int parent = delete_entity(dir[dir_handle].mount_point, inodenum);
    struct inode_t inode;
    read_inode(dir[dir_handle].mount_point, parent, &inode);
    for (int i = 0; i < inode.size; i++)
        if (inode.mappings[i] == inodenum)
        {
            for (int j = i; j < inode.size - 1; j++)
                inode.mappings[j] = inode.mappings[j + 1];
            inode.size--;
            break;
        }
    write_inode(dir[dir_handle].mount_point, parent, &inode);
    return 1;
}

int emufs_create(int dir_handle, char *name, int type)
{
    /*
        * Create a directory (type=1) / file (type=0) in the directory denoted by dir_handle
        * Check if a directory/file with the same name is present or not
        * Note that different entities with the same name work, like a file and directory of name 'foo' can exist simultaneously in a directory

        * Return value: -1, error
                         1, success
    */
    struct inode_t inode;
    read_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, &inode);
    if (inode.type == 0)
        return -1;
    for (int i = 0; i < inode.size; i++)
    {
        struct inode_t entry;
        read_inode(dir[dir_handle].mount_point, inode.mappings[i], &entry);
        if (memcmp(name, entry.name, MAX_ENTITY_NAME) == 0)
            return -1;
    }
    int inodenum = alloc_inode(dir[dir_handle].mount_point);
    if (inodenum == -1)
        return -1;
    read_inode(dir[dir_handle].mount_point, inodenum, &inode);
    memcpy(inode.name, name, MAX_ENTITY_NAME);
    inode.parent = dir[dir_handle].inode_number;
    write_inode(dir[dir_handle].mount_point, inodenum, &inode);
    inode.mappings[inode.size++] = inodenum;
    write_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, &inode);
    return 1;
}

int open_file(int dir_handle, char *path)
{
    /*
        * Open a file_handle to point to the file denoted by path
        * Get the inode using return_inode function
        * Get a file handle using alloc_file_handle
        * Initialize the file handle

        * Return value: -1, error
                         1, success
    */
    int inodenum = return_inode(dir[dir_handle].mount_point, dir[dir_handle].inode_number, path);
    if (inodenum == -1)
        return -1;
    int file_handle = alloc_file_handle();
    if (file_handle == -1)
        return -1;
    files[file_handle].mount_point = dir[dir_handle].mount_point;
    files[file_handle].inode_number = inodenum;
    files[file_handle].offset = 0;
    return file_handle;
}

int emufs_read(int file_handle, char *buf, int size)
{
    /*
        * Read the file into buf starting from seek(offset)
        * The size of the chunk to be read is given
        * size can and can't be a multiple of BLOCKSIZE
        * Update the offset = offset+size in the file handle (update the seek)
        * Hint:
            * Use a buffer of BLOCKSIZE and read the file blocks in it
            * Then use this buffer to populate buf (use memcpy)

        * Return value: -1, error
                         1, success
    */
    struct inode_t inode;
    read_inode(files[file_handle].mount_point, files[file_handle].inode_number, &inode);
    if (inode.type == 1)
        return -1;
    int num_blocks = inode.size / BLOCKSIZE;
    if (num_blocks * BLOCKSIZE < inode.size)
        num_blocks++;
    int offset = files[file_handle].offset;
    int block = offset / BLOCKSIZE;
    int block_offset = offset % BLOCKSIZE;
    int bytes_read = 0;
    while (bytes_read < size)
    {
        char buffer[BLOCKSIZE];
        read_datablock(files[file_handle].mount_point, inode.mappings[block], buffer);
        int bytes_to_read = BLOCKSIZE - block_offset;
        if (bytes_to_read > size - bytes_read)
            bytes_to_read = size - bytes_read;
        memcpy(buf + bytes_read, buffer + block_offset, bytes_to_read);
        bytes_read += bytes_to_read;
        block_offset = 0;
        block++;
    }
    files[file_handle].offset += size;
    return 1;
}

int emufs_write(int file_handle, char *buf, int size)
{
    /*
        * Write the memory buffer into file starting from seek(offset)
        * The size of the chunk to be written is given
        * size can and can't be a multiple of BLOCKSIZE
        * Update the inode of the file if need to be (mappings and size changed)
        * Update the offset = offset+size in the file handle (update the seek)
        * Hint:
            * Use a buffer of BLOCKSIZE and read the file blocks in it
            * Then write to this buffer from buf (use memcpy)
            * Then write back this buffer to the file

        * Return value: -1, error
                         1, success
    */
    struct inode_t inode;
    read_inode(files[file_handle].mount_point, files[file_handle].inode_number, &inode);
    if (inode.type == 1)
        return -1;
    int num_blocks = inode.size / BLOCKSIZE;
    if (num_blocks * BLOCKSIZE < inode.size)
        num_blocks++;
    int offset = files[file_handle].offset;
    int block = offset / BLOCKSIZE;
    int block_offset = offset % BLOCKSIZE;
    int bytes_written = 0;
    while (bytes_written < size)
    {
        char buffer[BLOCKSIZE];
        if (block < num_blocks)
            read_datablock(files[file_handle].mount_point, inode.mappings[block], buffer);
        else
        {
            int new_block = alloc_datablock(files[file_handle].mount_point);
            if (new_block == -1)
                return -1;
            inode.mappings[block] = new_block;
            inode.size += BLOCKSIZE;
            num_blocks++;
        }
        int bytes_to_write = BLOCKSIZE - block_offset;
        if (bytes_to_write > size - bytes_written)
            bytes_to_write = size - bytes_written;
        memcpy(buffer + block_offset, buf + bytes_written, bytes_to_write);
        write_datablock(files[file_handle].mount_point, inode.mappings[block], buffer);
        bytes_written += bytes_to_write;
        block_offset = 0;
        block++;
    }
    write_inode(files[file_handle].mount_point, files[file_handle].inode_number, &inode);
    files[file_handle].offset += size;
    return 1;
}

int emufs_seek(int file_handle, int nseek)
{
    /*
        * Update the seek(offset) of fie handle
        * Make sure its not negative and not exceeding the file size

        * Return value: -1, error
                         1, success
    */
    struct inode_t inode;
    read_inode(files[file_handle].mount_point, files[file_handle].inode_number, &inode);
    if (inode.type == 1)
        return -1;
    if (nseek < 0 || nseek > inode.size)
        return -1;
    files[file_handle].offset = nseek;
    return 1;

}

void flush_dir(int mount_point, int inodenum, int depth)
{
    /*
     * Print the directory structure of the device
     */

    struct inode_t inode;
    read_inode(mount_point, inodenum, &inode);

    for (int i = 0; i < depth - 1; i++)
        printf("|  ");
    if (depth)
        printf("|--");
    for (int i = 0; i < MAX_ENTITY_NAME && inode.name[i] > 0; i++)
        printf("%c", inode.name[i]);
    if (inode.type == 0)
        printf(" (%d bytes)\n", inode.size);
    else
    {
        printf("\n");
        for (int i = 0; i < inode.size; i++)
            flush_dir(mount_point, inode.mappings[i], depth + 1);
    }

}

void fsdump(int mount_point)
{
    /*
     * Prints the metadata of the file system
     */

    struct superblock_t superblock;
    read_superblock(mount_point, &superblock);
    printf("\n[%s] fsdump \n", superblock.device_name);
    flush_dir(mount_point, 0, 0);
    printf("Inodes in use: %d, Blocks in use: %d\n", superblock.used_inodes, superblock.used_blocks);
}