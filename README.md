# GTK3-Interface-Client

Ce projet est le côté client du projet C réalisé dans le cadre de notre 3e année ICS au sein de CPE.    
Vous y retrouverez donc l'interface client permettant d'intéragir avec le serveur dans le cadre du jeu du [dilemne du prisonnier](https://en.wikipedia.org/wiki/Prisoner%27s_dilemma).

## Compilation du projet

### Installation des dépendances

- Fedora, CentOS, RHEL :
```
dnf install glade gtk3-devel gtk+-devel gtk3-devel-docs
```
- Ubuntu:
```
sudo apt-get install glade libgtk-3-dev libglib2.0-dev
```

### Préparation de l'environnement

```bash
git clone https://github.com/AlexisVillaroya/GTK3-Interface-Client
cd GTK3-Interface-Client
make
```

Le fichier compilé sera dans le dossier output

## Librairie réseau adaptée

Pour fonctionner, ce projet est co-créer avec la librairie disponible ci-dessous, permettant
la normalisation des échanges de données avec le serveur, par le réseau, à l'aide des sockets.

https://github.com/Samoth69/LibPrisonerNetwork

Plusieurs prérequis sont nécessaires dans le code du client :
- l'initialisation des méthodes (net_client_set_func_*)
- l'implémentation de ces méthodes

## Développeurs

- Alexis Villaroya
- Wolodia Zdetovetzky (wolozdeto@gmail.com, https://github.com/wolozdeto)
