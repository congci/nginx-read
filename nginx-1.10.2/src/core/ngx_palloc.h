
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1) //ngx_pagesize的大小在操作系统中为4095 4095-1

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024) //内存块的默认大小

#define NGX_POOL_ALIGNMENT       16 //对齐按照什么来、16字节
#define NGX_MIN_POOL_SIZE    //内存池的最小大小                                                 \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT) //ngx_pool_t结构体的大小 + 两倍的大内存的大小




typedef void (*ngx_pool_cleanup_pt)(void *data); //回调函数ngx_pool_cleanup_pt

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;//函数
    void                 *data;//这个是数据的
    ngx_pool_cleanup_t   *next; //下一个 链表
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
}; //大内存的结构



typedef struct {
    u_char               *last; //最后的地址 下一次分配内存从这里分配
    u_char               *end; //此块内存的最后
    ngx_pool_t           *next; //下一块内存的
    ngx_uint_t            failed;
} ngx_pool_data_t; 


struct ngx_pool_s {
    ngx_pool_data_t       d; //内存抽象结构体
    size_t                max; //内存池大小
    ngx_pool_t           *current;//指向当前内存池
    ngx_chain_t          *chain; 
    ngx_pool_large_t     *large; //大块内存池的指针
    ngx_pool_cleanup_t   *cleanup;//清理内存的指针
    ngx_log_t            *log;//日志模块
};//内存池的结构


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
