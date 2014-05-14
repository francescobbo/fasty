fasty
=====

Fasty is a set of tools and libraries for Web Applications. It is written in C++ in order to achieve maximum performance.

Compilation
===========

Fasty requires OpenSSL 1.0.2-beta1 to work as [ALPN](http://tools.ietf.org/html/draft-friedl-tls-applayerprotoneg-02) over TLS is required for HTTP/2. Notice that that particular version of OpenSSL may still be vulnerable to the Heartbleed bug. So you may apply the patch yourself, wait for a more mature version to came, or just take the risk (not recommended).

OpenSSL should be installed into /usr/local/ssl, which is the default for the library, so that this will not impact on the official library distributed with your OS.

Just clone the Git repository and run `make`.
