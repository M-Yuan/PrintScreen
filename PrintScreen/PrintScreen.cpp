// PrintScreen.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <windows.h>
#include <iostream>
#include <afxwin.h>
using namespace std;

/**************************************************
	*
	*需在 stdafx.h 文件中加入 #include <conio.h>
	*
**************************************************/


void Screen(char filename[])
{
	CDC *pDC;//屏幕DC
	pDC = CDC::FromHandle(GetDC(NULL));//获取当前整个屏幕DC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	cout << "当前屏幕色彩模式为" << BitPerPixel << "位色彩" << endl
		<< "屏幕宽度：" << Width << endl
		<< "屏幕高度：" << Height << endl << endl;

	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息

	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filename, "w+b");

	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度

	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头

	byte *p = new byte[(bmp.bmWidthBytes) * (bmp.bmHeight)];//申请内存保存位图数据

	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
	delete []p;
	fclose(fp);
	memDC.SelectObject(oldmemBitmap);
}

int _tmain(int argc, _TCHAR* argv[])
{
	char Filename[100];
	int count = 0;

	while(!_kbhit())//用户按键则退出
	{
		Sleep(5000);//挂起5秒
		count ++;
		sprintf_s(Filename, sizeof(Filename), "%d.bmp", count);
		Screen(Filename);//调用Screen函数
	}

	return 0;
}


/**********************************************************************************************************************************
	将上述代码进行编译会出现以下问题：

	1>------ 已启动生成: 项目: PrintScreen, 配置: Debug Win32 ------
	1>  PrintScreen.cpp
	1>uafxcwd.lib(afxmem.obj) : error LNK2005: "void * __cdecl operator new[](unsigned int)" (??_U@YAPAXI@Z) 已经在 libcpmtd.lib
	(newaop.obj) 中定义
	1>C:\Works\Project\Test\PrintScreen\Debug\PrintScreen.exe : fatal error LNK1169: 找到一个或多个多重定义的符号
	========== 生成: 成功 0 个，失败 1 个，最新 0 个，跳过 0 个 ==========

	解决方法如下：

	项目属性页---》链接器---》输入，添加以下内容，但做如下改动，注意顺序

	附加依赖库：uafxcwd.lib;LIBCMTD.lib;
	忽略特定默认库：LIBCMTD.lib;uafxcwd.lib

	经上述操作后，编译正常

	1>------ 已启动生成: 项目: PrintScreen, 配置: Debug Win32 ------
	1>  PrintScreen.cpp
	1>  PrintScreen.vcxproj -> C:\Works\Project\Test\PrintScreen\Debug\PrintScreen.exe
	========== 生成: 成功 1 个，失败 0 个，最新 0 个，跳过 0 个 ==========

**********************************************************************************************************************************/

