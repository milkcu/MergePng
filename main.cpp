#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "png.h"
int mergePng(char * fileBack, char * fileFront, char * fileOut);
int main(void)
{
	mergePng("pic\\back.png", "pic\\front.png", "pic\\out.png");
	return 0;
}
int mergePng(char * fileBack, char * fileFront, char * fileOut)
{
	//fileBack为背景图片，色彩空间为RGB
	//fileFront为前景图片，色彩空间为RGBA
	//fileOut为叠加图片，色彩空间为RGBA
	int height = 480;
	int width = 640;
	png_image image;
	memset(&image, 0, sizeof image);
	image.version = PNG_IMAGE_VERSION;
	image.format = PNG_FORMAT_RGBA;

	png_image_begin_read_from_file(&image, fileBack);
	png_bytep bufBack = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
	png_image_finish_read(&image, NULL/*background*/, bufBack, 0/*row_stride*/, NULL/*colormap*/);
	
	png_image_begin_read_from_file(&image, fileFront);
	png_bytep bufFront = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
	png_image_finish_read(&image, NULL/*background*/, bufFront, 0/*row_stride*/, NULL/*colormap*/);
	
	png_bytep bufOut = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
	
	int countRgb = 0;
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width * 4; j += 4) {
			bufOut[i * width * 4 + j] = bufBack[countRgb++];
			bufOut[i * width * 4 + j + 1] = bufBack[countRgb++];
			bufOut[i * width * 4 + j + 2] = bufBack[countRgb++];
			bufOut[i * width * 4 + j + 3] = 255 - bufFront[i * width * 4 + j + 3];
		}
	}
	
	png_image_write_to_file(&image, fileOut, 0/*convert_to_8bit*/, bufOut, 0/*row_stride*/, NULL/*colormap*/);
	free(bufBack);
	free(bufFront);
	free(bufOut);
	return 0;
}
