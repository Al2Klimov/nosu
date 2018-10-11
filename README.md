## About

**nosu** is not su(1). It switches the user and group
and exec(3)s a command, but without fork(2)ing.

## Rationale

su(1) executes the given command in a separate process and waits for it to exit:

```
root@206aece88a19:/# ps -eH
  PID TTY          TIME CMD
    6 pts/1    00:00:00 bash
   17 pts/1    00:00:00   ps
    1 pts/0    00:00:00 bash
   13 pts/0    00:00:00   su
   14 ?        00:00:00     sleep
root@206aece88a19:/#
```

nosu doesn't create any additional process:

```
root@206aece88a19:/# ps -eH
  PID TTY          TIME CMD
    6 pts/1    00:00:00 bash
   17 pts/1    00:00:00   ps
    1 pts/0    00:00:00 bash
   13 pts/0    00:00:00   sleep
root@206aece88a19:/#
```

### Use cases

* nosu saves some RAM and CPU cycles compared to su(1) -
  useful on low-end systems
* nosu saves some HDD space compared to su(1) - useful in [Docker] images
* there's no "barrier" between the initiator and the process -
  e.g. `sleep 10 & kill -9 $!` works as expected.

## Installation

### Build requirements

* cmake
* make
* gcc
* libc-dev

### Compilation

```
$ mkdir build
$ pushd build
$ cmake ..
$ make
# mv nosu /usr/local/bin/
```

## Usage

nosu must be run as root:

```
# nosu USER GROUP COMMAND [ARGUMENT...]
```

For the user's group specify the empty string.

### Usage in Docker images

```Dockerfile
COPY --from=grandmaster/nosu:latest /usr/local/bin/nosu /usr/local/bin/nosu
```

## Proof of concept

### Functionality

```
$ docker run -it grandmaster/nosu bash
root@8189bcbd304f:/# nosu nobody www-data id
uid=65534(nobody) gid=33(www-data) groups=33(www-data)
root@8189bcbd304f:/#
```

### Security

```
$ docker run -it grandmaster/nosu bash
root@721c52f28ab7:/# nosu nobody www-data nosu root root id
setgid(): Operation not permitted
root@721c52f28ab7:/#
```

[Docker]: https://www.docker.com/
