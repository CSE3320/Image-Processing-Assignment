#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MESSAGE_LENGTH 255

int main()
{
  int x_res          = 0;
  int y_res          = 0;
  int num_components = 0;
  char * filename    = "2019-07-03-0.jpg";

  // We're allocating A LOT of extra memory here, around 24M for the working area. 
  // Each component can have values up to 255 so by using a 64-bit uint we can add up
  // 72340173000000000 images before we worry about overflowing an individual pixel's value 
  // before we divide it and get a pixel value average.
  uint8_t *working_values = NULL;

  unsigned char * data = stbi_load( filename, &x_res, &y_res, &num_components, 0 ); 

  if( data == NULL )
  {
    char * error = ( char * ) malloc ( MESSAGE_LENGTH * sizeof( char ) );
   
    sprintf( error, "Error opening file %s: ", filename ); 
    perror( error );

    free( error );

    exit( 1 );
  }

  working_values = ( uint8_t* ) malloc ( sizeof( uint8_t ) * x_res * y_res * num_components );

  if( working_values == NULL )
  {
    char * error = ( char * ) malloc ( MESSAGE_LENGTH * sizeof( char ) );
   
    sprintf( error, "Could not allocate %d bytes: ", x_res * y_res * num_components * sizeof( uint64_t ) ); 
    perror( error );

    free( error );

    exit( 1 );
  }

  printf( "x: %d y: %d n: %d\n", x_res, y_res, num_components );


  int index = 0;

  for( index = 0; index < x_res * y_res * num_components; index++ )
  {
      // Copy the red component
      working_values[ index ] = data[ index ];
  }

  stbi_write_jpg( "test.jpg", x_res, y_res, num_components, working_values, 100 );

  free( data );
  free( working_values );

  return 0;
}
