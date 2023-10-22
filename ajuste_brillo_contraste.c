#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <jpeglib.h>

// Función para ajustar el brillo y el contraste de una imagen PNG
void adjust_brightness_contrast_png(png_bytep data, int width, int height, int brightness, int contrast) {
    // Iterar a través de los píxeles de la imagen
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Iterar a través de los componentes de color (R, G, B)
            for (int c = 0; c < 3; c++) {
                // Calcular el índice del píxel en el arreglo de datos
                int index = (y * width + x) * 3 + c;
                // Aplicar el ajuste de brillo y contraste
                int new_value = data[index] * contrast / 100 + brightness;
                // Asegurarse de que el valor esté en el rango [0, 255]
                if (new_value < 0) {
                    new_value = 0;
                } else if (new_value > 255) {
                    new_value = 255;
                }
                // Actualizar el valor del píxel
                data[index] = (png_byte)new_value;
            }
        }
    }
}

// Función para procesar una imagen PNG
void process_png_image(const char* input_filename, const char* output_filename, int brightness, int contrast) {
    // Abrir archivos de entrada y salida
    FILE* infile = fopen(input_filename, "rb");
    FILE* outfile = fopen(output_filename, "wb");

    // Manejo de errores si no se pueden abrir los archivos
    if (!infile || !outfile) {
        fprintf(stderr, "Error al abrir archivos de entrada/salida.\n");
        return;
    }

    // Crear estructuras PNG para lectura y escritura
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fprintf(stderr, "Error al crear la estructura de lectura PNG.\n");
        fclose(infile);
        fclose(outfile);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fprintf(stderr, "Error al crear la estructura de información PNG.\n");
        png_destroy_read_struct(&png, (png_infopp)NULL, (png_infopp)NULL);
        fclose(infile);
        fclose(outfile);
        return;
    }

    // Configurar la lectura y escritura PNG
    png_init_io(png, infile);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);

    // Alojar memoria para almacenar las filas de la imagen
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
        png_read_row(png, row_pointers[y], NULL);
    }

    // Aplicar el ajuste de brillo y contraste a la imagen
    for (int y = 0; y < height; y++) {
        adjust_brightness_contrast_png(row_pointers[y], width, height, brightness, contrast);
    }

    // Configurar la escritura PNG
    png_init_io(png, outfile);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    // Escribir las filas procesadas en la imagen de salida
    for (int y = 0; y < height; y++) {
        png_write_row(png, row_pointers[y]);
    }

    png_write_end(png, NULL);

    // Liberar memoria y cerrar archivos
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png, &info, (png_infopp)NULL);

    fclose(infile);
    fclose(outfile);
}

// Función para ajustar el brillo y el contraste de una imagen JPEG
void adjust_brightness_contrast_jpeg(JSAMPROW data, int width, int brightness, int contrast) {
    // Iterar a través de los píxeles de la imagen
    for (int x = 0; x < width ; x++) {
        // Iterar a través de los componentes de color (R, G, B)
        for (int c = 0; c < 3; c++) {
            // Calcular el índice del píxel en el arreglo de datos
            int index = x * 3 + c;
            // Aplicar el ajuste de brillo y contraste
            int new_value = data[index] * contrast / 100 + brightness;
            // Asegurarse de que el valor esté en el rango [0, 255]
            if (new_value < 0) {
                new_value = 0;
            } else if (new_value > 255) {
                new_value = 255;
            }
            // Actualizar el valor del píxel
            data[index] = (JSAMPLE)new_value;
        }
    }
}

// Función para procesar una imagen JPEG
void process_jpeg_image(const char* input_filename, const char* output_filename, int brightness, int contrast) {
    // Estructuras JPEG para lectura y escritura
    struct jpeg_decompress_struct cinfo_in;
    struct jpeg_error_mgr jerr_in;
    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr_out;
    FILE* infile = fopen(input_filename, "rb");
    FILE* outfile = fopen(output_filename, "wb");

    // Manejo de errores si no se pueden abrir los archivos
    if (!infile || !outfile) {
        fprintf(stderr, "Error al abrir archivos de entrada/salida.\n");
        return;
    }

    // Configurar las estructuras JPEG para lectura y escritura
    cinfo_in.err = jpeg_std_error(&jerr_in);
    jpeg_create_decompress(&cinfo_in);
    jpeg_stdio_src(&cinfo_in, infile);
    jpeg_read_header(&cinfo_in, TRUE);
    jpeg_start_decompress(&cinfo_in);

    int width = cinfo_in.output_width;
    int height = cinfo_in.output_height;

    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = width;
    cinfo_out.image_height = height;
    cinfo_out.input_components = 3;
    cinfo_out.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, 100, TRUE);
    jpeg_start_compress(&cinfo_out, TRUE);

    while (cinfo_in.output_scanline < cinfo_in.output_height) {
        JSAMPROW row_pointer[1];
        row_pointer[0] = (JSAMPROW)malloc(cinfo_in.output_width * cinfo_in.output_components);

        jpeg_read_scanlines(&cinfo_in, row_pointer, 1);

        // Aplicar el ajuste de brillo y contraste a la fila actual
        adjust_brightness_contrast_jpeg(row_pointer[0], width, brightness, contrast);

        jpeg_write_scanlines(&cinfo_out, row_pointer, 1);

        free(row_pointer[0]);
    }

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    jpeg_finish_decompress(&cinfo_in);
    jpeg_destroy_decompress(&cinfo_in);

    // Cerrar archivos
    fclose(infile);
    fclose(outfile);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <input.png/jpeg> <output.png/jpeg> <brightness> <contrast>\n", argv[0]);
        return 1;
    }

    const char* input_filename = argv[1];
    const const char* output_filename = argv[2];
    int brightness = atoi(argv[3]);
    int contrast = atoi(argv[4]);

    // Determinar el tipo de archivo de entrada y procesar la imagen correspondiente
    if (strstr(input_filename, ".png") || strstr(input_filename, ".PNG")) {
        process_png_image(input_filename, output_filename, brightness, contrast);
    } else if (strstr(input_filename, ".jpg") || strstr(input_filename, ".jpeg") || strstr(input_filename, ".JPG") || strstr(input_filename, ".JPEG")) {
        process_jpeg_image(input_filename, output_filename, brightness, contrast);
    } else {
        fprintf(stderr, "El formato de archivo no es compatible. Use archivos PNG o JPEG.\n");
        return 1;
    }

    return 0;
}













