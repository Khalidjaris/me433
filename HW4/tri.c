// Online C compiler to run C program online
#include <stdio.h>
#define TRI_NUM 50
float tri[TRI_NUM];

int main() {
    float step = 1.0/TRI_NUM;
    for (int i = 0; i < TRI_NUM; i++) {
        float j = i;
        if (i < TRI_NUM/4) {
            tri[i] = 512 + (j * step)*512*4;
        } 
        else if (i <= TRI_NUM*3/4 && i >= TRI_NUM/4) {
            tri[i] = 512*3 - (i * step)*512*4;
        } 
        else if (i >= TRI_NUM*3/4) {
            tri[i] = - (512*3) + (i * step)*512*4;
        }
        
        if (tri[i] >= 1024) {
            tri[i] = 1023;
        }
        else if (tri[i] < 0) {
            tri[i] = 0;
        }
    }
    
    for (int i = 0; i < TRI_NUM; i++) {
        printf("%d: %d\n", i, (int) tri[i]);
    }
}
