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
  unsigned short width, height, color_depth, grayscale, header_size;

  fread(&width, 2, 1, file);
  fread(&height, 2, 1, file);
  fread(&color_depth, sizeof(unsigned short), 1, file); 
  fread(&grayscale, sizeof(unsigned short), 1, file); 
  fread(&header_size, sizeof(unsigned short), 1, file);

  printf("Width: %d, Height: %d, Header Size: %d bit, Grayscale %d, Headersize %d\n", width, height, color_depth, grayscale, header_size);

  fseek(file, header_size, SEEK_SET);

  unsigned short* image = (unsigned short*)malloc(width * height * (color_depth / 2));
  if(!image) {
    printf("Failed to allocate image");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
