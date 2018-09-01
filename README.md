# Mini-Google

## Instrucciones:

Clonar el repositorio:
```
git clone https://github.com/UCSP-Search-Engine/mini-google.git
```
Copiar dentro de la carpeta generada **mini-google** la carpeta **raw.es** con todos los artículos a ser pre-procesados.

### Parse: Pre-Procesamiento

Moverse a la carpeta

```
$cd core/parse
```
Ejecutar el siguiente comando para compilar:
```
make

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
Ejecutar:
```
./engine
```
Para la opción **App**, seguir las siguientes instrucciones:

### Mini-Google: App

Instalar el framework de Python necesario para ejecutar la aplicacion web.

Instalar Flask:

```
sudo easy_install Flask
```
Ejecutar el archivo **app.py** de la carpeta:

```
python app.py
```

Finalmente abrir un navegador y colocar la url: http://localhost:5000
