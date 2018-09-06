# Mini-Google

## Instrucciones:

Clonar el repositorio:
```
git clone https://github.com/UCSP-Search-Engine/mini-google.git
```
Copiar dentro de la carpeta generada **mini-google** la carpeta **raw.es** con todos los artículos a ser pre-procesados.

### Parse: Pre-Procesamiento

Moverse a la carpeta:

```
$cd mini-google/core/parse
```
Ejecutar el siguiente comando para compilar:
```
make
```
O

```
g++ -o ../milibreria.so ./main.cpp -std=c++11 -fPIC -shared -O3
```

Ejecutar:
```
./parse
```

### Mini-Google: Core
Moverse a la carpeta descargada:
```
$cd mini-google
```
Ejecutar el siguiente comando para compilar:
```
make
```

O

```
g++ -o engine core/console.cpp -std=c++11 -O3
```

Ejecutar:
```
./engine
```

## Mini-Google: App

### Instalar requisitos en linux

Instalar el framework de Python necesario para ejecutar la aplicacion web.

Instalar Flask:

```
pip3 install Flask
```

Instalar Cffi:

```
pip3 install cffi
```

### Instalar requisitos en MacOs

ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

Instalar python3

```
brew install python3
```
Instalar Flask:

```
pip3 install Flask
```

Instalar Cffi:

```
pip3 install cffi
```
### Compilar aplicación

Moverse a la carpeta:

```
$cd mini-google/core
```

Ejecutar el siguiente comando para compilar:
```
make
```
O

```
g++ -o ../milibreria.so ./main.cpp -std=c++11 -fPIC -shared -O3
```

### Ejecutar aplicación

Moverse a la carpeta:

```
$cd mini-google
```

Ejecutar el comando

```
python3 app.py
```

## Abrir aplicación


* [Server Link](http://127.0.0.1:5000) - presione para abrir 'http://127.0.0.1:5000'

## Abrir aplicación ubicada en la nube

* [Server Link](http://142.93.54.15:5000) - presione para abrir 'http://142.93.54.15:5000'