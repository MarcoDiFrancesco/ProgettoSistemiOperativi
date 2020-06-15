# Operative systems project

PROGETTO II LabSO1 - AA 2019-2020 -- 202343, 202369, 202351, 204679

Gruppo: `riccardo.parola@studenti.unitn.it - Variante "U"`

Componenti del gruppo:

``` plaintext
Riccardo Parola     202343  riccardo.parola@studenti.unitn.it    00000001  
Riccardo Peron      202369  riccardo.peron@studenti.unitn.it     00000002  
Marco Di Francesco  202351  marco.difrancesco@studenti.unitn.it  00000003  
Filippo Daniotti    204679  filippo.daniotti@studenti.unitn.it   00000004  
```

## How to use

Run `run.sh` to run docker using the DockerFile (a kind of docker-compose).

If you don't want to run `run.sh` use the docker [comamnds below](#docker-commands).

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
- `/root` is the folder inside the docker instance

``` Docker
docker container run \
--name osproject \
--interactive \
--tty \
--rm \
--volume $(pwd):/root \
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

Clean (inside the continer):

``` shell
make clean
```

## How it works

![Graph](https://i.imgur.com/7D2PV6L.png)

It's possible to run the program in two ways:

- Using main binary (`/root/bin/main`)
- Running the program Analyzer (`/root/bin/analyzer`), then running Report (`/root/bin/report`)

The files that will be analyzed are `.txt`, `.cpp`, `.c`, `.java` and `.py`.

### Main

To run **main** run `/root/bin/main`.  
It requires the files `/root/bin/analyzer` and `/root/bin/report` to exist.

It is used to manage Analyzer and Report.

### Report

To run **main** run `/root/bin/main`.  
It requires the file `/root/bin/analyzer` to exist.

It is used to run Analyzer and open a named pipe with it, than require analyzed data, than it will close analyzer than finish.

### Analyzer

To run **analyzer** run `/root/bin/analyzer`.  

It is used to create Counter and calulate which process P analyze which file. If Counter already exist, it will recalculate the processes P assigned files. Once this is completed it will close, it won't wait for Counter to close.

### Counter

Counter is run in [Analyzer](#Analyzer).

Is the process that manage and starts P and Q processes, based on the values N and M, gathers informations from theyir analysis and sends them trought messages to the process [Report](#Report).

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

When P is started, it will get the path of the file that it needs to analyze and it spowns the all the child processes Q.

### Q

Q is run in [P](#P).

When Q is started, it will get the path of the file that it needs to analyze and which part. When the process finishes, it communicates the results to P.

### Limitations

This program has limits that we couldn't solve in time:

- Max analyzable files is 500 (limited by the message to [Report](#Report))
- Max analyzable files in [Counter](#Counter) is 3077
- You can only add files on the fly but it's not possible to add folder once the program is started
- Max lenght of the analyzable path is 48 characters
- Max number of total process is ( N = 15 ) and ( N + M*N <= 150)
- There is one `sleep()` in the [P](#P) process to slow down the control of the termination of all the [Q](#Q) process
- It's possible to run the programs only if they're placed under `/root/bin/`
