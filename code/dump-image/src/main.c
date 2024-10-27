#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Invalid file given.\n");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "rb");
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return EXIT_FAILURE;
  }

  /**
   * This takes our width, height & header size (or color depth??)
   * from our given sample file
   */
  unsigned short width, height, header_size;

  fread(&width, sizeof(unsigned short), 1, file);
  fread(&height, sizeof(unsigned short), 1, file);
  fread(&header_size, sizeof(unsigned short), 1, file); // maybe that could be color-depth??

  printf("Width: %d, Height: %d, Header Size: %d bytes\n", width, height, header_size);

  /**
   * Move past the header size,
   * maybe the header size > 16??
   */
  fseek(file, header_size, SEEK_SET);

  // unsigned short = 16 bits = 2 bytes per pixel
  unsigned short *image = (unsigned short *)malloc(width * height * sizeof(unsigned short));
  if (image == NULL)
  {
    printf("Memory allocation failed.\n");
    fclose(file);
    return EXIT_FAILURE;
  }

  fread(image, sizeof(unsigned short), width * height, file);
  fclose(file);

  FILE *pgmFile = fopen("output.pgm", "wb");
  if (pgmFile == NULL)
  {
    printf("Error creating PGM file.\n");
    free(image);
    return EXIT_FAILURE;
  }

  fprintf(pgmFile, "P5\n%d %d\n65535\n", width, height);

  for (int i = 0; i < width * height; i++)
  {
    unsigned char highByte = (image[i] >> 8) & 0xFF;
    unsigned char lowByte = image[i] & 0xFF;

    fwrite(&highByte, 1, 1, pgmFile);
    fwrite(&lowByte, 1, 1, pgmFile);
  }

  fclose(pgmFile);
  free(image);

  printf("PGM file saved as output.pgm\n");

  return EXIT_SUCCESS;
}
