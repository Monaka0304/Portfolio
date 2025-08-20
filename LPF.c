#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define N_filt 101  // フィルタの長さ
#define N_dft 128  // DFTの長さ

int hamming_window(int n, double *w) {
     /* ハミング窓関数を算出(表9.2に参照すること) */
    for(int i = 0; i<n; i++){
       w[i] = 0.54 - 0.46 * cos(2 * M_PI * i / (n-1));
    }

    return 0;
}
    

int DFT(int N, double complex *x, double complex *y) {
    /* 11回課題で実装したDFTをここに利用　*/ 
    double complex Wn = cexp(-2.0 * I * M_PI / N);
    for (int k = 0; k < N; k++) {
        y[k] = 0;
        for (int n = 0; n < N; n++) {
            y[k] += x[n] * cpow(Wn, k * n); // y[k]を計算
        }
    }
    return 0; 
}

int phase_unwrap(double *phase, int length) {
    for (int i = 1; i < length; i++) {
        double diff = phase[i] - phase[i - 1];
        if (fabs(diff) > M_PI) {
            int n = round(diff / (2 * M_PI));
            phase[i] -= n * 2 * M_PI;
        }
    }
    return 0;
}

int main(){
    double omega_c = M_PI * 0.4;  // カットオフ周波数
    double *hd = calloc(N_filt, sizeof(double));;  // 理想フィルタのインパルス応答
    double *w = calloc(N_filt, sizeof(double));  // ハミング窓関数
    double *h = calloc(N_filt, sizeof(double));  // フィルタ係数
    double complex *h_dft = calloc(N_dft, sizeof(double complex));  // ゼロパディングされたフィルタ係数
    double complex *H = calloc(N_dft, sizeof(double complex));  // 周波数特性
    double *H_amp = calloc(N_dft / 2, sizeof(double));  // 振幅特性
    double *H_phase = calloc(N_dft / 2, sizeof(double));  // 位相特性

    // 1. 理想フィルタのインパルス応答を計算(sinc関数)
    /* 理想フィルタのインパルス応答を計算 */
      int mid = (N_filt - 1) / 2;
    for (int n = 0; n < N_filt; n++) {
        if (n == mid) {
            hd[n] = omega_c / M_PI;
        } else {
            double val = (double)(n - mid);
            hd[n] = sin(omega_c * val) / (M_PI * val);
        }
    }

    // 2. ハミング窓を適用し、フィルタ係数を計算
    hamming_window(N_filt, w);
    for (int i = 0; i < N_filt; i++) {
        h[i] = hd[i] * w[i];
    }

    // 3. DFTのためにゼロパディング
    for (int i = 0; i < N_filt; i++) {
        h_dft[i] = h[i] + 0.0 * I;  // 実部と虚部を設定
    }
    for (int i = N_filt; i < N_dft; i++) {
        h_dft[i] = 0.0 + 0.0 * I;  // ゼロパディング
    }

    // 4. 振幅/位相特性を計算
    DFT(N_dft, h_dft, H);
    for (int k=0; k < N_dft / 2; k++) {
        H_amp[k] = 20 * log10(cabs(H[k]));  // 振幅特性
        H_phase[k] = carg(H[k]);  // 位相特性
    }
    phase_unwrap(H_phase, N_dft / 2);  // 位相をアンラップ


    printf("Impulse response:\n");
    for (int i = 0; i < N_filt; i++) {
        printf("h[%d] = %f\n", i, h[i]);
    }
    printf("Amplitude response:\n");
    for (int k = 0; k < N_dft / 2; k++) {
        printf("H_amp[%d] = %f\n", k, H_amp[k]);
    }
    printf("Phase response:\n");
    for (int k = 0; k < N_dft / 2; k++) {
        printf("H_phase[%d] = %f\n", k, H_phase[k]);
    }

    // グラフ描画用にテキストファイルに出力
    // 実部、虚部、振幅をそれぞれのファイルに
    FILE *file_coeff = fopen("impulse_response.txt", "w");
    FILE *file_amp = fopen("amplitude.txt", "w");
    FILE *file_phase = fopen("phase.txt", "w");
    for (int k = 0; k < N_filt; k++) {
        fprintf(file_coeff, "%f\n", h[k]);
    }

    for (int k = 0; k < N_dft / 2; k++) {
        fprintf(file_amp, "%f\n", H_amp[k]);
        fprintf(file_phase, "%f\n", H_phase[k]);
    }

    return 0;
}