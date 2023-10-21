## Randy Cascante Espinoza C11718
## Proyecto de Programacion Bajo Plataformas Abiertas IE-0117
## Descripción del código

El código creado consiste en un programa en lenguaje C que permite ajustar el brillo y el contraste de imágenes en formatos PNG y JPEG. A continuación, se describe brevemente la estructura y las funciones del código:

- El código incluye bibliotecas estándar como `<stdio.h>`, `<stdlib.h>`, `<string.h>`, así como las bibliotecas `<png.h>` y `<jpeglib.h>` para el manejo de imágenes PNG y JPEG, respectivamente.

- Se define la función `adjust_brightness_contrast_png` que se encarga de ajustar el brillo y el contraste de una imagen PNG.

- La función `process_png_image` procesa una imagen PNG, aplicando los ajustes de brillo y contraste y guardando la imagen resultante en un archivo de salida.

- Se define la función `adjust_brightness_contrast_jpeg` para ajustar el brillo y el contraste de una imagen JPEG.

- La función `process_jpeg_image` procesa una imagen JPEG, aplicando los ajustes de brillo y contraste y guardando la imagen resultante en un archivo de salida.

- En la función `main`, se maneja la entrada de línea de comandos para determinar el archivo de entrada, el archivo de salida, el brillo y el contraste deseados, y se selecciona la función de procesamiento adecuada según el formato del archivo de entrada (PNG o JPEG).

Este código es un ejemplo de procesamiento de imágenes en C y muestra cómo se pueden aplicar ajustes de brillo y contraste a imágenes en dos formatos populares. El programa se ejecuta desde la línea de comandos y toma como argumentos el archivo de entrada, el archivo de salida, el brillo y el contraste deseados.
