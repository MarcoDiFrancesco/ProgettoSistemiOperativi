# Operative systems project

## Commands

Run docker:

``` Docker
sudo systemctl start docker
```

Build container:

``` Docker
docker build -t osproject:latest .
```

Run container:

``` Docker
docker container run --volume /home/marco/projects/OperativeSystems:/home -it osproject:latest
```

Compile C:

``` C
gcc -std=gnu90 -o /tmp/a.out processes.c && /tmp/a.out
```
