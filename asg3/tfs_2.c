// Aaron Bruner & David Brown
// C16480080 & C32056809
// ECE-3320 - Intro to OS
// asg3.c

#include "tfs.h"


//If a file's read byte is set to 1, return 1
unsigned int file_is_readable(unsigned int file_descriptor) {
    if (!!((directory[file_descriptor].rwd << 5) & 0x80) == 1) { return 1; }
    //printf("*** No read permission for file %s\n", directory[file_descriptor].name);
    return 0;
}

//If a file's write byte is set to 1, return 1
unsigned int file_is_writable(unsigned int file_descriptor) {
    if (!!((directory[file_descriptor].rwd << 6) & 0x80) == 1) { return 1; }
    //printf("*** No write permission for file %s\n", directory[file_descriptor].name);
    return 0;
}

//If a file's write byte is set to 1, return 1
void file_make_readable(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char readperm = 4;//0000 0100
    directory[file_descriptor].rwd = temp | readperm;
}

//If a file's write byte is set to 1, return 1
void file_make_writable(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char writeperm = 2;//0000 0010
    directory[file_descriptor].rwd = temp | writeperm;
}

unsigned int file_is_deletable(unsigned int file_descriptor) {
    if (!!((directory[file_descriptor].rwd << 7) & 0x80) == 1) { return 1; }
    //printf("*** No delete permission for file %s\n", directory[file_descriptor].name);
    return 0;
}

void file_make_deleteable(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char delperm = 1;//0000 0001
    directory[file_descriptor].rwd = temp | delperm;
}

//Functions to remove permissions. Useful due to binary operations being used.
void file_revoke_read(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char readperm = 251;//1111 1011
    directory[file_descriptor].rwd = temp & readperm;
}

void file_revoke_write(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char writeperm = 253;//1111 1101
    directory[file_descriptor].rwd = temp & writeperm;
}

void file_revoke_delete(unsigned int file_descriptor) {
    unsigned char temp = directory[file_descriptor].rwd;
    unsigned char delperm = 254;//1111 1110
    directory[file_descriptor].rwd = temp & delperm;
}

/* implementation of assigned functions */

/* tfs_delete()
 *
 * deletes a closed directory entry having the given file descriptor
 *   (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 *
 * postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 *
 * input parameter is a file descriptor
 *
 * return value is TRUE when successful or FALSE when failure
 */
unsigned int tfs_delete(unsigned int file_descriptor) {
    if (tfs_check_fd_in_range(file_descriptor)) {
        //Check if file can be deleted
        if (!file_is_deletable(file_descriptor)) {
            printf("File %s cannot be deleted; lacking permissions!\n", directory[file_descriptor].name);
            return 0;
        }
    }
    unsigned int block = directory[file_descriptor].first_block;
    unsigned int nextBlock;

    directory[file_descriptor].status = UNUSED; // delete directory

    if (directory[file_descriptor].first_block != 0) {
        while (block != LAST_BLOCK) { // Set all file blocks file_allocation_table to FREE
            nextBlock = file_allocation_table[block];
            file_allocation_table[block] = FREE;
            block = nextBlock;
        }
        file_allocation_table[block] = FREE; // Change last block's file_allocation_table to FREE
        return (1);
    }
    return (0);
}

/* tfs_read()
 *
 * reads a specified number of bytes from a file starting
 *   at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is
 *   incremented by the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * the function will read fewer bytes than specified if the
 *   end of the file is encountered before the specified number
 *   of bytes have been transferred
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 *   (4) the file has read permissions
 *
 * postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred
 *         except in the case that end of file was encountered
 *         before the transfer was complete
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */
unsigned int tfs_read(unsigned int file_descriptor, char *buffer, unsigned int byte_count) {

    unsigned int i, j, block, blockCnt, byteSize = 0;
    unsigned int byte = directory[file_descriptor].byte_offset % BLOCK_SIZE;

    if (directory[file_descriptor].first_block != 0 && tfs_check_fd_in_range(file_descriptor) &&
        tfs_check_file_is_open(file_descriptor)) {
        //Check if the file has read permissions
        if (!file_is_readable(file_descriptor)) {
            printf("File %s cannot be read from; lacking permissions!\n", directory[file_descriptor].name);
            return 0;
        }
        blockCnt = (byte_count / BLOCK_SIZE); // Determine number of blocks

        if (blockCnt != 0) blockCnt -= 1; // Cannot include the first block

        i = 0;
        j = directory[file_descriptor].first_block;
        while (i < blockCnt) { // Find desired block
            j = file_allocation_table[j];
            i++;
        }

        block = j;
        while (byteSize < byte_count) { // Read until end of byte_count
            if (byte % BLOCK_SIZE == 0) {
                block = file_allocation_table[block];
                if (block == LAST_BLOCK) break; // EOF
            }
            buffer[byteSize] = blocks[block].bytes[byte];

            byteSize++;
            byte++;
        }
    }
    return byteSize;
}

/* tfs_write()
 *
 * writes a specified number of bytes from a specified buffer
 *   into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by
 *   the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * furthermore, depending on the starting byte offset and the
 *   specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed;
 *   in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the
 *   original size of the file
 *
 * the function will read fewer bytes than specified if file
 *   blocks are not available
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has write permissions
 *
 * postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred
 *         except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate
 *         when transferred bytes extend beyond the previous
 *         end of the file
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */
unsigned int tfs_write(unsigned int file_descriptor, char *buffer, unsigned int byte_count) {

    unsigned int block, blockPrev = 0;
    unsigned int byteWritten = 0, write = directory[file_descriptor].byte_offset;

    if (tfs_check_fd_in_range(file_descriptor) && tfs_check_file_is_open(file_descriptor) &&
        tfs_check_block_in_range(tfs_new_block())) {
        //Check if file can be written to.
        if (!file_is_writable(file_descriptor)) {
            printf("File %s cannot be written to; lacking permissions!\n", directory[file_descriptor].name);
            return 0;
        }
        if (directory[file_descriptor].first_block == 0) { // Find next block
            directory[file_descriptor].first_block = tfs_new_block();
            block = directory[file_descriptor].first_block;

            file_allocation_table[block] = LAST_BLOCK;
        } else { // Begin writing
            unsigned int k = directory[file_descriptor].first_block;
            while (file_allocation_table[k] != LAST_BLOCK) {
                k++;
            }
            block = k;
        }
        while (byteWritten < byte_count) { // Write until end of byte_count
            if ((byteWritten != 0) && ((write % BLOCK_SIZE) == 0)) {
                blockPrev = block;
                file_allocation_table[blockPrev] = tfs_new_block();
                block = file_allocation_table[blockPrev];
                if (!tfs_check_block_in_range(block)) break;
                file_allocation_table[block] = LAST_BLOCK;
            }

            blocks[block].bytes[write % (BLOCK_SIZE)] = buffer[byteWritten];

            byteWritten++;
            write++;
        }

        // House cleaning after we've finished writing
        file_allocation_table[block] = LAST_BLOCK;
        directory[file_descriptor].byte_offset = write % BLOCK_SIZE;
        directory[file_descriptor].size += byteWritten;
    }
    file_make_readable(file_descriptor);
    file_revoke_write(file_descriptor);
    file_revoke_delete(file_descriptor);
    return byteWritten;
}