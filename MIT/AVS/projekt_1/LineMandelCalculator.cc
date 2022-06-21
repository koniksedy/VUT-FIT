/**
 * @file LineMandelCalculator.cc
 * @author Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 05.11.2021
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mm_malloc.h>	// _mm_malloc


#include "LineMandelCalculator.h"


LineMandelCalculator::LineMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
	// Zarovnání na 64 by bylo asi logičtější, ale takhle je to rychlejší. ¯\_(ツ)_/¯
	data = (int *)(_mm_malloc(height * width * sizeof(int), 256));
	Real = (float *)(_mm_malloc(width * sizeof(float), 256));
	Imag = (float *)(_mm_malloc(width * sizeof(float), 256));
	x = (float *)(_mm_malloc(width * sizeof(float), 256));
}

LineMandelCalculator::~LineMandelCalculator() {
	_mm_free(data);
	data = NULL;
	_mm_free(Real);
	Real = NULL;
	_mm_free(Imag);
	Imag = NULL;
	_mm_free(x);
	x = NULL;
}


int * LineMandelCalculator::calculateMandelbrot () {

	// ptrData není sice nutný, ale zvyšuje výkon.
	int *ptrData = data;
	float *tmpX = x;
	float *tmpReal = Real;
	float *tmpImag = Imag;

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

		// Získání ukazatele na začátek řádku.
		int *ptrDataBackup = ptrData;
		
		// Hodnoty y je konstantní vrámci řádku.
		const float y = y_start + i * dy;
		for (int k = 0; k < limit; ++k)
		{
			ptrData = ptrDataBackup;

			// Odpočítává, kolik buněk v řádku chce ještě iterovat
			int brk = width;

			#pragma omp simd reduction(-:brk) aligned(tmpReal,tmpImag,tmpX,ptrData:256)
			for (int j = 0; j < width; ++j)
			{

				float r2 = tmpReal[j] * tmpReal[j];
				float i2 = tmpImag[j] * tmpImag[j];


				const int cnd = (r2 + i2  > 4.0f);
				brk -= cnd;

				ptrData[width*i + j] += !cnd;

				// Pokud již chce iterace zkončit, neudělám nic.
				tmpImag[j] = cnd ? tmpImag[j] : 2.0f * tmpReal[j] * tmpImag[j] + y;
				tmpReal[j] = cnd ? tmpReal[j] : r2 - i2 + tmpX[j];
			}
			// Pokud již nechcenikdo pokračovat v iteraci, ukončím smyčku.
			k = (brk == 0) ? limit : k;
		}
	}

	return data;

}
