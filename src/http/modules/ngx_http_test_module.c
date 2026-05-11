
/*
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_flag_t  enable;
} ngx_http_test_module_conf_t;


static ngx_int_t ngx_http_test_module_init(ngx_cycle_t *cycle);
static void *ngx_http_test_module_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_test_module_init_main_conf(ngx_conf_t *cf, void *conf);


static ngx_command_t  ngx_http_test_module_commands[] = {

    { ngx_string("test_module"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_MAIN_CONF_OFFSET,
      offsetof(ngx_http_test_module_conf_t, enable),
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_test_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */
    ngx_http_test_module_create_main_conf, /* create main configuration */
    ngx_http_test_module_init_main_conf,   /* init main configuration */
    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */
    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_test_module = {
    NGX_MODULE_V1,
    &ngx_http_test_module_ctx,             /* module context */
    ngx_http_test_module_commands,         /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    ngx_http_test_module_init,             /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static void *
ngx_http_test_module_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_test_module_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_test_module_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    conf->enable = NGX_CONF_UNSET;

    return conf;
}


static char *
ngx_http_test_module_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_test_module_conf_t  *tmcf = conf;

    if (tmcf->enable == NGX_CONF_UNSET) {
        tmcf->enable = 0;
    }

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_test_module_init(ngx_cycle_t *cycle)
{
    ngx_http_test_module_conf_t  *tmcf;

    tmcf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_test_module);

    if (tmcf && tmcf->enable) {
        ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "hello");
    }

    return NGX_OK;
}
