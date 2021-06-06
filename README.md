# xserver

a simple http server written in c language.

It is a freshman task of @UniqueStudio in 2021 spring. I write it as a demo because I'm the task publisher.

### How to use

1. install cjson (see [tool chain](###tool chain))
2. edit `conf.json` to modify config
3. `mkdir build && cd build && cmake .. && make`. The executable file is at `build/src/x-server`
4. the x-server binary file will get conf file path from environment variable `env_file_path` in release mode(default mode). please set `env_file_path` first before executing the binary file.

> you can remove command for `_DEBUG_CONFIG_PATH` in `src/conf/conf.h` and set the proper value of it to set project to debug mod. it will load file from the value you defined before.

### Project source code layout

```
.
├── CMakeLists.txt
├── conf.json
└── src
    ├── CMakeLists.txt
    ├── conf                    // read config from `conf.json`
    │   ├── CMakeLists.txt
    │   ├── conf.c
    │   └── conf.h
    ├── ctrl.h                  // global control def(TODO)
    ├── http
    │   ├── CMakeLists.txt
    │   ├── conn.c              // bind and listen connection, while managing them by epoll
    │   ├── conn.h
    │   ├── req.c               // parse http request and return proper response
    │   ├── req.h
    │   ├── resp.c              // specific http response implementation
    │   └── resp.h
    ├── log                     // uniform log handle(TODO)
    │   └── log.h
    └── main.c                  // project entry
```

---

### tool chain

- [cJSON](https://github.com/DaveGamble/cJSON)

> install cjson locally.(by following `make install` step in cjson readme)

`run install` will install cjson lib at `/usr/local/lib`. append it in file `/etc/ld.so.conf`. else program will fail with error `error while loading shared libraries: libcjson.so.1: cannot open shared object file: No such file or directory`.

then run command `/sbin/ldconfig` on root permission to refresh it.

### TODO list

- [ ] uniform log handle
- [ ] global control def management
- [ ] http big file body handle
