
-openssl-linked  -plugin-sql-sqlite

OPENSSL_LIBS='-L/home/doudou/openssl-1.1.0a -lssl -lcrypto'  ./configure -static  -opensource -nomake examples -nomake tests -openssl  -plugin-sql-sqlite -xcb -system-xcb -qpa xcb -qt-xcb



OPENSSL_LIBS='-L/home/doudou/openssl-1.1.0a -lssl -lcrypto'  ./configure -static  -opensource -nomake examples -nomake tests -openssl  -plugin-sql-sqlite

