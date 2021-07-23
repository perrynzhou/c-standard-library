/*************************************************************************
   > File Name: main.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 四  7/22 18:11:00 2021
 ************************************************************************/
#include "list.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#define BLOCK_AVG_SIZE (64 * 1024 * 1024)
#define BLOCK_AVG_COUNT 16
#define INODE_TABLE_SIZE (40960)
static uint32_t extent_id;
static uint32_t block_id;
static uint32_t inode_id;
static list *l_extent;
static int fd_extent;
enum inode_type {
   FILE=0,
   DIR =1,
};
typedef struct inode
{
   uint32_t blk_id;
   uint8_t type;
   size_t size;
   uint32_t block_count;
   uint32_t offset;
   uint32_t *blocks;
} inode;
typedef struct inode_table
{
   uint32_t inode_size;
   uint32_t *inodes;
} inode_table;
typedef struct block
{
   uint32_t blk_id;
   size_t offset;
} block;

typedef struct extent
{
   uint32_t ext_id;
   uint32_t blk_count;
   inode_table table;
   list *inodes;
   block *blk;
} extent;

extent *extent_create()
{
   extent *ext = calloc(1, sizeof(extent));
   assert(ext != NULL);
   ext->ext_id = extent_id;
   __sync_fetch_and_add(&extent_id, 1);
   ext->blk_count = BLOCK_AVG_COUNT;
   memset(&ext->table, 0, sizeof(ext->table));
   ext->table->inode_size = INODE_TABLE_SIZE;
   int i = 0;
   ext->table->inodes = calloc(INODE_SIZE, sizeof(uint32_t));
   ext->table->inode_count = INODE_SIZE;
   ext->inodes = list_create(sizeof(inode), 0);
   ext->blk = calloc(BLOCK_AVG_COUNT, sizeof(block));

   char buffer[256] = {'\0'};
   char *ptr_buffer = (char *)&buffer;
   snprintf(ptr_buffer, 256, "%d", extent_id);
   fd_extent = open(ptr_buffer, O_RDWR | O_CREAT | O_TRUNC, 0755);
   // {inode_tables}{datablock}
   fallocate(fd_extent, FALLOC_FL_KEEP_SIZE, 0, BLOCK_AVG_SIZE * BLOCK_AVG_COUNT + sizeof(uint32_t) * INODE_TABLE_SIZE);
   lseek(fd_extent, sizeof(uint32_t) * INODE_TABLE_SIZE, SEEK_SET);
   for (i = 0; i < BLOCK_AVG_COUNT; i++)
   {
      lseek(fd_extent, ext->table->inode_size * sizeof(uint32_t) + i * BLOCK_AVG_SIZE, SEEK_SET);
      ext->blk[i].blk_id = block_id;
      ext->blk[i].offset = 0;
      ext->blk[i].total = BLOCK_AVG_SIZE;
      __sync_fetch_and_add(&block_id, 1);
   }
   ext->blk_count = BLOCK_AVG_COUNT;
   if (l_extent == NULL)
   {
      l_extent = list_create(sizeof(extent), 0);
   }
   list_node *node = list_node_create(ext, sizeof(ext));
   if
   list_push_back(l_extent, node);
   return ext;
}
int blob_put(extent *ext, inode *addr, const char *path)
{
    struct stat st;
   stat(path,&st);
   int i;
   block *blk = NULL;
   for(i=0;i<BLOCK_AVG_COUNT;i++) {
      if(sizeof(inode) < (BLOCK_AVG_SIZE-ext->blk[i].offset))
      {
         blk = &ext->blk[i];
         break;
      }
   }
   inode *in = calloc(1,sizeof(inode));
   in->type=FILE;
   if(S_ISDIR(&st)) {
      in->type=DIR;
   }
   in->blk_id = blk->blk_id;
   in->size = + st.st_size;
   in->offset = blk->offet ;
   blk->offset +=st.st_size;
  // int fd = open(path,O_WRONLY);
    __sync_fetch_and_add(&inode_id, 1);
}
size_t blob_get(extent *ext, inode *in)
{
}
int main(void)
{
   return 0;
}
