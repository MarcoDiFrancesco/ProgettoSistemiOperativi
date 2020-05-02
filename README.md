# Operative systems project

PROGETTO II LabSO1 - AA 2019-2020 -- 202343, 202369, 202351, 204679

Gruppo: riccardo.parola@studenti.unitn.it - Variante "U"

Componenti del gruppo:

``` plaintext
Riccardo Parola     202343  riccardo.parola@studenti.unitn.it    00000001  
Riccardo Peron      202369  riccardo.peron@studenti.unitn.it     00000002  
Marco Di Francesco  202351  marco.difrancesco@studenti.unitn.it  00000003  
Filippo Daniotti    204679  filippo.daniotti@unitn.it            00000004  
```

## Docker commands

Run docker (if not enabled):

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
- `$(pwd)` is the local folder
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

``` C
make build
```
