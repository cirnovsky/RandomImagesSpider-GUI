#include <windows.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <io.h>
#include <graphics.h>

const std::string URL_TABLE[] = {
	"https://api.ixiaowai.cn/mcapi/mcapi.php",
	"http://api.mtyqx.cn/tapi/random.php",
	"http://www.dmoe.cc/random.php",
	"https://img.paulzzh.tech/touhou/random"
};

const int WRITE_WIDTH = 750;
const int WRITE_HEIGHT = 415;
const int GRAPH_WIDTH = 800;
const int GRAPH_HEIGHT = 600;
const int URLTABLE_SIZE = 4;

void URLImageDownloadToFileWithCertutil(std::string url, int identity) {
	std::string command;
	command += "certutil -urlcache -split -f ";
	command += url;
	command += " \"";
	command += std::to_string(identity); 
	command += ".png\"";
	std::cout << command << std::endl;
	system(command.c_str());
}

void URLImageDownloadToFileWithVBS(std::string url, int identity) {
	std::string command;
	command += "cscript download.vbs ";
	command += url;
	command += " \"";
	command += std::to_string(identity);
	command += ".png\"";
	std::cout << command << std::endl;
	system(command.c_str());
}

void URLImageDownloadToFileWithPowerShellExec(std::string url, int identity) {
	std::string command;
	command += "powershell -exec bypass -c (new-object System.Net.WebClient).DownloadFile('";
	command += url;
	command += "','";
	command += std::to_string(identity);
	command += ".png')";
	std::cout << command << std::endl;
	system(command.c_str());
}

void URLImageDownloadToFileWithPowerShellWGet(std::string url, int identity) {
	std::string command;
	command += "powershell Invoke-WebRequest -Uri \"";
	command += url;
	command += "\" -OutFile \"";
	command += std::to_string(identity);
	command += ".png\"";
	std::cout << command << std::endl;
	system(command.c_str());
}

int GetImageFileSizeToCheckDelete(std::string filename) {
	FILE *fp = fopen(filename.c_str(), "r");
	if (!fp) return -1;
	fseek(fp, 0L, SEEK_END);
	int filesize = ftell(fp);
	fclose(fp);
	return filesize / 1024;
}

void SetImageSize(PIMAGE& image, int WIDTH, int HEIGHT, int x, int y) {
	PIMAGE current = gettarget();
	settarget(image);
	int width, height;
	width = getwidth();
	height = getheight();
	settarget(current);
	PIMAGE tempImage = newimage(WIDTH, HEIGHT);
	putimage(tempImage, 0, 0, WIDTH, HEIGHT, image, 0, 0, width, height);
	getimage(image, tempImage, 0, 0, WIDTH, HEIGHT);
	delimage(tempImage);
}

void ShowImageAtXY(int imageID, int x, int y) {
	std::string filename = std::to_string(imageID) + ".png";
	PIMAGE image = newimage();
	getimage(image, filename.c_str(), 0, 0);
	SetImageSize(image, WRITE_WIDTH, WRITE_HEIGHT, x, y);
	putimage(x, y, WRITE_WIDTH, WRITE_HEIGHT, image, 0, 0);
	delimage(image);
}

int main() {
	srand(time(0));
	HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
//	if (hwnd) ShowWindow(hwnd, SW_HIDE);
	initgraph(GRAPH_WIDTH, GRAPH_HEIGHT, 0);
	setcaption("Random images spider by BoringHacker ");
	int beginID = 0, imagesNumber = 0, chooseDownloader = 0, chooseModel = 0;
	char inputInformation[100];
	memset(inputInformation, 0, sizeof inputInformation);
	inputbox_getline("Information!",
					"Input beginID and imagesNumber(0<imagesNumber,separated by spaces):",
					inputInformation, sizeof(inputInformation) / sizeof(*inputInformation));
	int size = strlen(inputInformation), isCatchedSpace;
	for (int i = 0; i < size; ++i) {
		if (inputInformation[i] == ' ') isCatchedSpace = 1;
		else {
			if (isCatchedSpace == 0) beginID = beginID * 10 + (inputInformation[i] - '0');
			else imagesNumber = imagesNumber * 10 + (inputInformation[i] - '0');
		}
	}
	memset(inputInformation, 0, sizeof inputInformation);
	inputbox_getline("Information!",
					"Choose downloader:\n1.Windows Command Certutil\n2.Windows Command with \"download.vbs\"\n3.Windows Power Shell 1\n4.Windwos Power Shell 2",
					inputInformation, sizeof(inputInformation) / sizeof(*inputInformation));
	chooseDownloader = (*inputInformation) - '0';
	memset(inputInformation, 0, sizeof inputInformation);
	inputbox_getline("Information!",
					"Choose model:\n1.Browse Model\n2.Batch Download Model",
					inputInformation, sizeof(inputInformation) / sizeof(*inputInformation));
	chooseModel = (*inputInformation) - '0';

	for (int i = beginID; i < beginID + imagesNumber; ++i) {
		switch (chooseDownloader) {
			case 1:
				URLImageDownloadToFileWithCertutil(URL_TABLE[rand() % URLTABLE_SIZE], i);
				break;
			case 2:
				URLImageDownloadToFileWithVBS(URL_TABLE[rand() % URLTABLE_SIZE], i);
				break;
			case 3:
				URLImageDownloadToFileWithPowerShellExec(URL_TABLE[rand() % URLTABLE_SIZE], i);
				break;
			case 4:
				URLImageDownloadToFileWithPowerShellWGet(URL_TABLE[rand() % URLTABLE_SIZE], i);
				break;
			default: break;
		}
		
		std::string filename = std::to_string(i);
		filename += ".png";
		if (GetImageFileSizeToCheckDelete(filename) <= 50) {
			std::string command = "del ";
			command += filename;
			system(command.c_str());
			--i;
			puts("log:Download failed,retried");
		}
		else {
			ShowImageAtXY(i, (GRAPH_WIDTH - WRITE_WIDTH) / 2, (GRAPH_HEIGHT - WRITE_HEIGHT) / 2);
			if (chooseModel == 1) getch();
			cleardevice();
		}
	}
	getch();
	closegraph();
}
