import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

order = 4
lowcut = 0.05
highcut = 0.1
sampleRate = 100.0

# load data
data = np.loadtxt('test_wave.txt')

# create IIR filter prototype
sos = signal.iirfilter(order, [lowcut/sampleRate, highcut/sampleRate],
                       btype='bandpass', ftype='butter',
                       output='sos')
print(sos)

# filter the input data
filtered = signal.sosfilt(sos, data)

# show the output
x = np.arange(0, data.shape[0])
plt.subplot(211)
plt.plot(x, data, 'k')
plt.ylabel('Raw Data')
plt.grid()
plt.subplot(212)
plt.plot(x, filtered, 'k')
plt.ylabel('Bandpassed Data')
plt.grid()
plt.show()
