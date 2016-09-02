// PrintScreen.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include <windows.h>
#include <iostream>
#include <afxwin.h>
using namespace std;

/**************************************************
	*
	*���� stdafx.h �ļ��м��� #include <conio.h>
	*
**************************************************/


void Screen(char filename[])
{
	CDC *pDC;//��ĻDC
	pDC = CDC::FromHandle(GetDC(NULL));//��ȡ��ǰ������ĻDC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	cout << "��ǰ��Ļɫ��ģʽΪ" << BitPerPixel << "λɫ��" << endl
		<< "��Ļ��ȣ�" << Width << endl
		<< "��Ļ�߶ȣ�" << Height << endl << endl;

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filename, "w+b");

	BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���

	BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ

	byte *p = new byte[(bmp.bmWidthBytes) * (bmp.bmHeight)];//�����ڴ汣��λͼ����

	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����
	delete []p;
	fclose(fp);
	memDC.SelectObject(oldmemBitmap);
}

int _tmain(int argc, _TCHAR* argv[])
{
	char Filename[100];
	int count = 0;

	while(!_kbhit())//�û��������˳�
	{
		Sleep(5000);//����5��
		count ++;
		sprintf_s(Filename, sizeof(Filename), "%d.bmp", count);
		Screen(Filename);//����Screen����
	}

	return 0;
}


/**********************************************************************************************************************************
	������������б��������������⣺

	1>------ ����������: ��Ŀ: PrintScreen, ����: Debug Win32 ------
	1>  PrintScreen.cpp
	1>uafxcwd.lib(afxmem.obj) : error LNK2005: "void * __cdecl operator new[](unsigned int)" (??_U@YAPAXI@Z) �Ѿ��� libcpmtd.lib
	(newaop.obj) �ж���
	1>C:\Works\Project\Test\PrintScreen\Debug\PrintScreen.exe : fatal error LNK1169: �ҵ�һ���������ض���ķ���
	========== ����: �ɹ� 0 ����ʧ�� 1 �������� 0 �������� 0 �� ==========

	����������£�

	��Ŀ����ҳ---��������---�����룬����������ݣ��������¸Ķ���ע��˳��

	���������⣺uafxcwd.lib;LIBCMTD.lib;
	�����ض�Ĭ�Ͽ⣺LIBCMTD.lib;uafxcwd.lib

	�����������󣬱�������

	1>------ ����������: ��Ŀ: PrintScreen, ����: Debug Win32 ------
	1>  PrintScreen.cpp
	1>  PrintScreen.vcxproj -> C:\Works\Project\Test\PrintScreen\Debug\PrintScreen.exe
	========== ����: �ɹ� 1 ����ʧ�� 0 �������� 0 �������� 0 �� ==========

**********************************************************************************************************************************/

