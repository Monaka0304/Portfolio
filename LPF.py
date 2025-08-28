import numpy as np
import matplotlib.pyplot as plt

# パラメータ設定
L = 101  # フィルタ長（奇数）
omega_c = 0.4 * np.pi  # カットオフ周波数
N = 128  # DFT点数

# 1. 理想フィルタのインパルス応答を計算
hd = np.zeros(L)
M = (L - 1) // 2  # 中心
for n in range(L):
    if n == M:
        hd[n] = omega_c / np.pi
    else:
        hd[n] = (np.sin(omega_c * (n - M))) / (np.pi * (n - M))

# 2. ハミング窓を作成
w = 0.54 - 0.46 * np.cos(2 * np.pi * np.arange(L) / (L - 1))

# フィルタ係数 h = hd * w
h = hd * w

# 3. ゼロパディングして DFT 用の配列を作成
h_dft = np.concatenate((h, np.zeros(N - L)))

# 4. DFT を計算
def DFT(x, N):
    n = np.arange(len(x))
    k = np.arange(N).reshape((N, 1))
    W = np.exp(-1j * 2 * np.pi * k * n / N)
    return np.dot(W, x)

H = DFT(h_dft, N)

# 振幅特性・位相特性
H_amp = 20 * np.log10(np.abs(H[:N//2]))
H_phase = np.unwrap(np.angle(H[:N//2]))

# ------------------ グラフ描画 ------------------
plt.figure(figsize=(12, 8))

# インパルス応答
plt.subplot(3, 1, 1)
plt.stem(h)
plt.title("Impalseouto h(n)")
plt.xlabel("n")
plt.ylabel("h(n)")

# 振幅特性
plt.subplot(3, 1, 2)
plt.plot(np.linspace(0, 0.5, N//2), H_amp)
plt.title("Shinpukutokusei")
plt.xlabel("seikikasyuhasu (×π rad)")
plt.ylabel("Amplitude [dB]")

# 位相特性
plt.subplot(3, 1, 3)
plt.plot(np.linspace(0, 0.5, N//2), H_phase)
plt.title("isoutokusei")
plt.xlabel("seikikasyuhasu (×π rad)")
plt.ylabel("Phase [rad]")

plt.tight_layout()
plt.savefig("LPF_result.png")