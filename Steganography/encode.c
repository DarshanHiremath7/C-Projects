#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        printf("src_image_fname: %s\n", encInfo->src_image_fname);
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        printf("secret_fname: %s\n", encInfo->secret_fname);
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        printf("stego_image_fname: %s\n", encInfo->stego_image_fname);
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*
* Validate the command-line arguments is proper with all inputs
* The input check is : ./a.out -e beautiful.bmp secret.txt [stego.bmp]
*/
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* check the 2nd argument is the .bmp extention */
    if(strstr(argv[2], ".bmp") != NULL){
        encInfo->src_image_fname = argv[2];
    }
    /* if it is not .bmp extention it return command-line argument failed */
    else{
        return e_failure;
    }

    /* check the 3rd argument is the .txt extention */
    if(strstr(argv[3], ".txt") != NULL){
        encInfo->secret_fname = argv[3];
    }
    /* if it is not .txt extention it return command-line argument failed */
    else{
        return e_failure;
    }

    /* check the 4th argument is present or not */
    /* if it is NULL then create stego.bmp file */
    if(argv[4] != NULL){
        encInfo->stego_image_fname = argv[4];
    }
    else{
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

/*
* Check operation type 
* Wheather it is type of encode or decode
*/
OperationType check_operation_type(char *argv[])
{
    /* Check 1st command-line argument is -e (encode type) if yes return encode */
    if(strcmp(argv[1], "-e") == 0)
    return e_encode;
    /* Check 1st command-line argument is -d (decode type) if yes return decode */
    else if(strcmp(argv[1], "-d") == 0)
    return e_decode;
    /* Check 1st command-line argument is neither encode or decode then return unsupported */
    else
    return e_unsupported;
}

/*
* Calculate size of file 
* Input is file pointer
* run pointer from start '0' to the END and return byte size
*/
uint get_file_size(FILE *fptr){
    // Move the file pointer to the end of the file to determine its size.
    fseek(fptr, 0, SEEK_END);
    // Return the current position of the file pointer, which represents the total file size in bytes.
    return ftell(fptr);
}

/*
* Check capacity of Source file and Secret file 
*/
Status check_capacity(EncodeInfo *encInfo){
    /* Get the source image size by calling get_image_size_for_bmp()
       and store the result in the image_capacity field of the EncodeInfo structure. */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    /* Get the secret file size by callink get_file_size()
       and store the result in the size_secret_file field of the Encodeinfo structure. */
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    /* print image capacity just for reference */
    printf("image capacity : %u\n",encInfo->image_capacity);
    /* print secret file size just for reference */
    printf("secret file : %ld\n",encInfo->size_secret_file);
    /*
    * Check image_capacity is greater than 16+32+32+32+(size_secret_file*8)
    * 16 bits for magic string
    * 32 bits for secret file extension size
    * 32 bits for secret file size
    * 32 bits for secret file extension
    * and 8 bits per byte of the actual secret file data.
    */
    if(encInfo->image_capacity > 16+32+32+32+(encInfo->size_secret_file*8))
    {
        printf("encInfo->size_secret_file : %ld\n", 16+32+32+32+(encInfo->size_secret_file*8));
        return e_success;
    }
    else{
        return e_failure;
    }
}

/*
* copy the header file from source image file to setgo image file
* Input passing file pointer source image and file pointer destination image
*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    /* pointer starts from 0 in the source image file */
    fseek(fptr_src_image, 0, SEEK_SET);
    /* Declare a character array 'str' of size 54, typically used to store the 54-byte BMP header. */
    char str[54];
    /* 
    * Read the 54-byte BMP header from the source image file into the 'str' buffer. 
    * str destination buffer where the read data will be stored
    * 54 The size of each element to read, in bytes
    * 1 The number of elements to read. We're reading one block of 54 bytes.
    * fptr_src_image The file pointer to the source image file from which to read.
    */
    fread(str,54,1,fptr_src_image);
    /*
    * Write the 54-byte BMP header from the str into destination image
    * str Pointer to the data buffer to write — in this case, the 54-byte array that holds the BMP header
    * 54 The size (in bytes) of each element to write. Here, we want to write a block of 54 bytes.
    * 1 The number of elements to write — just 1 block of 54 bytes.
    * fptr_dest_image File pointer to the destination file (the stego image) where the data will be written.
    */
    fwrite(str,54,1,fptr_dest_image);
    return e_success;
}

/*
* Encodes a single byte of data into the LSBs of 8 consecutive bytes in the image buffer.
* This function modifies each of the first 8 bytes in the given image buffer
* so that their least significant bits represent the 8 bits of the input data byte.
*/
Status encode_byte_to_lsb(char data, char *image_buffer){
    for(int i=0; i<8; i++){
        /* Clear the LSB of image_buffer[i] using AND with 0xFE (11111110),
           then set it to the i-th bit of 'data' using OR. */
        image_buffer[i] = image_buffer[i] & 0xFE | (data >> i & 1);
    }
    return e_success;
}

