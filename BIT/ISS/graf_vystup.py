import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import IPython
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk
import statistics
from scipy import stats
import os

# Michal Šedý <xsedym02>

# Vyhledávání hitů mezi všemi vstupními soubory
for src in os.listdir('../sentences'):
	# Načtení věty
	s, fs = sf.read('../sentences/'+src)
	# Odfitrování střední hodnoty
	s = s - statistics.mean(s)
	t = np.arange(s.size) / fs

	# Nastavení grafu signálu
	plt.figure(figsize=(7,5))
	plt.subplot(3,1,1)
	plt.plot(t, s)
	plt.gca().set_xlabel('$t$[$s$]')
	plt.gca().set_ylabel('signal')
	plt.yticks(np.arange(-1, 1, step=0.2))
	plt.title(src.split('.')[0])
	plt.margins(0)
	plt.tight_layout()

	# Výopčet parametrů pro spectrogram
	wlen = 25e-3 * fs
	wshift = 10e-3 * fs
	woverlap = wlen - wshift
	# Získání hammingova okna
	win = np.hamming(wlen)

	# Výpočet spectrogramu
	f, t, sgr = spectrogram(s, fs, win, None, woverlap, 511)

	# Vytvoření matíce o rozměnech 16x256, a naplnění nulama
	A = np.zeros((16, 256))

	# Vyplnění matice pomocí lineární banky filtrů
	for i in range(0,16):
		for j in range(0,16):
			A[i][j+i*16] = 1

	F = A.dot(sgr)

	# Výpočet logaritmu před zobrazením.
	# Protože ze spektrogramu někdy výjde 0, tak se pokaždé přičítá malá hodnota.
	F_v = 10 * np.log10(F+1e-20) 

	# Nastavení grafu Features
	plt.subplot(3,1,2)
	plt.pcolormesh(F_v)
	plt.xticks(np.arange(0, len(F_v[0])+1, step = 100), [0,1,2,3,4,5,6,7,8,9,10,11])
	plt.yticks(np.arange(0, 17, step=4))
	plt.gca().set_xlabel('$t$[$s$]')
	plt.gca().set_ylabel('features')
	plt.margins(0)
	plt.tight_layout()

	# Načítání query1 a zpracování viz výše
	s_q1, fs_q1 = sf.read('../queries/q1.wav')
	s_q1 = s_q1 - statistics.mean(s_q1)
	t_q1 = np.arange(s_q1.size) / fs_q1

	f, t, sgr_q1 = spectrogram(s_q1, fs, win, None, woverlap, 511)

	F_q1 = A.dot(sgr_q1)

	#Vypočet podobnosti
	# Matici F a Q1 transponujeme, abychom je mohli procházet po řádcích
	F = F.transpose()
	Q1 = F_q1.transpose()
	result1 = []

	# Počet testů je menší o výšku Q1, abyhcom nevypadli z matice F
	testy = len(F) - len(Q1) + 1

	#Výpočet vzdílenostri F a Q1 s pomosí pearsonových korelačních koeficientů
	for i in range(0,testy):
		r = 0	
		for j in range(0, len(Q1)):
			tmp, nic = stats.pearsonr(Q1[j], F[i+j])
			r+=tmp
		result1 = np.append(result1, r)

	# Ošetření pole podobností na zápornou podobnost
	result1 = abs(result1)

	# Načítání query2 a zpracování viz výše
	s_q2, fs_q2 = sf.read('../queries/q2.wav')
	s_q2 = s_q2 - statistics.mean(s_q2)
	t_q2 = np.arange(s_q1.size) / fs_q2

	f, t, sgr_q2 = spectrogram(s_q2, fs, win, None, woverlap, 511)

	F_q2 = A.dot(sgr_q2)

	#Výpočet podobnosti
	# Matici Q2 transponujeme, abychom ji mohli procházet také po řádcích
	Q2 = F_q2.transpose()
	result2 = []

	# Počet testů je menší o výšku Q1, abyhcom nevypadli z matice F
	testy = len(F) - len(Q2) + 1

	for i in range(0,testy):
		r = 0	
		for j in range(0, len(Q2)):
			tmp, nic = stats.pearsonr(Q2[j], F[i+j])
			r+=tmp
		result2 = np.append(result2, r)

	# Ošetření pole podobností na zápornou podobnost
	result2 = abs(result2)

	# Nastavení grafu urovně podobnosti věty s q1 a q2
	plt.subplot(3,1,3)
	plt.plot(np.arange(len(result1))/100, result1/len(Q1), label="overweight")
	plt.plot(np.arange(len(result2))/100, result2/len(Q2), label="retaliatory")
	plt.gca().set_xlabel('$t$[$s$]')
	plt.gca().set_ylabel('score')
	plt.yticks(np.arange(0, 1.1, step=0.2))
	plt.xticks(np.arange(0, 5.1, step=1))
	plt.margins(0)
	plt.legend()
	plt.ylim(0, 1)
	plt.xlim(0, (len(F)+1)/100)
	plt.tight_layout()

	plt.show()