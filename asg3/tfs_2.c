// Aaron Bruner
// C16480080
// ECE-3320 - Intro to OS
// asg3.c

#include "tfs.h"


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

    unsigned int block = directory[file_descriptor].first_block;
    unsigned int nextBlock;

    directory[file_descriptor].status = UNUSED; // delete directory

    if (directory[file_descriptor].first_block != 0) {
        while (block != LAST_BLOCK) {
            nextBlock = file_allocation_table[block];
            file_allocation_table[block] = FREE;
            block = nextBlock;
        }
        file_allocation_table[block] = FREE;
        return(1);
    }
    return(0);
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

    if (directory[file_descriptor].first_block != 0 && tfs_check_fd_in_range(file_descriptor) && tfs_check_file_is_open(file_descriptor)) {
        blockCnt = (byte_count / BLOCK_SIZE);

        if (blockCnt != 0) blockCnt -= 1;

        i = 0;
        j = directory[file_descriptor].first_block;
        while(i < blockCnt) {
            j = file_allocation_table[j];
            i++;
        }

        block = j;

        while (byteSize < byte_count) {
            if (byte % BLOCK_SIZE == 0) {
                block = file_allocation_table[block];
                if (block == LAST_BLOCK) break;
            }
            buffer[byteSize] = blocks[block].bytes[byte];

            byteSize++; byte++;
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

    if (tfs_check_fd_in_range(file_descriptor) && tfs_check_file_is_open(file_descriptor) && tfs_check_block_in_range(tfs_new_block())) {
        if (directory[file_descriptor].first_block == 0) {
            directory[file_descriptor].first_block = tfs_new_block();
            block = directory[file_descriptor].first_block;

            file_allocation_table[block] = LAST_BLOCK;
        } else {
            unsigned int k = directory[file_descriptor].first_block;
            while(file_allocation_table[k] != LAST_BLOCK){
                k++;
            }
            block = k;
        }
        while (byteWritten < byte_count) {
            if ((byteWritten != 0) && ((write % BLOCK_SIZE) == 0)) {
                blockPrev = block;
                file_allocation_table[blockPrev] = tfs_new_block();
                block = file_allocation_table[blockPrev];
                if (!tfs_check_block_in_range(block)) break;
                file_allocation_table[block] = LAST_BLOCK;
            }

            blocks[block].bytes[write % (BLOCK_SIZE)] = buffer[byteWritten];

            byteWritten++; write++;
        }

        file_allocation_table[block] = LAST_BLOCK;

        directory[file_descriptor].byte_offset = write % BLOCK_SIZE;
        directory[file_descriptor].size += byteWritten;
    }
    return byteWritten;
}
