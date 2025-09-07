#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Decode file Info */
    char *decode_fname;
    FILE *fptr_decode;
    int secret_ext_file_size;
    long secret_ext_size;
    char decode_data[MAX_SECRET_BUF_SIZE];
    long size_deocde_file;

} DecodeInfo;


/* Decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status file_open(DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *image_buffer, char *data);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode Magic String for comparing */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension from stego image */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode a 32-bit value from the LSBs of 32 bytes of image data */
long decode_size_of_lsb(DecodeInfo *decInfo);

/* Decode the size of the secret file extension from stego image */
Status decode_secret_file_ext_size(DecodeInfo *decInfo);

/* Decode the size of the actual secret file content from stego image */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

#endif