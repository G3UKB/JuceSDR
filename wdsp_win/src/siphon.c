/*  siphon.c

This file is part of a program that implements a Software-Defined Radio.

Copyright (C) 2013 Warren Pratt, NR0V

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

The author can be reached by email at  

warren@wpratt.com

*/

#include "comm.h"

void build_window (SIPHON a)
{
	int i;
	double arg0, cosphi;
	double sum, scale;
	arg0 = 2.0 * PI / ((double)a->fftsize - 1.0);
	sum = 0.0;
	for (i = 0; i < a->fftsize; i++)
	{
		cosphi = cos (arg0 * (double)i);
		a->window[i] =	+ 6.3964424114390378e-02
		  + cosphi *  ( - 2.3993864599352804e-01
		  + cosphi *  ( + 3.5015956323820469e-01
		  + cosphi *  ( - 2.4774111897080783e-01
		  + cosphi *  ( + 8.5438256055858031e-02
		  + cosphi *  ( - 1.2320203369293225e-02
		  + cosphi *  ( + 4.3778825791773474e-04 ))))));
		sum += a->window[i];
	}
	scale = 1.0 / sum;
	for (i = 0; i < a->fftsize; i++)
		a->window[i] *= scale;
}

SIPHON create_siphon (int run, int insize, double* in, int sipsize, int fftsize, int specmode)
{
	SIPHON a = (SIPHON) malloc0 (sizeof (siphon));
	a->run = run;
	a->insize = insize;
	a->in = in;
	a->sipsize = sipsize;	// NOTE:  sipsize MUST BE A POWER OF TWO!!
	a->sipbuff = (double *) malloc0 (a->sipsize * sizeof (complex));
	a->idx = 0;
	a->sipout  = (double *) malloc0 (a->sipsize * sizeof (complex));
	a->fftsize = fftsize;
	a->specout = (double *) malloc0 (a->fftsize * sizeof (complex));
	a->specmode = specmode;
	a->sipplan = fftw_plan_dft_1d (a->fftsize, (fftw_complex *)a->sipout, (fftw_complex *)a->specout, FFTW_FORWARD, FFTW_PATIENT);
	a->window  = (double *) malloc0 (a->fftsize * sizeof (complex));
	build_window (a);
	return a;
}

void destroy_siphon (SIPHON a)
{
	fftw_destroy_plan (a->sipplan);
	_aligned_free (a->window);
	_aligned_free (a->specout);
	_aligned_free (a->sipout);
	_aligned_free (a->sipbuff);
	_aligned_free (a);
}

void flush_siphon (SIPHON a)
{
	memset (a->sipbuff, 0, a->sipsize * sizeof (complex));
	memset (a->sipout , 0, a->sipsize * sizeof (complex));
	memset (a->specout, 0, a->fftsize * sizeof (complex));
	a->idx = 0;
}

void xsiphon (SIPHON a)
{
	if (a->run)
	{
		if (a->insize >= a->sipsize)
			memcpy (a->sipbuff, &(a->in[2 * (a->insize - a->sipsize)]), a->sipsize * sizeof (complex));
		else
		{
			memcpy (&(a->sipbuff[2 * a->idx]), a->in, a->insize * sizeof (complex));
			if ((a->idx += a->insize) == a->sipsize) a->idx = 0;
		}
	}
}

void suck (SIPHON a)
{
	if (a->outsize <= a->sipsize)
	{
		int mask = a->sipsize - 1;
		int j = (a->idx - a->outsize) & mask;
		int size = a->sipsize - j;
		if (size >= a->outsize)
			memcpy (a->sipout, &(a->sipbuff[2 * j]), a->outsize * sizeof (complex));
		else
		{
			memcpy (a->sipout, &(a->sipbuff[2 * j]), size * sizeof (complex));
			memcpy (&(a->sipout[2 * size]), a->sipbuff, (a->outsize - size) * sizeof (complex));
		}
	}
}

void sip_spectrum (SIPHON a)
{
	int i;
	for (i = 0; i < a->fftsize; i++)
	{
		a->sipout[2 * i + 0] *= a->window[i];
		a->sipout[2 * i + 1] *= a->window[i];
	}
	fftw_execute (a->sipplan);
}

