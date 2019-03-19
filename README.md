status-nginx-module
===================
![](https://travis-ci.org/hnlq715/status-nginx-module.svg)

The req status module for pure nginx. The core source file comes from Tengine, which is developed and maintained by Alibaba.


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

Starting from NGINX 1.9.11, you can also compile this module as a dynamic
module, by using the `--add-dynamic-module=PATH` option instead of
`--add-module=PATH` on the ./configure command line above. And then you can
explicitly load the module in your `nginx.conf` via the *load\_module* directive.


Configuration Directives
========================

http\_status\_zone
------------------
**syntax:** *http\_status\_zone &lt;zone\_name&gt; &lt;row\_name&gt; &lt;shm\_size&gt;* \
**default:** *no* \
**context:** *http*

Sets up shared memory zone named *zone_name* for statistics. Statistics are
simple table where first column is key and its value is computed by evaluating
expression *row\_name*. Remaining columns are collected metrics (see
[below](#format-of-statistics-output)).
*shm_size* specifies reserved memory for the statistics, each row will consume
about 200 bytes of memory (this is rough estimation, see source code for exact
value). If request should create new statistics row and there is no more memory
in zone, then it is not counted in this zone and appropriate warning is logged.

http\_status
------------
**syntax:** *http\_status &lt;zone\_name&gt; [&lt;zone\_name&gt; ...];* \
**default:** *no* \
**context:** *http*, *server*, *location*

Takes list of zones (at least one) where request should be counted.

http\_status\_bypass
---------------------
**syntax:** *http\_status\_bypass &lt;condition&gt; [&lt;condition&gt; ...];* \
**default:** *no* \
**context:** *http*, *server*, *location*

Defines conditions under which request will not be counted in statistics at
all.  If at least one value of the string parameters is not empty and is not
equal to “0” then the request will not be counted.

http\_status\_show
------------
**syntax:** *http\_status\_show [&lt;zone\_name&gt; [&lt;zone\_name&gt; ...]];* \
**default:** *no* \
**context:** *location*

Renders statistics as a response to the request matching current location.
No Content-Type response header is set.


Variables
=========

$upstream\_first\_addr
----------------------

This module defines variable *$upstream\_first\_addr* which contains
address of first upstream that was contacted when handling current request.


Format of statistics output
===========================
Statistics are formatted as comma separated values (without quoting, thus you
can create more virtual columns by having commas in *row\_name*). No header row
is rendered.

  * **row\_name** - value of expression defined by the directive `http_status_zone`
  * **bytesintotal** - total number of bytes received from client
  * **bytesouttotal** - total number of bytes sent to client
  * **conn\_total** - total number of accepted connections
  * **req\_total** - total number of processed requests
  * **2xx** - total number of 2xx requests
  * **3xx** - total number of 3xx requests
  * **4xx** - total number of 4xx requests
  * **5xx** - total number of 5xx requests
  * **other total** - number of other requests
  * **rt\_total** - accumulated request time (miliseconds)
  * **upstream\_req** - total number of requests calling for upstream
  * **upstream\_rt** - accumulated time of upstream calls (miliseconds)
  * **upstream\_tries** - total number of calls for upstream


Example
=======

```
http {
    # Set up statistics zone named 'host'.
    # ( Name of shared memory zone cannot be used in other
    #   places using named shared memory zones (proxy_cache_path ... keys_zone=..., ). )
    http_status_zone host "$host,$server_addr:$server_port" 10M;

    # Prepare variable for statistics with custom differrentiation.
    # ( We cannot use 'set' in 'http' block, so we use map and set it to "$server_name" by default,
    #   with mapping of server_name '_' to "unnamed_server" );
    map "$server_name" $custom_stat_name {
        default "$server_name";
        _ unnamed_server;
    }
    http_status_zone custom_stats "_CUSTOM,$custom_stat_name" 10M;

    # Count served pages only in zone 'host' by default
    http_status host;


    server {
        server_name _;

        location /all-stats {
            #Display rows from all zones together
            http_status_show;
        }

        location /host-stats {
            #Display rows only from 'host' zone;
            http_status_show host;
        }

        location /all-stats-ex {
            #Display rows from both zones (defined explicitly for configuration demonstration)
            http_status_show host custom_stats;

            #Override custom_stats_name when counting accesses to this location
            set $custom_stat_name "all stats explicit";
        }

        # Count all accesses to this server in both statistics zones.
        http_status host custom_stats;
    }
}
```

When you call '/all-stats', you will get results like this:
```
localhost,127.0.0.1:80,162,6242,1,1,1,0,0,0,0,10,1,10,1
_CUSTOM,unnamed_server,6242,1,1,1,0,0,0,0,10,1,10,1
```
