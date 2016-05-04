#include "ScreenCapture.h"


ScreenCapture::ScreenCapture(void)
{
}


ScreenCapture::~ScreenCapture(void)
{
}

//Description 폴더 내에 [오늘 날짜]를 이름으로 하는 Folder를 생성
//만약 Description 폴더가 존재하지 않을 경우, Description 폴더도 생성
void ScreenCapture::CreateFolder()
{
	//Init Capture Index 
	m_nCaptureIndx = 0;
	m_nExportIdx = 0;
	//Init String valuable
	m_strCaptureFile = new char[256];
	m_strToday = new char[256];
	m_strCurrentTime = new char[256];
	m_strExportFile = new char[256];

	//Description 을 오늘 날짜 폴더에 저장
	time_t t = time(NULL);
	localtime_s( &today, &t);
	sprintf(m_strToday,  "..\\Description\\%d_%d_%d", today.tm_year+1900, today.tm_mon+1, today.tm_mday);
	
	//Create Folder
	CreateDirectory("..\\Description", NULL);
	CreateDirectory(m_strToday, NULL);

	char strCaptureDir[256];
	char strExportDir[256];
	sprintf_s(m_strCurrentTime, size_t(m_strCurrentTime), "%s\\%d_%d_%d", m_strToday, today.tm_hour, today.tm_min, today.tm_sec);
	sprintf_s(strCaptureDir, size_t(strCaptureDir), "%s\\%s", m_strCurrentTime, "Capture");
	sprintf_s(strExportDir, size_t(strExportDir), "%s\\%s", m_strCurrentTime, "Export");
	sprintf_s(m_strCaptureFile, size_t(m_strCaptureFile), "%s\\capture_", strCaptureDir);
	sprintf_s(m_strExportFile, size_t(m_strExportFile), "%s\\export_", strExportDir);

	CreateDirectory(m_strCurrentTime, NULL);
	CreateDirectory(strCaptureDir, NULL);
	CreateDirectory(strExportDir, NULL);
}

void ScreenCapture::BeginCapture()
{
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 800;

	char filename[255];
	sprintf_s(filename, size_t(filename), "%s%04d.bmp", m_strCaptureFile, m_nCaptureIndx);
	printf("   - Capture Screen Number : %d \n", m_nCaptureIndx);
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
	FILE *file;
	fopen_s(&file, filename, "wb");
	if( image!=NULL )
	{
		if( file!=NULL ) 
		{
			glReadPixels( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, image );

			memset( &bf, 0, sizeof( bf ) );
			memset( &bi, 0, sizeof( bi ) );

			bf.bfType = 'MB';
			bf.bfSize = sizeof(bf)+sizeof(bi)+WINDOW_WIDTH * WINDOW_WIDTH * 3;
			bf.bfOffBits = sizeof(bf)+sizeof(bi);
			bi.biSize = sizeof(bi);
			bi.biWidth = WINDOW_WIDTH;
			bi.biHeight = WINDOW_WIDTH;
			bi.biPlanes = 1;
			bi.biBitCount = 24;
			bi.biSizeImage = WINDOW_WIDTH * WINDOW_WIDTH*3;

			fwrite( &bf, sizeof(bf), 1, file );
			fwrite( &bi, sizeof(bi), 1, file );
			fwrite( image, sizeof(unsigned char), WINDOW_WIDTH * WINDOW_WIDTH * 3, file );

			fclose( file );
		}
		free( image );
	}

	m_nCaptureIndx ++;
}

void ScreenCapture::ReleaseMemory()
{
	delete[] m_strToday;
	delete[] m_strCurrentTime;
	delete[] m_strCaptureFile;
}