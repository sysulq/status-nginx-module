#vi:filetype=perl

use lib 'lib';
use Test::Nginx::Socket;

repeat_each(3);

plan tests => repeat_each() * (blocks() * 1 + 1);
no_root_location();
no_long_string();
$ENV{TEST_NGINX_SERVROOT} = server_root();
run_tests();

__DATA__
=== TEST 1: Basic GET request

--- http_config
http_status_zone host "$host,$server_addr:$server_port" 10M;

--- config
http_status host;
location / {
  root html;
  index index.html index.htm;
}
location /t {
    http_status_show;
}
--- request
GET /t
--- error_code: 200
--- response_body_like
\s?|localhost,127.0.0.1:1984,55,129,1,1,1,0,0,0,0,0,0,0,0|localhost,127.0.0.1:1984,110,318,2,2,2,0,0,0,0,0,0,0,0
