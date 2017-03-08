
#include <graphics.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define FLOWER_SUM 10
#define IMGWIDTH 250
#define IMGHEIGHT 200
#define MOVELENGTH 20
#define MAXPATH 1024

IMAGE imgbg, imgflower[10], imgflowerbg[10];

struct flowerPosition {
	int x[10];
	int y[10];
	int width[10];
	int height[10];
	int climb[10];
	int lastY[10];
}flower[2];
void flyScreen();
void paintFlowers();
unsigned WINAPI threadFly(void *arg);
void getRes();

int main(int argc, char *argv[]) {
	
	int w,n;
	HANDLE thread1;
	unsigned threadID;
	int a = 0;
	initgraph(WIDTH,HEIGHT,0);
	SetWindowTextA(GetHWnd(),"飘花");
	setbkmode(TRANSPARENT);
	getRes();
	BeginBatchDraw();
	putimage(0,0,&imgbg);
	putimage(0,0,&imgflowerbg[0],SRCAND);
	putimage(0,0,&imgflower[0],SRCPAINT);
	FlushBatchDraw();
	EndBatchDraw();
	//开始一个线程
	thread1 = (HANDLE)_beginthreadex(NULL,0,threadFly,(void *)&a,0,&threadID);
	if (threadID == 0){
		MessageBoxA(GetHWnd(),"没有线程","TIP",MB_OK);
	}
	w = 1;
	MessageBoxA(GetHWnd(), "主线程", "TIP", MB_OK);
	while (1){
		for ( n = 0; n< w; n++){
			outtextxy(WIDTH/2+n*10,HEIGHT/2,L"5");
		}
		w++;
		Sleep(500);
		if (w == 6){
			outtextxy(WIDTH/2,HEIGHT/2,L"     ");
			w = 1;
		}
	}
	closegraph();
	return 0;
}

void getRes() {
	loadimage(&imgbg, L"C:\\Users\\zzf\\Desktop\\asa.bmp", 0, 0, false);
	WCHAR path[MAXPATH];
	WCHAR a[] = L"hb0.bmp";// i = 3
	WCHAR b[] = L"hb0b.bmp";
	
	//strcpy(path,"H:\\taohua\\");
	for (int i = 0; i < FLOWER_SUM; i++) {
		wcscpy(path, L"H:\\taohua\\");
		imgflower[i] = NULL;
		a[2] = '0' + i;
		b[2] = '0' + i;
		wcscat(path,a);
		loadimage(&imgflower[i], path, 0, 0, false);
		wcscpy(path, L"H:\\taohua\\");
		wcscat(path,b);
		loadimage(&imgflowerbg[i],path, 0, 0, false);
		flower[0].width[i] = imgflower[i].getwidth();
		flower[0].height[i] = imgflower[i].getheight();
		if (&imgflower[i] == NULL){
			MessageBoxA(GetHWnd(),"没有加载到img","IPS",MB_OK);
		}
	}
}

unsigned WINAPI threadFly(void *arg) {
	int i;
	const int a = WIDTH / FLOWER_SUM;
	const int b = HEIGHT / FLOWER_SUM;
	//生成初始位置---
	srand((unsigned)time(NULL));
	for ( i = 0; i < FLOWER_SUM; i++){
		flower[0].x[i] = rand()%FLOWER_SUM*a;
		//flower.x[i] =i*a;
		//flower.y[i] = 0;
		flower[0].y[i] = rand() % FLOWER_SUM*b;
		flower[0].climb[i] = 0;
	}
	while (1){
		//输出花朵
		
		paintFlowers();

		
		for ( i = 0; i < FLOWER_SUM; i++){
			flower[0].y[i] += MOVELENGTH;
			if (flower[0].y[i] > HEIGHT){
				flower[0].lastY[i] = flower[0].y[i];
				flower[0].y[i] = 0;
				flower[0].climb[i] = 1;
			}
		}
		//
		//getchar();
		Sleep(300);
	}
	//产生第一幕-------
	return 0;
}
void flyScreen() {
	
}

void paintFlowers() {
	int i;
	static IMAGE imgBeyond[FLOWER_SUM];
	static int first = 1;
	//得到未画之前的底图
	
	BeginBatchDraw();
	//覆盖原先的绘图------清除
	if (!first) {
		for (i = 0; i < FLOWER_SUM; i++) {
			if (flower[0].climb[i]){
				putimage(flower[0].x[i],flower[0].lastY[i],&imgBeyond[i]);
				flower[0].climb[i] = 0;
			}
			else
			putimage(flower[0].x[i], flower[0].y[i]-MOVELENGTH, &imgBeyond[i]);
			//putimage(flower.x[i], flower.y[i], &imgflower[i], SRCPAINT);
			//MessageBoxA(GetHWnd(),"first","tip",MB_OK);
		}
	}
	else {
		first = 0;
		//MessageBoxA(GetHWnd(), "first no", "tip", MB_OK);
	}
	for ( i = 0; i < FLOWER_SUM; i++){
		getimage(&imgBeyond[i],flower[0].x[i],flower[0].y[i],flower[0].width[i],flower[0].height[i]);
	}
	for (i = 0; i < FLOWER_SUM; i++) {
		putimage(flower[0].x[i], flower[0].y[i], &imgflowerbg[i], SRCAND);
		putimage(flower[0].x[i], flower[0].y[i], &imgflower[i], SRCPAINT);
		//putimage(flower.x[i],flower.y[i],&imgflower[i]);
		//outtextxy(flower.x[i],flower.y[i],L"花");
	}
	FlushBatchDraw();
	EndBatchDraw();

}