/*
* encode data and store into LSB of destination file
*/
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo){
    /* Run a loop until the size of extention */
    for(int i=0; i<size; i++){
        /* Read 8 bytes of image data from the source image file into the buffer.
            These 8 bytes will be used to hide one byte of secret data.*/
        fread(encInfo->image_data, 8, 1, encInfo->fptr_src_image);
        /* Encode one byte of secret data (data[i]) into the least significant bits
            of the 8 image bytes stored in encInfo->image_data. */
        encode_byte_to_lsb(data[i], encInfo->image_data);
        /* Write the modified 8 bytes (with hidden data) into the stego image file. */
        fwrite(encInfo->image_data, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

/*
* encode magic string read from common.h about magic string
* and store in the destination file
*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo){
    /* to write into the destination file pass this to function call encode_data_to_image */
    encode_data_to_image(magic_string, strlen(magic_string), encInfo);
    return e_success;
}

/*
* Encode size to LSB
* read the size of source file 
* and store into destination file
*/
Status encode_size_to_lsb(int size, EncodeInfo *encInfo){
    /* Declare a character array 'str' of size 32, typically used to store the 32-bytes of size source file */
    char str[32];
    /* Read 32 bytes of image data from the source image file into the str.
            These 32 bytes will be used to store size of source file. */
    fread(str, 32, 1, encInfo->fptr_src_image);
    //For each of the 32 bits in the integer 'size':
    for(int i=0; i<32; i++){
    /* Right shift 'size' by i to bring the i-th bit to the least significant position.
    * Mask it with & 1 to extract just that bit (either 0 or 1).
    * Clear the LSB of str[i] using & 0xFE (11111110).
    * Set the LSB of str[i] to the extracted bit using OR.
    */
        str[i] = str[i] & 0xFE | (size >> i & 1);
    }
    /* Write the modified 32 bytes into the stego image file. */
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/*
* Encodes the size of the secret file extension into the LSB of the image data.
*/
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo){
    // Call a helper function that encodes to LSBs of image data
    encode_size_to_lsb(size, encInfo);
    return e_success;
}

/*
* encode secret file extention and store in destination file
*/
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo){
    /* to write into the destination file pass this to function call encode_data_to_image */
    encode_data_to_image(file_extn ,strlen(file_extn) , encInfo);
    return e_success;
}

/*
* Encodes the size of the secret file into the LSB of the image data.
*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    // Call a helper function that encodes to LSBs of image data
    encode_size_to_lsb(file_size , encInfo);
    return e_success;
}

/*
* Encodes the actual content of the secret file into the image using LSB
*/
Status encode_secret_file_data(EncodeInfo *encInfo){
    //Create a buffer to hold the entire secret file content
    char str[encInfo->size_secret_file];
    //Reset file pointer to start of the secret file
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    //Read the entire secret file into the buffer
    fread(str, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    //Encode the data buffer into the image using LSB method
    encode_data_to_image(str, encInfo->size_secret_file, encInfo);
    return e_success;
}

/*
* Copies the remaining image data from the source image file to the destination image file.
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    //copy byte by byte
    // Temporary variable to hold each byte during copying
    char ch;
    // Read one byte at a time from source and write to destination
    while(fread(&ch , 1 , 1 , fptr_src) > 0)
    {
        fwrite(&ch , 1 , 1 , fptr_dest);
    }
    //Returns e_success when copying is completed successfully.
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo){
    /* Check whether the file is successfully opened or not */
    if(open_files(encInfo) == e_success){
        printf("Open file is sucess\n");
    }
    else{
        printf("open file is failure\n");
        return e_failure;
    }

    /* Check whether the image bmp have enough capacity to hold secret text */
    if(check_capacity(encInfo) == e_success)
    {
        printf("Check capacity success\n");
    }
    else{
        printf("Check capacity is failed");
        return e_failure;
    }

    /* copy the header of source image  to stego image file */
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("copy bmp header sussecc\n");
    }
    else{
        printf("copy bmp header failed\n");
        return e_failure;
    }

    /* encode magic string and store in stego image */
    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success){
        printf("Encode magic string successfull\n");
    }
    else{
        printf("Failed to encode magic string\n");
        return e_failure;
    }

    /* Copy the file extension (starting from the dot) from the secret file name
       into the 'extn_secret_file' field of the EncodeInfo structure. */
    strcpy(encInfo->extn_secret_file, strchr(encInfo->secret_fname, '.'));

    /* Calculate secret file extention size */
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success){
        printf("Enocde secret file extention size successfull\n");
    }
    else{
        printf("Failed encode secret file extention size\n");
        return e_failure;
    }

    /* encode the secret file extention */
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success){
        printf("Enocde secret file extention successfull\n");
    }
    else{
        printf("Failed encode secret file extention\n");
        return e_failure;
    }

    /* encode secret file size */
    if(encode_secret_file_size( encInfo->size_secret_file, encInfo) == e_success)
    {
        printf("Encoded secrete file data size successfully\n");
    }
    else
    {
        printf("Failed to encode secrete file size\n");
        return e_failure;
    }

    /* encode secret file data and store into LSB in strgo */
    if(encode_secret_file_data(encInfo) == e_success){
        printf("Encode secret data successfull\n");
    }
    else{
        printf("Failed to encode secret data\n");
        return e_failure;
    }

    /* Copy all remaining source image data to stego image file*/
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success){
        printf("copied remaining data successful\n");
    }
    else{
        printf("Failed to copy remaining data\n");
        return e_failure;
    }

    return e_success;
}