/********************************************************************************************************
*																										*
*											RXA Properties												*
*																										*
********************************************************************************************************/

//PORT
void RXAGetaSipF (int channel, float* out, int size)
{	// return raw samples as floats
	SIPHON a;
	int i;
	EnterCriticalSection (&ch[channel].csDSP);
	a = rxa[channel].sip1.p;
	a->outsize = size;
	suck (a);
	LeaveCriticalSection (&ch[channel].csDSP);
	for (i = 0; i < size; i++)
	{
		out[i] = (float)a->sipout[2 * i + 0];
	}
}

//PORT
void RXAGetaSipF1 (int channel, float* out, int size)
{	// return raw samples as floats
	SIPHON a;
	int i;
	EnterCriticalSection (&ch[channel].csDSP);
	a = rxa[channel].sip1.p;
	a->outsize = size;
	suck (a);
	LeaveCriticalSection (&ch[channel].csDSP);
	for (i = 0; i < size; i++)
	{
		out[2 * i + 0] = (float)a->sipout[2 * i + 0];
		out[2 * i + 1] = (float)a->sipout[2 * i + 1];
	}
}

/********************************************************************************************************
*																										*
*											TXA Properties												*
*																										*
********************************************************************************************************/

//PORT
void TXAGetaSipF (int channel, float* out, int size)
{	// return raw samples as floats
	SIPHON a;
	int i;
	EnterCriticalSection (&ch[channel].csDSP);
	a = txa[channel].sip1.p;
	a->outsize = size;
	suck (a);
	LeaveCriticalSection (&ch[channel].csDSP);
	for (i = 0; i < size; i++)
	{
		out[i] = (float)a->sipout[2 * i + 0];
	}
}

//PORT
void TXAGetaSipF1 (int channel, float* out, int size)
{	// return raw samples as floats
	SIPHON a;
	int i;
	EnterCriticalSection (&ch[channel].csDSP);
	a = txa[channel].sip1.p;
	a->outsize = size;
	suck (a);
	LeaveCriticalSection (&ch[channel].csDSP);
	for (i = 0; i < size; i++)
	{
		out[2 * i + 0] = (float)a->sipout[2 * i + 0];
		out[2 * i + 1] = (float)a->sipout[2 * i + 1];
	}
}

//PORT
void TXAGetSpecF1 (int channel, float* out)
{	// return spectrum magnitudes in dB
	SIPHON a;
	int i, j, mid, m, n;
	EnterCriticalSection (&ch[channel].csDSP);
	a = txa[channel].sip1.p;
	a->outsize = a->fftsize;
	suck (a);
	LeaveCriticalSection (&ch[channel].csDSP);
	sip_spectrum (a);
	mid = a->fftsize / 2;
	if (a->specmode == 0)
		// swap the halves of the spectrum
		for (i = 0, j = mid; i < mid; i++, j++)
		{
			out[i] = (float)(10.0 * mlog10 (a->specout[2 * j + 0] * a->specout[2 * j + 0] + a->specout[2 * j + 1] * a->specout[2 * j + 1] + 1.0e-60));
			out[j] = (float)(10.0 * mlog10 (a->specout[2 * i + 0] * a->specout[2 * i + 0] + a->specout[2 * i + 1] * a->specout[2 * i + 1] + 1.0e-60));
		}
	else
		// mirror each half of the spectrum in-place
		for (i = 0, j = mid - 1, m = mid, n = a->fftsize - 1; i < mid; i++, j--, m++, n--)
		{
			out[i] = (float)(10.0 * mlog10 (a->specout[2 * j + 0] * a->specout[2 * j + 0] + a->specout[2 * j + 1] * a->specout[2 * j + 1] + 1.0e-60));
			out[m] = (float)(10.0 * mlog10 (a->specout[2 * n + 0] * a->specout[2 * n + 0] + a->specout[2 * n + 1] * a->specout[2 * n + 1] + 1.0e-60));
		}
}