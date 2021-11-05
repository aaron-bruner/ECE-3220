#include "tfs.h"
#include <stdlib.h>

/* helper function declarations */
unsigned int findLastBlock(unsigned int fd);

/* implementation of assigned functions */

/* tfs_delete()
 * -deletes a closed directory entry having the given file descriptor (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 * -postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 * -input parameter is a file descriptor
 * -return value is TRUE when successful or FALSE when failure
 */
unsigned int tfs_delete( unsigned int file_descriptor ){
  unsigned int fd = file_descriptor;
  unsigned int currBlock = directory[fd].first_block;

  //Delete directory entry (change status to unused)
  directory[fd].status = UNUSED;

  if(directory[fd].first_block != 0){
    //Release all allocated file blocks
    while(currBlock != LAST_BLOCK){
      unsigned int nextBlock = file_allocation_table[currBlock];
      //Change file_allocation_table to FREE (0) for all associated file blocks
      file_allocation_table[currBlock] = FREE;
      currBlock = nextBlock;
    }
    //Change file_allocation_table to FREE (0) for last block
    file_allocation_table[currBlock] = FREE;
    return(TRUE);
  }
  return(FALSE);
}

/* tfs_read()
 * -reads a specified number of bytes from a file starting at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is incremented by the number of bytes transferred
 * -depending on the starting byte offset and the specified number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 * -the function will read fewer bytes than specified if the end of the file is encountered before the specified number
 *   of bytes have been transferred
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 * -postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred except in the case that end of file was encountered
 *         before the transfer was complete
 * -input parameters are a file descriptor, the address of a buffer of bytes to transfer, and the count of bytes to
 *   transfer
 * -return value is the number of bytes transferred
 */
unsigned int tfs_read( unsigned int file_descriptor, char *buffer, unsigned int byte_count ){
  unsigned int fd = file_descriptor;
  unsigned int currBlock, numBlocks;
  unsigned int startReading = directory[fd].byte_offset % BLOCK_SIZE;
  unsigned int bytesRead = 0;
  unsigned int i, j;

  if(directory[fd].first_block != 0 && tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd)){
    //Figure out the number of blocks we're going to read
    numBlocks = (byte_count / BLOCK_SIZE);
    //If numBlocks = 0, then we don't need to move to any additional blocks
    if(numBlocks != 0){numBlocks -= 1;} //Must exclude first block

    //If necessary, iterate throught file_allocation_table until we reach the desired block
    i = 0;
    j = directory[fd].first_block;
    while(i < numBlocks){
      j = file_allocation_table[j];
      i++;
    }

    currBlock = j;

    //Read until bytesRead = byte_count
    while(bytesRead < byte_count){
      //If we've reached the end of a particular block....
      if(startReading % BLOCK_SIZE == 0){
        //Iterate to the next block
        currBlock = file_allocation_table[currBlock];
        //If EOF is reached, break
        if(currBlock == LAST_BLOCK){break;}
      }
      //This is the line where we actually read in from bytes[] to buffer[]
      buffer[bytesRead] = blocks[currBlock].bytes[startReading];

      bytesRead++;
      startReading++;
    }
  }
  return bytesRead;
}

/* tfs_write()
 * -writes a specified number of bytes from a specified buffer into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by the number of bytes transferred
 * -depending on the starting byte offset and the specified number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 * -furthermore, depending on the starting byte offset and the specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed; in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the original size of the file
 * -the function will read fewer bytes than specified if file blocks are not available
 * -preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 * -postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate when transferred bytes extend beyond the previous
 *         end of the file
 * -input parameters are a file descriptor, the address of a buffer of bytes to transfer, and the count of bytes to
 *   transfer
 * -return value is the number of bytes transferred
 */
unsigned int tfs_write( unsigned int file_descriptor, char *buffer, unsigned int byte_count ){
  unsigned int fd = file_descriptor;
  unsigned int currBlock, lastBlock = 0;
  unsigned int bytesWritten = 0;
  unsigned int startWriting = directory[fd].byte_offset;

  if(tfs_check_fd_in_range(fd) && tfs_check_file_is_open(fd) && tfs_check_block_in_range(tfs_new_block())){
    //Locate next available block after init() has been called
    if(directory[fd].first_block == 0){
      directory[fd].first_block = tfs_new_block();
      currBlock = directory[fd].first_block;
      //Set file_allocation_table entry to last_block for now
      file_allocation_table[currBlock] = LAST_BLOCK;
    }
    else{ //Start writing wherever the previous write ended
      currBlock = findLastBlock(fd);
    }

    //Keep writing unless bytesWritten = byte_count or we run out of space
    while(bytesWritten < byte_count){
      if((bytesWritten != 0) && ((startWriting % BLOCK_SIZE) == 0)){ //Check if block is full
        //If so, acquire new block and update file_allocation_table
        lastBlock = currBlock;
        file_allocation_table[lastBlock] = tfs_new_block();
        currBlock = file_allocation_table[lastBlock];
        if(!tfs_check_block_in_range(currBlock)){break;} //Check that new block is in range
        //Set currBlock as last_block for now
        file_allocation_table[currBlock] = LAST_BLOCK;
      }

      //This is the line where we actually write into bytes[] from buffer[]
      blocks[currBlock].bytes[startWriting % (BLOCK_SIZE)] = buffer[bytesWritten];

      bytesWritten++;
      startWriting++;
    }

    //At this point, we're done writing so set currBlock = LAST_BLOCK
    file_allocation_table[currBlock] = LAST_BLOCK;

    //Update byte_offset by setting it to how many bytes were written into the last block
    directory[fd].byte_offset = bytesWritten % BLOCK_SIZE;
    //Increase file size by # of bytesWritten
    directory[fd].size += bytesWritten;
  }

  return bytesWritten;
}

/* helper functions */
//Function to find last block of a file that was written to
unsigned int findLastBlock(unsigned int fd){
  unsigned int i = directory[fd].first_block;
  while(file_allocation_table[i] != LAST_BLOCK){
    i++;
  }
  return i;
}
