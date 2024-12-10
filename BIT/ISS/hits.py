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
	#Odfiltrování střední hodnoty
	s = s - statistics.mean(s)
	t = np.arange(s.size) / fs

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

	# Načítání query1 a zpracování viz výše
	s_q1, fs_q1 = sf.read('../queries/q1.wav')
	s_q1 = s_q1 - statistics.mean(s_q1)
	t_q1 = np.arange(s_q1.size) / fs_q1

	f, t, sgr_q1 = spectrogram(s_q1, fs, win, None, woverlap, 511)

	F_q1 = A.dot(sgr_q1)

	#Výpočet podobnosti
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

	# Ořezání koeficientů podobnosti do rozsahu 0..1
	result1 = result1/len(Q1)
	result2 = result2/len(Q2)

	# Vyhledávání query ve zdrojových větách
	q1_name = 1
	i = 0
	while i < len(result1):
		# Pokud je podobnost větší než 0.9, pak jsme nalezli hit
		if result1[i] > 0.90:
			print(src.split('.')[0]+'_'+str(q1_name)+'   vzorek: '+str(i*160))
			# Od detekovaného hitu nahrajeme do výsledku signál stejně dlouhý jako query
			sf.write('../hits/q1_'+src.split('.')[0]+'_'+str(q1_name)+'.wav', s[i:i+len(s_q1)], 16000)
			# Počítadlo pouneme o query
			i += len(s_q1)
			q1_name += 1
		i += 1

	# Obdobný výpočet jako výše
	q2_name = 1
	i = 0
	while i < len(result2):
		if result2[i] > 0.90:
			print(src.split('.')[0]+'_'+str(q2_name)+'   vzorek: '+str(i*160))
			sf.write('../hits/q2_'+src.split('.')[0]+'_'+str(q2_name)+'.wav', s[i:i+len(s_q2)], 16000)
			i += len(s_q2)
			q2_name += 1
		i += 1
