/*********************************************** 
 * Name: Darshan Hiremath
 * Project description: A practice of concealing a file, message, image, 
 *                      or video within another file, message, image, or video.
 * Date: 12th May 2025
*************************************************/

#include <stdio.h>      // Standard I/O library
#include <string.h>     // string handling functions
#include "encode.h"     // User-defined header for encoding-related functions
#include "decode.h"     // User-defined header for decoding-related functions
#include "types.h"      // User-defined header


// argc: Argument count - holds the number of command-line arguments including the program name.
// argv: Argument vector - an array of strings representing the command-line arguments.
int main(int argc, char *argv[])
{
    int line_count = argc;      //argument count is stored to integer line_count
    //printf("Line count : %d\n",line_count);
    //when ./a.exe has proper len arguments with it
    if(check_operation_type(argv) == e_encode){     //check operation type is encoding
        if(line_count < 4 || line_count > 5){       //checking that encoding contains 4 to 5 arguments 
                                                    // if error print the error statements 
            printf("For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
            printf("Error in Command-line arguments\n");
            return -1;      //terminated with an error.
        }
    }
    else if(check_operation_type(argv) == e_decode){        //check operation type is decoding
        if(line_count < 3 || line_count > 4){               //checking that decoding contains 3 to 4 arguments
                                                            // if error print the error statements
            printf("For decoding : ./a.out -d stego.bmp [decode.txt]\n");
            printf("Error in Command-line arguments\n");
            return -1;       //terminated with an error.
        }
    }
    
    if(check_operation_type(argv) == e_encode){     //check operation type is encoding
        printf("Encoding selected\n");              //print Encoding selected
        EncodeInfo encode;                          //EncodeInfo is struct type, This is an instance of that structure 
        // Check if the command-line arguments for encoding are valid.
        // fuction call If valid, proceed with the encoding process.
        if(read_and_validate_encode_args(argv, &encode) == e_success){   
            printf("Read and validation of encode is success\n");
            printf("Encoding started ................\n");
            do_encoding(&encode);   // Call the function that performs the encoding process using the data in the EncodeInfo structure.
        }
        else{       //if read_and_validate_encode_args fails when there is improper input
            printf("Read and validation of encoder is failed\n");
            return -1;
        }
    }
    
    else if(check_operation_type(argv) == e_decode){    //check operation type is decoding
        printf("Decoding selected\n");                  //print Decoding selected
        DecodeInfo decode;                              //DecodeInfo is struct type, This is an instance of that structure 
        // Check if the command-line arguments for decoding are valid.
        // fuction call If valid, proceed with the decoding process.
        if(read_and_validate_decode_args(argv, &decode) == e_success){
            printf("Read and validation of decode is success\n");
            printf("Decoding started ................\n");
            do_decoding(&decode);   // Call the function that performs the decoding process using the data in the DecodeInfo structure.
        }
        else{       //if read_and_validate_decode_args fails when there is improper input
            printf("Read and validate od decoda is failed\n");
            return -1;

        }
    }
    else{          // If the user has not specified a valid operation (neither "-e" for encode nor "-d" for decode)
        printf("Invalid input\n");
        printf("For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding : ./a.out -d stego.bmp [decode.txt]\n");
        return -1;
    }

    return 0;
}


