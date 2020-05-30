# Operative systems project

PROGETTO II LabSO1 - AA 2019-2020 -- 202343, 202369, 202351, 204679

Gruppo: `riccardo.parola@studenti.unitn.it - Variante "U"`

Componenti del gruppo:

``` plaintext
Riccardo Parola     202343  riccardo.parola@studenti.unitn.it    00000001  
Riccardo Peron      202369  riccardo.peron@studenti.unitn.it     00000002  
Marco Di Francesco  202351  marco.difrancesco@studenti.unitn.it  00000003  
Filippo Daniotti    204679  filippo.daniotti@unitn.it            00000004  
```

## How to use

Run `run.sh` to run all these commands in once.

## Docker commands

**Run** docker (if not enabled):

``` Docker
sudo systemctl start docker
```

**Build image** from Dockerfile (need to be run in the folder of the DockerFile):

- `tag` specifies name of the contaier:the version

``` Docker
docker image build --tag osproject:latest .
```

**Run container** from previously built image:

- `rm` deletes the container once closed
- `$(pwd)` mount volume in the local folder
- `/home` is the folder inside the docker instance

``` Docker
docker container run \
--name osproject \
--interactive \
--tty \
--rm \
--volume $(pwd):/home \
osproject:latest
```

**Attach** to a running container:

``` Docker
docker exec --interactive --tty osproject bash
```

## Compiler commands

Compile (inside the continer):

``` shell
make build
```

## How it works

![Graph](https://i.imgur.com/r1lvqIh.png)

### Main

To run **main** run `./bin/main`.  
It requires the files `./bin/analyzer` and `./bin/report` to exist.

It is used to manage Analyzer and Report.

### Report

To run **main** run `./bin/main`.  
It requires the file `./bin/analyzer` to exist.

It is used to run Analyzer and open a named pipe with it, than require analyzed data, than it will close analyzer than finish.

### Analyzer

To run **analyzer** run `./bin/analyzer`.  

It is used to create Counter and calulate which process P analyze which file. If Counter already exist, it will recalculate the processes P assigned files. Once this is completed it will close, it won't wait for Counter to close.

### Counter

Counter is run in [Analyzer](#Analyzer).

Counter is the only process (except Main) that won't be stopped by anything.  
When it's created, it will create all P processes.

It is used to gather all the information in a struct:

``` C
struct CounterStruct {
    string FileName = "";
    int ProcessP = -1;
    int Statistics[Cluster];
    CounterStruct *next = NULL;
}
```

### P

P is run in [Counter](#Counter).

When P is started, it will get the path of the file that it needs to analyze and it spowns the child processes Q and it waits for them to finish.

### Q

Q is run in [P](#P).

When Q is started, it will get the path of the file that it needs to analyze and which part. When the process finishes, it communicates the results to P.
