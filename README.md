# SistemaBiblioteca
Proyecto en lenguaje C, Sistema de bibliotecas

Desarrolladores:
Jordano Escalante Lopez
Hengerlyn Nash

Desarrollado para el proyecto del curso de lenguajes de programacin del instituto tecnologico de Costa Rica en el anio 2023

La documentacin externa de este proyecto puede ser cosultada en el siguiente enlace:
https://docs.google.com/document/d/1CYxoYMduonu6hwZghsRCiT4yFBCm1OOSUNc7fnENjJA/edit?usp=sharing

Instrucciones de compilacion:
Para poder compilar este proyecto es necesaio que cuenten con un compilador de lenguaje c, para este ejemplo usamos el compilador gcc, para instalar gcc en ubunto debe ejecutar en bash en comando sudo apt install build-essential.

Tambien es necesario que cuenten con el manejador de archivos JSON para lenguaje C, para instalar este manejar de archivos en Ubuntu debe ejecutar el comando
de bash sudo apt-get install libjson-c-dev

Una vez instalados estos dos aquetes debe ejecutar el comando gcc -o programa programa.c -ljson-c
Si el proceso de compilacion se realiza con exito deberia aparecer en la carpeta donde se encuentra el cdigo fuente un ejecutable con el nombre "programa" que le dimos anteriormente.

Instrucciones de uso:
En la consola, introduzca el comando ./programa y esto ejecutaria el programa principal haciendo aparecer el menu principal del mismo en consola.
Si llegase a aparecer un error que dice que no cuenta con los permisos de ejecucion, introduzca el comando "chmod +x programa", si este comando se ejecuta con exito podra ejecutar el comando de ejecucion ./programa y se desplegara el programa principal en su consola.
El programa es un programa simple de consola, para utilizarlo debera ingresar por medio del teclado lo que el programa le solicite y de esa forma navegara a traves de todos los menu de forma sencilla, evite el uso de caracteres especiales.
