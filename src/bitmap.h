/*************************************************************************
  > File Name: bitmap.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Fri 23 Jul 2021 04:36:11 PM CST
 ************************************************************************/

#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
 #include <string.h>
#define BITMAP_WORD 32
#define BITMAP_SHIFT 5
#define BITMAP_MASK 0x1F
struct bitmap *bitmap_new(uint32_t count, uint32_t start)
{
  struct bitmap *bmap = calloc(1, sizeof(struct bitmap));
  assert(bmap != NULL);
  bitmap_init(bmap,count,start);
  return bmap;
}
int bitmap_init(struct bitmap *bmap, uint32_t count, uint32_t start)
{
  if (bmap != NULL)
  {
    assert(bmap != NULL);
    size_t alloc = (count%BITMAP_WORD==0)? (count/BITMAP_WORD):(1 + count/BITMAP_WORD);
    bmap->bits = malloc(alloc*sizeof(uint32_t));
    assert(bmap->bits != NULL);
    memset(bmap->bits,0,alloc*sizeof(uint32_t));
    bmap->count = count;
    bmap->start = start;
    return 0;
  }
  return -1;
}
int bitmap_set(struct bitmap *bmap, uint32_t val)
{
  if (bmap == NULL)
  {
    return -1;
  }
  if (val < bmap->start || val > (bmap->count + bmap->start))
  {
    return -1;
  }
  uint32_t left = val - bmap->start;
  uint32_t byte_position = left >> BITMAP_SHIFT;
  uint32_t bit_position = left & BITMAP_MASK;
  bmap->bits[byte_position] = bmap->bits[byte_position] | (1 << bit_position);
  return 0;
}
int bitmap_reset(struct bitmap *bmap, uint32_t val)
{
  if (bmap == NULL)
  {
    return -1;
  }
  if (val < bmap->start || val > (bmap->count + bmap->start))
  {
    return -1;
  }
  uint32_t left = val - bmap->start;
  uint32_t byte_position = left >> BITMAP_SHIFT;
  uint32_t bit_position = left & BITMAP_MASK;
  bmap->bits[byte_position] = bmap->bits[byte_position] & ~(1 << bit_position);
  return 0;
}
int bitmap_get(struct bitmap *bmap, uint32_t val)
{
  if (bmap == NULL)
  {
    return 0;
  }
  if (val < bmap->start || val > (bmap->count + bmap->start))
  {
    return 0;
  }
  uint32_t left = val - bmap->start;
  uint32_t byte_position = left >> BITMAP_SHIFT;
  uint32_t bit_position = left & BITMAP_MASK;
  return bmap->bits[byte_position] & (1 << bit_position);
}
void bitmap_deinit(struct bitmap *bmap) {
  if(bmap!=NULL)
  {
    free(bmap->bits);
    bmap->bits=NULL;
    bmap->count = 0;
    bmap->start = 0;
  }
}
void bitmap_destroy(struct bitmap *bmap) {
  bitmap_deinit(bmap);
  if(bmap!=NULL)
  {
    free(bmap);
    bmap = NULL;
  }
}
#ifdef BITMAP_TEST
#include <stdlib.h>
int main(int argc,char *argv[]) {
  int n = atoi(argv[1]);
  int *arr =calloc(n,sizeof(int));
  struct bitmap *bmap = bitmap_new(1024,0);
  int i=0;
  for(;i<n;i++)
  {
    arr[i] = (rand()+i)%1024;
    fprintf(stdout,"bitmap_set(%d)=%d\n",arr[i],bitmap_set(bmap,arr[i]));
  }
  for(i=0;i<n;i++) {
       fprintf(stdout,"bitmap_get(%d)=%d\n",arr[i],bitmap_get(bmap,arr[i]));
  }
  fprintf(stdout,"******************\n");
    for(i=0;i<n*3;i++) {
       fprintf(stdout,"bitmap_get(%i)=%d\n",i,bitmap_get(bmap,i));
  }
  bitmap_destroy(bmap);
}
#endif
