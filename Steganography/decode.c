#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"


/*
* Check operation type 
* Wheather it is type of encode or decode
*/
// OperationType check_operation_type(char *argv[])
// {
//     /* Check 1st command-line argument is -e (encode type) if yes return encode */
//     if(strcmp(argv[1], "-e") == 0)
//     return e_encode;
//     /* Check 1st command-line argument is -d (decode type) if yes return decode */
//     else if(strcmp(argv[1], "-d") == 0)
//     return e_decode;
//     /* Check 1st command-line argument is neither encode or decode then return unsupported */
//     else
//     return e_unsupported;
// }

/*
* Validate the command-line arguments is proper with all inputs
* The input check is : ./a.out -d stego.bmp [decode.txt]
*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo){
    /* check the 2nd argument is the .bmp extention */
    if(strstr(argv[2], ".bmp") != NULL){
        decInfo->stego_image_fname = argv[2];
    }
    /* if it is not .bmp extention it return command-line argument failed */
    else{
        return e_failure;
    }
    /* check the 3rd argument is the .txt extention */
    if(argv[3] != NULL){
        if(strstr(argv[3], ".txt") != NULL){
            decInfo->decode_fname = argv[3];
        }
        /* if it is not .txt extention it return command-line argument failed */
        else{
            return e_failure;
        }
    }
    /* if it is NULL then create decode.txt file */
    else{
        decInfo->decode_fname = "decode.txt";
    }
    return e_success;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status file_open(DecodeInfo *decInfo){
    //Stego Image file 
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    //Do Error handling
    if(decInfo->fptr_stego_image == NULL){
        printf("stego_image_fname: %s\n", decInfo->stego_image_fname);
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
    }
    //Decode file
    decInfo->fptr_decode = fopen(decInfo->decode_fname, "wb+");
    // Do Error handling
    if(decInfo->fptr_decode == NULL){
        printf("decode_fname: %s\n", decInfo->decode_fname);
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);
    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

/*
* Decodes a single byte of hidden data from the LSB 
* of 8 consecutive bytes of image data.
*/
Status decode_byte_from_lsb(char *image_buffer, char *data){
    // char ch initial zero 
    char ch = 0;
    /* Loop through 8 bytes to reconstruct one byte of hidden data */
    for(int i=0; i<8; i++){
        /*
        * Extract the LSB from image_buffer[i] and place it at the i-th bit of ch
          (image_buffer[i] & 1) gets the LSB, then << i shifts it to the correct position
        */
        ch = ch | (image_buffer[i] & 1) << i;
    }
    /* Store the reconstructed byte in the output variable */
    *data = ch;
    return e_success;
}

/*
* Decodes a sequence of bytes (data) from the LSBs of the stego image.
*/
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        // Temporary buffer to hold 8 bytes from the stego image
        char imag_buffer[8];
        // Read 8 bytes from the stego image file
        fread(imag_buffer, 1, 8, decInfo->fptr_stego_image);
        // Read 8 bytes from the stego image file
        decode_byte_from_lsb(imag_buffer, &data[i]);
    }
    return e_success;
}

/*
* Decode secret file data
*/
Status decode_secret_file_data(DecodeInfo *decInfo){
    char str[decInfo->secret_ext_size+1];
    fseek(decInfo->fptr_decode, 0, SEEK_SET);
    decode_data_from_image(str, decInfo->secret_ext_size, decInfo);
    str[decInfo->secret_ext_size] = '\0';
    //printf("STR : %s\n",str);
    fwrite(str, 1, decInfo->secret_ext_size, decInfo->fptr_decode);
    return e_success;
}

/*
Decode magic string data from stego and store into buffer
if the stego magic_string is compare with common.h
if equal the proceed else return error
*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo){
    int length = strlen(magic_string);
    char decoded_string[length + 1];
    
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    
    decode_data_from_image(decoded_string, length, decInfo);
    decoded_string[length] = '\0';

    printf("Decoded Magic String: %s\n", decoded_string);

    if (strcmp(decoded_string, magic_string) == 0){
        return e_success;
    }
    else{
        return e_failure;
    }
}

/*
* decode secret file extention
*/
Status decode_secret_file_extn(DecodeInfo *decInfo){
    int len = decInfo->secret_ext_file_size;
    char decode_ext_file[len + 1];
    for(int i=0; i<decInfo->secret_ext_file_size; i++){
        char ext_file[8];
        fread(ext_file, 8, 1, decInfo->fptr_stego_image);
        decode_byte_from_lsb(ext_file, &decode_ext_file[i]);
    }
    decode_ext_file[len] = '\0';
    printf("File ext string is %s\n",decode_ext_file);
    return e_success;
}

/*
* This function is used to extract values such as the size of the secret file extension
* or the secret file content size. Each LSB of the 32 bytes is combined to reconstruct
* the original 32-bit integer.
*/
long decode_size_of_lsb(DecodeInfo *decInfo){
    // Buffer to store 32 bytes read from the stego image
    char ext_buffer[32];
    // Read 32 bytes from the stego image file
    fread(ext_buffer, 32, 1, decInfo->fptr_stego_image);
    // Initialize the variable to store reconstructed size
    int size=0;
    // Loop to extract the LSB from each byte and reconstruct the 32-bit integer
    for(int i=0; i<32; i++){
        /*
        * Extract the LSB of ext_buffer[i] using & 1
        * Then left shift it by i positions and OR it with size
        * This accumulates each bit into the correct position
        */
        size |= (ext_buffer[i] & 1) << i;
    }
    return size;
}

/*
* Decode secret file extention size to decode which type of secret file is this 
*/
Status decode_secret_file_ext_size(DecodeInfo *decInfo){
    /* decode size of secret file extention and store in DecodeInfo structure */
    decInfo->secret_ext_file_size = decode_size_of_lsb(decInfo);
    return e_success;
}

/*
* Decode secret file size
*/
Status decode_secret_file_size(DecodeInfo *decInfo){
    /* decode size of secret file and store in DecodeInfo structure */
    decInfo->secret_ext_size = decode_size_of_lsb(decInfo);
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo){
    /* File open condition */
    if(file_open(decInfo) == e_success){
        printf("File open successfull\n");
    }
    else{
        printf("Failed to open file\n");
        return e_failure;
    }

    /* Comparing magic string from encode and decode */
    if(decode_magic_string(MAGIC_STRING, decInfo) == e_success){
        printf("Magic string match successful.\n");
    }
    else{
        printf("Magic string mismatch.\n");
        return e_failure;
    }

    /* Decode secret file exention size */
    if(decode_secret_file_ext_size(decInfo) == e_success){
        printf("Sizeof secret file extention : %d\n",decInfo->secret_ext_file_size);
        printf("Decode of secret file extention successfull\n");
    }
    else{
        printf("Failed to decode secret file extention\n");
        return e_failure;
    }

    /* Decode the secret file extention */
    if(decode_secret_file_extn(decInfo) == e_success){
        printf("Decode of ext file name successfull\n");
    }
    else{
        printf("Failed to decode ext file name\n");
        return e_failure;
    }

    /* Decode secret file size */
    if(decode_secret_file_size(decInfo) == e_success){
        printf("Size of secret file : %d\n", decInfo->secret_ext_size);
        printf("Decode of secret file size successfull\n");
    }
    else{
        printf("Failed to decode secret file size\n");
        return e_failure;
    }

    /* Decode secret file data and store into decode text file */
    if(decode_secret_file_data(decInfo) == e_success){
        printf("decode secret data successfull\n");
    }
    else{
        printf("Failed to decode secret data\n");
        return e_failure;
    }

    return e_success;
}