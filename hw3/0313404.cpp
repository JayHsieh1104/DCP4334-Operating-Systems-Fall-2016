#include "bmpReader.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>	//line 6
using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int BILINEAR_RATIO;
float ROTATION_ANGLE;
float SHEAR_VERTICAL_DEGREE;
float SHEAR_HORIZONTAL_DEGREE;
char *inputfile_name = "input.bmp";
char *outputfile_name[5] = { "0313404_output1.bmp", "0313404_output2.bmp", "0313404_output3.bmp", "0313404_output4.bmp", "0313404_output5.bmp"};
unsigned char *input_pic, *bilinear_pic, *rotation_pic, *shearVert_pic, *shearHori_pic, *mix_pic, *temp_pic;

unsigned char bilinear(unsigned char *pic, int w, int h, int color)
{
	int pixel = 0;
	int relevant_w = (int)(w / BILINEAR_RATIO);
	int relevant_h = (int)(h / BILINEAR_RATIO);
	int reference_points[4];

	reference_points[0] = pic[3 * (relevant_h * imgWidth + relevant_w) + color];
	reference_points[1] = pic[3 * (relevant_h * imgWidth + (relevant_w + 1)) + color];
	reference_points[2] = pic[3 * ((relevant_h + 1) * imgWidth + relevant_w) + color];
	reference_points[3] = pic[3 * ((relevant_h + 1) * imgWidth + (relevant_w + 1)) + color];

	double t = (w % BILINEAR_RATIO) / BILINEAR_RATIO;
	double u = (h % BILINEAR_RATIO) / BILINEAR_RATIO;
	double ut = u * t;
	int a = reference_points[2] - reference_points[0];
	int b = reference_points[1] - reference_points[0];
	int c = reference_points[0] - reference_points[1] - reference_points[2] + reference_points[3];
	int d = reference_points[0];
	
	pixel = (int)(a * t + b * u + c * ut + d);

	if (pixel < 0) pixel = 0;
	if (pixel > 255) pixel = 255;

	return (unsigned char)pixel;
}

unsigned char rotation(unsigned char *pic, int w, int h, int w_offset, int h_offset, int color)
{
	int pixel = 0;
	double angle = (double)ROTATION_ANGLE * M_PI / 180.0;
	double cos_theta = cos(angle);    
	double sin_theta = sin(angle);
	int relevant_w = w - w_offset;
	int relevant_h = h - h_offset;
	int trans_w = (int)(relevant_w * cos_theta - relevant_h * sin_theta) + w_offset;
	int trans_h = (int)(relevant_h * cos_theta + relevant_w * sin_theta) + h_offset;

	if (trans_w >= 0 && trans_w < w_offset * 2 && trans_h >= 0 && trans_h < h_offset * 2)
		pixel = pic[3 * (trans_h * 2 * w_offset + trans_w) + color];

	return (unsigned char)pixel;
}

unsigned char shear_vertical(unsigned char *pic, int w, int h, int border_w, int border_h, int color)
{
	int pixel = 0;
	int h_offset = (int)(border_w * SHEAR_VERTICAL_DEGREE / 2);
	int trans_w = w;
	int trans_h = (int)(h + w * SHEAR_VERTICAL_DEGREE) - h_offset;

	if (trans_h >= 0 && trans_h < border_h)
		pixel = pic[3 * (trans_h * border_w + trans_w) + color];

	return (unsigned char)pixel;
}

unsigned char shear_horizontal(unsigned char *pic, int w, int h, int border_w, int border_h, int color)
{
	int pixel = 0;
	int w_offset = (int)(border_h * SHEAR_HORIZONTAL_DEGREE / 2);
	int trans_w = (int)(w + h * SHEAR_HORIZONTAL_DEGREE) - w_offset;
	int trans_h = h;

	if (trans_w >= 0 && trans_w < border_w)
		pixel = pic[3 * (trans_h * border_w + trans_w) + color];

	return (unsigned char)pixel;
}
//line 92
#define THREAD_NUM 16  
typedef struct   
{  
    int  start;  
    int  end;  
    pthread_t ppid;  
    pthread_attr_t attr_t;  
}task_struct;  
  
task_struct tasks[THREAD_NUM];  

int nw, nh;
int w_offset, h_offset;

void * bilinear_pic_task(void * arg)  {
	task_struct* t = (task_struct*)arg;  
	for (int i = t->start; i < t->end; i++){
		for (int j = 0; j < nh; j++){
			bilinear_pic[3 * (j * nw + i) + MYRED] = bilinear(input_pic, i, j, MYRED);
			bilinear_pic[3 * (j * nw + i) + MYGREEN] = bilinear(input_pic, i, j, MYGREEN);
			bilinear_pic[3 * (j * nw + i) + MYBLUE] = bilinear(input_pic, i, j, MYBLUE);
		}
	}
	pthread_attr_destroy(&(t->attr_t));  
    pthread_exit(NULL);  
}

void * rss_task(void * arg)  {
	task_struct* t = (task_struct*)arg;  
	for (int i = t->start; i < t->end; i++){
		for (int j = 0; j < nh; j++){
			rotation_pic[3 * (j * nw + i) + MYRED] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYRED);
			rotation_pic[3 * (j * nw + i) + MYGREEN] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYGREEN);
			rotation_pic[3 * (j * nw + i) + MYBLUE] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYBLUE);

			shearVert_pic[3 * (j * nw + i) + MYRED] = shear_vertical(bilinear_pic, i, j, nw, nh, MYRED);
			shearVert_pic[3 * (j * nw + i) + MYGREEN] = shear_vertical(bilinear_pic, i, j, nw, nh, MYGREEN);
			shearVert_pic[3 * (j * nw + i) + MYBLUE] = shear_vertical(bilinear_pic, i, j, nw, nh, MYBLUE);

			shearHori_pic[3 * (j * nw + i) + MYRED] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYRED);
			shearHori_pic[3 * (j * nw + i) + MYGREEN] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYGREEN);
			shearHori_pic[3 * (j * nw + i) + MYBLUE] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYBLUE);
		}
	}
	pthread_attr_destroy(&(t->attr_t));  
    pthread_exit(NULL);  
}

