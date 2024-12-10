import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import IPython
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk
import statistics
from scipy import stats

# Michal Šedý <xsedym02>

# Získání signálu z wavu
s, fs = sf.read('../sentences/sa2.wav')
# Odfiltrování střední hodnoty
s = s - statistics.mean(s)
t = np.arange(s.size) / fs

# Výpočet parametrů pro funkci spectrogram
wlen = 25e-3 * fs
wshift = 10e-3 * fs
woverlap = wlen - wshift
# Vytvoření hamingova okna
win = np.hamming(wlen)

f, t, sgr = spectrogram(s, fs, win, None, woverlap, 511)

# Protože ze spektrogramu někdy výjde 0, tak se pokaždé přičítá malá hodnota
sgr = 10 * np.log10(sgr+1e-20) 

# Vykreslování grafu
plt.figure(figsize=(9,3))
plt.pcolormesh(t, f, sgr)
plt.gca().set_xlabel('Time [s]')
plt.gca().set_ylabel('Frequency [Hz]')
plt.title('sa2')
	
plt.tight_layout()

plt.show()
