#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct ngx_ysec_status_rbnode_s ngx_ysec_status_rbnode_t;


struct ngx_ysec_status_rbnode_s {
    u_char                       color;
    u_char                       padding[3];
    uint32_t                     len;
    ngx_queue_t                  queue;
    ngx_atomic_t                 bytes_in;
    ngx_atomic_t                 bytes_out;
    ngx_atomic_t                 conn_total;
    ngx_atomic_t                 req_total;
    ngx_atomic_t                 http_2xx;
    ngx_atomic_t                 http_3xx;
    ngx_atomic_t                 http_4xx;
    ngx_atomic_t                 http_5xx;
    ngx_atomic_t                 other_status;
    ngx_atomic_t                 rt;
    ngx_atomic_t                 ureq;
    ngx_atomic_t                 urt;
    ngx_atomic_t                 utries;
    u_char                       data[1];
};


typedef struct {
    ngx_array_t                 *monitor;
    ngx_array_t                 *display;
    ngx_array_t                 *bypass;
} ngx_ysec_status_conf_t;


typedef struct {
    ngx_rbtree_t                 rbtree;
    ngx_rbtree_node_t            sentinel;
    ngx_queue_t                  queue;
} ngx_ysec_status_shctx_t;


typedef struct {
    ngx_str_t                   *val;
    ngx_slab_pool_t             *shpool;
    ngx_ysec_status_shctx_t    *sh;
    ngx_http_complex_value_t     value;
} ngx_ysec_status_ctx_t;


#define NGX_YSEC_STATUS_BYTES_IN                                       \
    offsetof(ngx_ysec_status_rbnode_t, bytes_in)

#define NGX_YSEC_STATUS_BYTES_OUT                                      \
    offsetof(ngx_ysec_status_rbnode_t, bytes_out)

#define NGX_YSEC_STATUS_CONN_TOTAL                                     \
    offsetof(ngx_ysec_status_rbnode_t, conn_total)

#define NGX_YSEC_STATUS_REQ_TOTAL                                      \
    offsetof(ngx_ysec_status_rbnode_t, req_total)

#define NGX_YSEC_STATUS_2XX                                            \
    offsetof(ngx_ysec_status_rbnode_t, http_2xx)

#define NGX_YSEC_STATUS_3XX                                            \
    offsetof(ngx_ysec_status_rbnode_t, http_3xx)

#define NGX_YSEC_STATUS_4XX                                            \
    offsetof(ngx_ysec_status_rbnode_t, http_4xx)

#define NGX_YSEC_STATUS_5XX                                            \
    offsetof(ngx_ysec_status_rbnode_t, http_5xx)

#define NGX_YSEC_STATUS_OTHER_STATUS                                   \
    offsetof(ngx_ysec_status_rbnode_t, other_status)

#define NGX_YSEC_STATUS_RT                                             \
    offsetof(ngx_ysec_status_rbnode_t, rt)

#define NGX_YSEC_STATUS_UPS_REQ                                        \
    offsetof(ngx_ysec_status_rbnode_t, ureq)

#define NGX_YSEC_STATUS_UPS_RT                                         \
    offsetof(ngx_ysec_status_rbnode_t, urt)

#define NGX_YSEC_STATUS_UPS_TRIES                                      \
    offsetof(ngx_ysec_status_rbnode_t, utries)

#define REQ_FIELD(node, offset)                                         \
    ((ngx_atomic_t *) ((char *) node + offset))