void * temp_task(void * arg)  {
	task_struct* t = (task_struct*)arg;  
	for (int i = t->start; i < t->end; i++){
		for (int j = 0; j < nh; j++){
			temp_pic[3 * (j * nw + i) + MYRED] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYRED);
			temp_pic[3 * (j * nw + i) + MYGREEN] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYGREEN);
			temp_pic[3 * (j * nw + i) + MYBLUE] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYBLUE);
		}
	}
	pthread_attr_destroy(&(t->attr_t));  
    pthread_exit(NULL);  
}

void * mix_task(void * arg)  {
	task_struct* t = (task_struct*)arg;  
	for (int i = t->start; i < t->end; i++){
		for (int j = 0; j < nh; j++){
			mix_pic[3 * (j * nw + i) + MYRED] = shear_horizontal(temp_pic, i, j, nw, nh, MYRED);
			mix_pic[3 * (j * nw + i) + MYGREEN] = shear_horizontal(temp_pic, i, j, nw, nh, MYGREEN);
			mix_pic[3 * (j * nw + i) + MYBLUE] = shear_horizontal(temp_pic, i, j, nw, nh, MYBLUE);
		}
	}
	pthread_attr_destroy(&(t->attr_t));  
    pthread_exit(NULL);  
}

//line 93
int main()
{
	BmpReader* bmpReader = new BmpReader();
	struct timeval  tv1, tv2;
	
	// read cfg file
	FILE* cfg;
	cfg = fopen("cfg.txt", "r");
	fscanf(cfg, "%d", &BILINEAR_RATIO);
	fscanf(cfg, "%f", &ROTATION_ANGLE);
	fscanf(cfg, "%f", &SHEAR_VERTICAL_DEGREE);
	fscanf(cfg, "%f", &SHEAR_HORIZONTAL_DEGREE);

	// timing function 1
	gettimeofday(&tv1, NULL);
	input_pic = bmpReader->ReadBMP(inputfile_name, &imgWidth, &imgHeight);
//line 110
	nw = (int)(imgWidth * BILINEAR_RATIO);
	nh = (int)(imgHeight * BILINEAR_RATIO);
	
	bilinear_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	rotation_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	shearVert_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	shearHori_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	mix_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	temp_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	
	/*	bilinear_pic_task */
	int  step = nw / THREAD_NUM;  
    for (int  i = 0; i < THREAD_NUM; i++) {  
        tasks[i].start = i * step; //分配任務  
        tasks[i].end = tasks[i].start + step;  
        if (i == THREAD_NUM - 1)  
            tasks[i].end += nw % THREAD_NUM;  
		
        pthread_attr_init(&(tasks[i].attr_t));  
        pthread_create(&(tasks[i].ppid),&(tasks[i].attr_t),bilinear_pic_task,( void *)&tasks[i]);  
    }  
    void * status1;  
    for (int  i = 0;i < THREAD_NUM; i++) {  
        pthread_join(tasks[i].ppid,&status1);
    }  
	/*	bilinear_pic_task */
	
	w_offset = (int)(nw / 2);
	h_offset = (int)(nh / 2);
	
	/*	rotation、shearVert、shearHori_task */
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_attr_init(&(tasks[i].attr_t));  
        pthread_create(&(tasks[i].ppid),&(tasks[i].attr_t),rss_task,( void *)&tasks[i]); 
    }  
    void * status2;  
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_join(tasks[i].ppid,&status2);
    } 
	/*	rotation、shearVert、shearHori_task */

	
	
	/*	temp_pic_task */
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_attr_init(&(tasks[i].attr_t));  
        pthread_create(&(tasks[i].ppid),&(tasks[i].attr_t),temp_task,( void *)&tasks[i]);  
    }  
    void * status3;  
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_join(tasks[i].ppid,&status3);
    } 
	/*	temp_pic_task */

	
	
	/*	mix_pic_task */
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_attr_init(&(tasks[i].attr_t));  
        pthread_create(&(tasks[i].ppid),&(tasks[i].attr_t),mix_task,( void *)&tasks[i]); 
    }  
    void * status4;  
    for ( int  i = 0;i < THREAD_NUM;i++) {  
        pthread_join(tasks[i].ppid,&status4);
    } 
	/*	mix_pic_task */
	
	
	bmpReader->WriteBMP(outputfile_name[0], nw, nh, bilinear_pic);
	bmpReader->WriteBMP(outputfile_name[1], nw, nh, rotation_pic);
	bmpReader->WriteBMP(outputfile_name[2], nw, nh, shearVert_pic);
	bmpReader->WriteBMP(outputfile_name[3], nw, nh, shearHori_pic);
	bmpReader->WriteBMP(outputfile_name[4], nw, nh, mix_pic);
//line 177
	free(input_pic);
	free(bilinear_pic);
	free(rotation_pic);
	free(shearVert_pic);
	free(shearHori_pic);
	free(mix_pic);
	free(temp_pic);
	
	// timing function 2
	gettimeofday(&tv2, NULL);
	printf("0313404 %f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec)); //(line 188) modify 0316001 to your student ID

	return 0;
}