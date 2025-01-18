#include <stdio.h>
#include <math.h>

// Read the cycle count using RISC-V assembly
unsigned long read_cycles(void) {
    unsigned long cycles;
    asm volatile ("rdcycle %0" : "=r" (cycles));
    return cycles;
}

// Calculate log base 2 of an integer, refer to problem D, quize 5, C.A.2024.
int logint(int N)
{
    int k = N, i = 0;
    while (k) {
        k >>= 1;
        i++;
    }
    return i - 1;
}
// Reverse bits of an index for bit-reversal permutation, refer to problem D, quize 5, C.A.2024.
int reverse(int N, int n) {
    int j, p = 0;
    for (j = 1; j <= logint(N); j++) {
        if (n & (1 << (logint(N) - j)))
            p |= 1 << (j - 1);
    }
    return p;
}
void DIT_FFT(int size, float *real, float *image) {
    int i, j, k, stage, step;
    int halfStep;
    float tempReal, tempImage;

    // data swift to new index
    for (i = 0; i < size; i++) {
        int revIndex = reverse(size, i);
        if (i < revIndex) {
            // Swap real parts
            tempReal = real[i];
            real[i] = real[revIndex];
            real[revIndex] = tempReal;

            // Swap imaginary parts
            tempImage = image[i];
            image[i] = image[revIndex];
            image[revIndex] = tempImage;
        }
    }

    // Perform FFT computation
    for (stage = 1; stage <= logint(size); stage++) {
        step = 1 << stage;       // 2^stage
        halfStep = step >> 1;    // Step size / 2

        for (k = 0; k < size; k += step) {
            for (j = 0; j < halfStep; j++) {
                // Compute twiddle factor
                float angle = -2.0f * M_PI * (float)j / (float)step; // Angle for twiddle factor
                float cosValue = cos(angle);    // Replace with precomputed cosine lookup table for optimization
                float sinValue = sin(angle);    // Replace with precomputed sine lookup table for optimization

                // Butterfly computation
                int index1 = k + j;
                int index2 = index1 + halfStep;

                tempReal = cosValue * real[index2] - sinValue * image[index2];
                tempImage = sinValue * real[index2] + cosValue * image[index2];

                real[index2] = real[index1] - tempReal;
                image[index2] = image[index1] - tempImage;

                real[index1] += tempReal;
                image[index1] += tempImage;
            }
        }
    }
}

int main() {
    // Example input data
    float real[] = {1, 3, 1, 2, 6, 6, 1, 7}; // Real parts
    float image[] = {3, 6, 1, 6, 3, 2, 1, 7}; // Imaginary parts
    int size = 8; // Number of points in the FFT

    DIT_FFT(size, real, image);
    for (int i = 0; i < size; i++) {
        printf("%f + %fi\n",real[i], image[i]);
    }
    return 0;
}