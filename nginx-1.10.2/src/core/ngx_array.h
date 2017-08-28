
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 nginx上分配内存并非是原生的申请内存、也是要放在nginx的内存池里申请、
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

//数组的结构
typedef struct {
    void        *elts; //数据指针、也就是每个元素
    ngx_uint_t   nelts;//大小、数组的实际大小
    size_t       size;//一个块的大小、数组单元
    ngx_uint_t   nalloc;//临界大小
    ngx_pool_t  *pool;//内存池
} ngx_array_t;


ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
void ngx_array_destroy(ngx_array_t *a);
void *ngx_array_push(ngx_array_t *a);
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


//初始化数组
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;//一个块的大小
    array->nalloc = n; //数组的大小
    array->pool = pool;//分配这块数组的内存块

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
