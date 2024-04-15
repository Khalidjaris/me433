// Online C compiler to run C program online
#include <stdio.h>
#include <math.h>

#define FREQUENCY 1   
#define SINAMPS (FREQUENCY * 50) // number of points in waveform
float sine_array[SINAMPS];

// Function to convert decimal to binary
int decimalToBinary(int n) {
    // Array to store binary number
    int binaryNum[32];
    
    // Counter for binary array
    int i = 0;
    while (n > 0) {
        // Store remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    
    // Print binary array in reverse order
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binaryNum[j]);
    }
}

int main() {
    float v = 3.3;
    int channel = 1;
    int amp = (v/3.3) * 1024 / 2;
    int frequency = 1;
    int buf[2];
    int first = (channel<<3) | 0b0111;
    
    for (int i = 0; i < SINAMPS; i++) {
        float increment = (2 * M_PI) / SINAMPS;
        int sine = amp * sin(increment * i) + amp;
        buf[0] = first<<4 | sine>>6;
        buf[1] = sine<<2;
        // sine_array[i] = first<<4 | sine>>6;
        // printf("sine: %d\n", (int) sine);
        // printf("buf1: %d\n", (int) buf[0]);
        // printf("buf2: %d\n", (int) buf[1]);
        // printf("sine: %d\n", (int) sine);
        printf("Sine: ");
        decimalToBinary(sine);
        printf("\n");
        printf("bu1: ");
        decimalToBinary(buf[0]);
        printf("\n");
        printf("buf2: ");
        decimalToBinary(buf[1]);
        printf("\n");
        printf("\n");
        printf("buf: ");
        decimalToBinary(buf[0]);
        printf(" ");
        decimalToBinary(buf[1]);
        printf("\n");
        printf("\n");
    }

    // printf("%f\n", sine_array[0]);
    // printf("%f\n", sine_array[25]);
    // printf("%f\n", sin((2 * M_PI / NUMSAMPS) * 50/4));

    return 0;
}