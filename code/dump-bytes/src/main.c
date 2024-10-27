#include <stdio.h>
#include <gip_gfx/gip_gfx.h>

/**
 * This dumps the first 16-bytes of a file.
 * First argument needs to be the file path.
 */
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Invalid file given.");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "rb");
  if (!file)
    return EXIT_FAILURE;

  unsigned char buffer[16] = {0};

  size_t bytes_read = fread(buffer, sizeof(unsigned char), sizeof(buffer), file);
  fclose(file);

  for (size_t i = 0; i < bytes_read && i < sizeof(buffer); i++)
    printf("%02x ", buffer[i]);

  printf("\n");

  return EXIT_SUCCESS;
}
