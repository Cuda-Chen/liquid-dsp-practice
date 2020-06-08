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

# write filtered data
#np.savetxt('scipy_result.txt', filtered)

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

# show frequency response
w, h = signal.sosfreqz(sos, worN=5000)
db = 20*np.log10(np.maximum(np.abs(h), 1e-5))
plt.subplot(3, 1, 1)
plt.plot(w/np.pi, db)
plt.ylim(-3, 0)
plt.grid(True)
plt.yticks([0, -0.5, -1, -1.5, -2, -2.5, -3])
plt.ylabel('pass band [dB]')
plt.title('Frequency Response in Pass Band')
plt.subplot(3, 1, 2)
plt.plot(w/np.pi, db)
plt.ylim(-75, 5)
plt.grid(True)
plt.yticks([0, -20, -40, -60])
plt.ylabel('Gain [dB]')
plt.title('Frequency Response')
plt.subplot(3, 1, 3)
plt.plot(w, 180/np.pi*np.angle(h))
plt.grid(True)
'''
plt.yticks([-np.pi, -0.5*np.pi, 0, 0.5*np.pi, np.pi],
           [r'$-\pi$', r'$-\pi/2$', '0', r'$\pi/2$', r'$\pi$'])
'''
#plt.ylabel('Phase [rad]')
plt.ylabel('Phase [degree]')
plt.xlabel('Normalized frequency (1.0 = Nyquist)')
plt.show()
