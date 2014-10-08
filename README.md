status-nginx-module
===================

The req status module for pure nginx, patched with received counter, which is tested in nginx-1.4.7

Description
===========

This module will help monitor running status of Nginx.
* It can provide running status information of Nginx.
* The information is divided into different zones, and each zone is independent.
* The status information is about connections, requests, response status codes, input and output flows, rt, and upstreams.
* It shows all the results by default, and can be set to show part of them by specifying zones.

Compilation
===========

```
git clone https://github.com/hnlq715/status-nginx-module
./configure --add-module=/path/to/status-nginx-module
make && make install
```

Example
=======

```
http {
    ysec_status_zone host "$host,$server_addr:$server_port" 10M;

    server {
        location /us {
            ysec_status_show;
        }

        ysec_status host;
    }
}
```

When you call '/us', you will get the results like this:
```
localhost,127.0.0.1:80,162,6242,1,1,1,0,0,0,0,10,1,10,1
```

- Line Format:
  * kv value of the variable defined by the directive 'reqstatuszone'
  * bytesintotal total number of bytes received from client
  * bytesouttotal total number of bytes sent to client
  * conn_total total number of accepted connections
  * req_total total number of processed requests
  * 2xx total number of 2xx requests
  * 3xx total number of 3xx requests
  * 4xx total number of 4xx requests
  * 5xx total number of 5xx requests
  * other total number of other requests
  * rt_total accumulation or rt
  * upstream_req total number of requests calling for upstream
  * upstream_rt accumulation or upstream rt
  * upstream_tries total number of times calling for upstream


