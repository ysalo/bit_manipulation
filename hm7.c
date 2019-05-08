#define SHIFT_32 0x80000000
#define OFFSET 127
#include <stdio.h>

/* Prompts the user for a float, loops until a float is entered. */
void user_input(float *fp) {

    printf("Enter a float: ");
    
    while( scanf("%f", fp) != 1 ) { // Check input
        
        printf("That is not a float!\nEnter a float: ");
        scanf("%s*");
    }
    printf("\n");
}

/* Prints the users input as it was read. */
void print_as_read(float const *fp) {
    printf("Your float was read as: %f\n", *fp);
}

/* Prints the users input as a binary number. */
void print_as_binary(int number) {
    printf("Your float in 32 bits: ");
    for(int i = 0; i < 32 ; i++) {

        printf("%d", ((number & SHIFT_32) >> 31) ); // shift the most significant digit to the right 
        number = number << 1; // shift to the next significant digit 
   }
   printf("\n");
}

/* Prints out the sign of the number. */
void print_sign(int number) {
    printf("Sign: %d\n", number);
}

/* Prints the exponent in binary. */
void print_exponent_binary(unsigned int number) {
    number = number << 1; // shift away the sign bit.
    printf("Exponent: ");
    for(int i = 0; i < 8; i++) {
        printf("%d", ((number & SHIFT_32) >> 31)  ); 
        number = number << 1;
    }
    printf("\n");
}

/* Prints the fraction part of the number in binary. */
void print_fraction_binary(unsigned int number) {
    number = number << 9; // shift away the sign and the exponent bits.
    printf("Fraction: ");
    for(int i = 0; i < 23; i++) {
        printf("%d", ((number & SHIFT_32) >> 31)  );
        number = number << 1;
    }
    printf("\n");
}

/* Calculate the sign on the number. Return 1 for positive and 0 for negative. */
int get_sign(unsigned int number) {
    number = (number & SHIFT_32) >> 31;
    return number;
}

/* Calculates the unbiased exponent and returns it. */
int get_exp( unsigned int number) {
    number = number << 1; // shift away the sign.
    number = number >> 24; // shift to the right to get the actual number. 

    int actual_exp = number - OFFSET; //subtract the offset.

    return actual_exp;
}

/* Creates and returns the fraction part. */
float create_fraction(unsigned int number) {

    printf("Creating the fraction: \n");
    float fraction = 1;
    printf("fraction = %f  (the implicit 1) \n", fraction);

    number = number << 9; //shift way the sign and the exponent.
    float frac_part = 0.5; //start the fraction.

    for(int i = 0; i < 23; i++) {
        int bit = number >> 31;
        if(bit == 1) { //check if to skip or add.
            fraction += frac_part;
            printf("fraction = %f, after adding %f\n", fraction, frac_part);
        } else {
            printf("fraction = %f, after skipping %f\n", fraction, frac_part);
        }
        number = number << 1;
        frac_part /= 2;


    }
    printf("\n");

    return fraction;
}

/* Applies the exponent to the fraction. */
float apply_exponent(float fraction, int exp, int sign) {

    printf("Applying the exponent:\n");
    printf("unbiased exponent = %d\n", exp);

    if(exp >= 0) { //check the exponent.
        for(int i = 0; i < exp; i++) {
            fraction = fraction * 2;
            printf("times 2 = %f\n", fraction);
        }
    } else {
        for(int i = exp; i < 0; i++) {
            fraction = fraction / 2;
            printf("divided by 2 = %f\n", fraction);
        }
    }

    if(sign == 1) { // if the sign was negative multiply the fraction by -1
        fraction *= -1;
    }
    return fraction;
}

/* Recreates the number by calling other functions. */
float create_number(unsigned int number, int exp, int sign) {
    float fraction = create_fraction(number);
    float new_num = apply_exponent(fraction, exp, sign);
    return new_num;
}

/* Starting point for the program. */
int main(void) {

    unsigned int number;
    float *fp = (float*) &number;
    user_input(fp);
    print_as_read(fp);
    print_as_binary(number);
    int sign = get_sign(number);
    print_sign(sign);
    print_exponent_binary(number);
    print_fraction_binary(number);
    int exp = get_exp(number);
    printf("\n");
    float rec_number = create_number(number, exp, sign);
    printf("\nFinal Answer: %f\n", rec_number);

    return 0;
}
