#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MESSAGE_LENGTH 255

void show_help()
{
  printf("Use: stack [options]\n");
  printf("Where options are:\n");
  printf("-i <file>   Set input file format. It is assumed the last character is the image number.\n");
  printf("-o <file>   Set output file. (default=output.jpg)\n");
  printf("-n          Provide the number of input images.\n");
  printf("-h          Show this help text.\n");
  printf("\nSome examples are:\n");
  printf("stack -i IMG_000 -n 10 -o stack.jpg\n");
}

int main( int argc, char * argv[] )
{
  char c;
  int num_images          = 0;
  int x_res               = 0;
  int y_res               = 0;
  int num_components      = 0;
  char * input_filename   = NULL;
  char * output_filename  = NULL;

  while( ( c = getopt( argc, argv, "n:x:h:i:o:" ) ) != -1 ) 
  {
    switch( c ) 
    {
      case 'n':
        num_images = atoi( optarg );
      break;
      case 'i':
        input_filename = (char*) malloc ( strlen( optarg ) );
        strncpy( input_filename, optarg, strlen( optarg ) );
      break;
      case 'o':
        output_filename = (char*) malloc ( strlen( optarg ) );
        strncpy( output_filename, optarg, strlen( optarg ) );
      break;
      case 'h':
        show_help();
      break;
    }
  }

  if( num_images == 0 )
  {
    printf("You must provide the number of input images -n. \n\n");
    show_help();
    exit(1);
  }

  if( input_filename == NULL )
  {
    printf("You must provide an output input_filename with -i. \n\n");
    show_help();
    exit(1);
  }

  // We're allocating A LOT of extra memory here, around 24M for the working area. 
  // Each component can have values up to 255 so by using a 64-bit uint we can add up
  // 72340173000000000 images before we worry about overflowing an individual pixel's value 
  // before we divide it and get a pixel value average.
  uint8_t *working_values = NULL;

  unsigned char * data = stbi_load( input_filename, &x_res, &y_res, &num_components, 0 ); 

  if( data == NULL )
  {
    char * error = ( char * ) malloc ( MESSAGE_LENGTH * sizeof( char ) );
  
    memset( error, 0, MESSAGE_LENGTH ); 
    sprintf( error, "Error opening file %s: ", input_filename ); 
    perror( error );

    free( error );

    exit( 1 );
  }

  working_values = ( uint8_t* ) malloc ( sizeof( uint8_t ) * x_res * y_res * num_components );

  if( working_values == NULL )
  {
    char * error = ( char * ) malloc ( MESSAGE_LENGTH * sizeof( char ) );
   
    memset( error, 0, MESSAGE_LENGTH ); 
    sprintf( error, "Could not allocate %d bytes: ", x_res * y_res * num_components * sizeof( uint64_t ) ); 
    perror( error );

    free( error );

    exit( 1 );
  }

  int index = 0;

  for( index = 0; index < x_res * y_res * num_components; index++ )
  {
    working_values[ index ] = data[ index ];
  }

  stbi_write_jpg( "test.jpg", x_res, y_res, num_components, working_values, 100 );

  free( data );
  free( working_values );
  free( input_filename );

  return 0;
}
