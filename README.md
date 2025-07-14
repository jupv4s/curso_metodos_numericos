## Clonación de repositorios

Primero debemos identificarnos 
```bash
    git config --global user.email "micorreo@gmail.com"
    git config --global user.name "minombredeusuario"
```
Luego podemos ver si se guardó nuestra configuración de git
```bash
    git config --list
```
Después creamos una clave SSH
```bash
    ssh-keygen -t ed25519 -C "micorreo@gmail.com"
```
Cargamos la clave al agente SSH
```bash
    ssh-agent -c | source
    ssh-add ~/.ssh/id_ed25519
```
Copiamos la clave pública en Github
```bash
    cat ~/.ssh/id_ed25519.pub
```
Verificamos si la conexión tuvo éxito
```bash
    ssh -T git@github.com
```
Finalmente clona con
```bash
    git clone git@github.com:usuario/repositorio.git
```
(Adicional): En caso de no poder sincronizar los repositorios usar
```
    git fetch origin
    git reset --hard origin/main
    git pull
```