/**
 * @file BatchMandelCalculator.cc
 * @author Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date 05.11.2021
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <mm_malloc.h>  // _mm_malloc, _mm_free

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
	// Zarovnání na 64 by bylo asi logičtější, ale takhle je to rychlejší. ¯\_(ツ)_/¯
	data = (int *)(_mm_malloc(height * width * sizeof(int), 256));
	Real = (float *)(_mm_malloc(width * sizeof(float), 256));
	Imag = (float *)(_mm_malloc(width * sizeof(float), 256));
	x = (float *)(_mm_malloc(width * sizeof(float), 256));
}

BatchMandelCalculator::~BatchMandelCalculator() {
	_mm_free(data);
	data = NULL;
	_mm_free(Real);
	Real = NULL;
	_mm_free(Imag);
	Imag = NULL;
	_mm_free(x);
	x = NULL;
}


int * BatchMandelCalculator::calculateMandelbrot () {
	
	// ptrData není sice nutný, ale zvyšuje výkon.
	int *ptrData = data;
	
	float *tmpX = x;
	float *tmpReal = Real;
	float *tmpImag = Imag;


	// Konstanty pro výpočet dávky
	const int burst = 64;
	const int rest = width%burst;
	const int alignWidth = width - rest;

	for (int i = 0; i < height; ++i)
	{
		// Inicializace první hodnoty Real a Imag.
		// Vypočítání hodnoty x, která je konstantní vrámci sloupce.
		#pragma omp simd
		for (int j = 0; j < width; ++j)
		{
			tmpImag[j] = y_start + i * dy;
			const float tmp = x_start + j * dx;
			tmpReal[j] = tmp;
			tmpX[j] = tmp;
		}

		// Hodnoty y je konstantní vrámci řádku.
		const float y = y_start + i * dy;
		// BODY loop
		for (int m = 0; m < alignWidth; m += burst)
		{
			// Získání ukazatele na začátek dávky.
			int *ptrDataBackup = ptrData;
			// Konec dávky
			const int nextStop = m + burst;
			
			for (int k = 0; k < limit; ++k)
			{
				// Odpočítává, kolik buněk v dávce  chce ještě iterovat
				int brk = burst;
				ptrData = ptrDataBackup;

				#pragma omp simd reduction(-:brk) aligned(tmpReal,tmpImag,tmpX,ptrData:256)
				for (int j = m; j < nextStop; ++j)
				{
					float r2 = tmpReal[j] * tmpReal[j];
					float i2 = tmpImag[j] * tmpImag[j];

					const int cnd = (r2 + i2  > 4.0f);
					brk -= cnd;

					ptrData[width*i + j] += !cnd;

					// Pokud již chce iterace zkončit, neudělám nic.
					tmpImag[j] = (cnd) ? tmpImag[j] : 2.0f * tmpReal[j] * tmpImag[j] + y;
					tmpReal[j] = (cnd) ? tmpReal[j] : r2 - i2 + tmpX[j];
				}

				// Pokud již nechce nikdo pokračovat v iteraci, ukončím smyčku.
				k = (brk == 0) ? limit : k;
			}
		}


		int *ptrDataBackup = ptrData;
		// EPILOG loop
		for (int k = 0; k < limit; ++k)
		{
			// Odpočítává, kolik buněk chce ještě iterovat
			int brk = rest;
			ptrData = ptrDataBackup;

			#pragma omp simd reduction(-:brk) aligned(tmpReal,tmpImag,tmpX,ptrData:256)
			for (int j = alignWidth; j < width; ++j)
			{
				float r2 = tmpReal[j] * tmpReal[j];
				float i2 = tmpImag[j] * tmpImag[j];

				const int cnd = (r2 + i2  > 4.0f);
				brk -= cnd;

				ptrData[width*i + j] += !cnd;

				// Pokud již chce iterace zkončit, neudělám nic.
				tmpImag[j] = (cnd) ? tmpImag[j] : 2.0f * tmpReal[j] * tmpImag[j] + y;
				tmpReal[j] = (cnd) ? tmpReal[j] : r2 - i2 + tmpX[j];
			}
			// Pokud již nechce nikdo pokračovat v iteraci, ukončím smyčku.
			k = (brk == 0) ? limit : k;
		}
	}

	return data;
}