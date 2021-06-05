# xserver

a simple http server written in c language.

It is a freshman task of @UniqueStudio in 2021 spring. I write it as a demo because I'm the task publisher.

tool chain:

- [cJSON](https://github.com/DaveGamble/cJSON)

> install cjson locally.(by following `make install` step in cjson readme)

`run install` will install cjson lib at `/usr/local/lib`. append it in file `/etc/ld.so.conf`. else program will fail with error `error while loading shared libraries: libcjson.so.1: cannot open shared object file: No such file or directory`.

then run command `/sbin/ldconfig` on root permission to refresh it.
