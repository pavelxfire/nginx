#!/usr/bin/perl

# (C) Nginx, Inc.

# Tests for http test module.
# Requires nginx-tests (https://github.com/nginx/nginx-tests.git).
# Usage:
#   TEST_NGINX_BINARY=/path/to/nginx prove http_test_module.t

###############################################################################

use warnings;
use strict;

use Test::More;

BEGIN { use FindBin; chdir($FindBin::Bin); }

use lib 'lib';
use Test::Nginx;

###############################################################################

select STDERR; $| = 1;
select STDOUT; $| = 1;

my $t = Test::Nginx->new()->has(qw/http/);

$t->plan(3)->write_file_expand('nginx.conf', <<'EOF');

%%TEST_GLOBALS%%

daemon off;

events {
}

http {
    %%TEST_GLOBALS_HTTP%%

    test_module on;

    server {
        listen       127.0.0.1:8080;
        server_name  localhost;
        location / { }
    }
}

EOF

$t->run();

http_get('/');

$t->stop();

like($t->read_file('error.log'), qr/\[notice\] .* hello/,
    'test_module on logs hello');

###############################################################################

# clear error log and test with off

$t->write_file('error.log', '');
$t->write_file_expand('nginx.conf', <<'EOF');

%%TEST_GLOBALS%%

daemon off;

events {
}

http {
    %%TEST_GLOBALS_HTTP%%

    test_module off;

    server {
        listen       127.0.0.1:8080;
        server_name  localhost;
        location / { }
    }
}

EOF

$t->run();

http_get('/');

$t->stop();

unlike($t->read_file('error.log'), qr/\[notice\] .* hello/,
    'test_module off does not log hello');

###############################################################################

# clear error log and test with default (no directive)

$t->write_file('error.log', '');
$t->write_file_expand('nginx.conf', <<'EOF');

%%TEST_GLOBALS%%

daemon off;

events {
}

http {
    %%TEST_GLOBALS_HTTP%%

    server {
        listen       127.0.0.1:8080;
        server_name  localhost;
        location / { }
    }
}

EOF

$t->run();

http_get('/');

$t->stop();

unlike($t->read_file('error.log'), qr/\[notice\] .* hello/,
    'test_module default does not log hello');

###############################################################################
