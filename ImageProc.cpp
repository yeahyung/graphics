// ------------------------------------------------------------------------------------------------
// Image class Implementation file
// ------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ImageProc.h"
#include<glut.h>

// ------------------------------------------------------------------------------------------------
// Convert to gray-scale
// ------------------------------------------------------------------------------------------------

void ImageProc::convertToGray()
{
	int  x, y;
	byte R, G, B;
	byte Gray;

	for (y = 0; y < m_iHeight; y++)
	{
		for (x = 0; x < m_iWidth; x++)
		{
			getPixel(x, y, R, G, B);

			Gray = xClip(0.299*R + 0.587*G + 0.114*B);

			setPixel(x, y, Gray, Gray, Gray);
		}
	}
}

void ImageProc::convertToSepia()
{
	int  x, y;
	byte R, G, B;
	byte RR, GG, BB;

	for (y = 0; y < m_iHeight; y++)
	{
		for (x = 0; x < m_iWidth; x++)
		{
			getPixel(x, y, R, G, B);

			RR = xClip(R*0.393 + G * 0.769 + B * 0.189);
			GG = xClip(R*0.349 + G * 0.686 + B * 0.168);
			BB = xClip(R*0.272 + G * 0.534 + B * 0.131);

			setPixel(x, y,RR,GG,BB);
		}
	}
}

void ImageProc::samplingBy2()
{   // 크기를 절반으로
	int  x, y;
	byte R, G, B;

	for (y = 0; y < m_iHeight/2; y++)
	{
		for (x = 0; x < m_iWidth/2; x++)
		{
			getPixel(x*2, y*2, R, G, B);                     // 1  2  3  4
			m_pRGB[(y * m_iWidth / 2 * 3 + x * 3) + 0] = R;  // 5  6  7  8         1  3  
			m_pRGB[(y * m_iWidth / 2 * 3 + x * 3) + 1] = G;  // 9 10 11 12     ->  9 11
			m_pRGB[(y * m_iWidth / 2 * 3 + x * 3) + 2] = B;  //13 14 15 16
			//getAddr return  y * m_iWidth * 3 + x * 3;
		}
	}
	m_iHeight /= 2;
	m_iWidth /= 2;
}

void ImageProc::quantization(int step)
{   // 색을 절반으로 
	int  x, y;
	byte R, G, B;
	byte RR, GG, BB;
	if (step >=128)
		step = 128;
	for (y = 0; y < m_iHeight; y++)
	{
		for (x = 0; x < m_iWidth; x++)
		{
			getPixel(x, y, R, G, B);
			RR = xClip(R - (R%step));
			GG = xClip(G - (G%step));
			BB = xClip(B - (B%step));
			setPixel(x, y, RR,GG,BB);
		}
	}
}