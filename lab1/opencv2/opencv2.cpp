// OpenCVDefault.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>     // std::string, std::to_string

using namespace std;
using namespace cv;
#define ENTER  13
#define R 114
#define ESPACE 32
#define TAB 9
#define M 109

//Donnee image
Mat matrice;
Mat matrice_PositionSouris;

//Couleur des pixel
Scalar PipetteColor;
Scalar CurrentPixelColor;

//Position Souris
int MouseX;
int MouseY;
char PositionColorSouris[500];

//affiche text
char MouseText[100];
char ColorText[100];

//Camera
bool CameraMode = false;

//7,8,10) Capteur de la souris
void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{
	MouseX = x;
	MouseY = y;

	if (event == EVENT_LBUTTONDOWN)
	{
		//7) MÉMORISER LA COULEUR DE LA SOURIS
		PipetteColor = CurrentPixelColor;
		//cout << "Left button of the mouse is clicked - position(" << x << "," << y  << ")" << endl;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		//8) DESSINER UNE CROIX AVEC LA PIPETTE
		int CrossSize = 20;

		//Ligne horizontal
		Point P1(x - CrossSize / 2, y);
		Point P2(x + CrossSize / 2, y);

		//Ligne Verticale
		Point P3(x, y - CrossSize / 2);
		Point P4(x, y + CrossSize / 2);

		line(matrice, P1, P2, PipetteColor);
		line(matrice, P3, P4, PipetteColor);

		CameraMode = false;
		imwrite("SnapShot.png", matrice);

		//cout << "Right button of the mouse is clicked - position(" << x << "," << y  << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		//cout << " Mouse over this position(" << x << "," << y  << ")" << endl;

		Scalar Color(
			matrice.at<cv::Vec3b>(MouseY, MouseX)[0],
			matrice.at<cv::Vec3b>(MouseY, MouseX)[1],
			matrice.at<cv::Vec3b>(MouseY, MouseX)[2],
			255
			);
		CurrentPixelColor = Color;
	}
}

void ReleaseImage()
{
	IplImage* src = cvLoadImage("breadfish.png");
	cvReleaseImage(&src);
};
//Prendre une image
void TakePicture(Mat matrice, bool &CameraMode)
{
	imwrite("SnapShot.png", matrice);
	CameraMode = false;
}

//4) DESSINER UNE DIAGONALE ROUGE PARTANT DUNE EXTREMITÉ
//   VERT L'AUTRE
void DrawRedDiagonal(Mat matrice, int Width, int Height)
{
	//Prends les 2 Extremite
	Point P1(0, 0);
	Point P2(Width, Height);

	//Couleur Rouge
	Scalar Color(0, 0, 255);

	line(matrice, P1, P2, Color);
};

int main(int argc, char** argv)
{
	//Image
	int Height;
	int Width;

	//camera
	VideoCapture camera(0);
	bool HaveCamera = camera.isOpened();
	namedWindow("Lab1", 1);

	//key Keyboard
	int key;
	bool Loop = true;

	//1) LOADER UNE SIMPLE IMAGE
	//matrice = imread("breadfish.png",CV_LOAD_IMAGE_COLOR);
	//imshow("LoadSimpleImage",matrice);

	while (Loop)
	{
		key = waitKey(10);

		//capte la souris
		setMouseCallback("Lab1", CallBackFunc, NULL);

		//Avoir la longueur et la largeur de la matrice(Image) en cours
		Height = matrice.rows;
		Width = matrice.cols;

		/****** MODE Camera *******/
		if (CameraMode)
		{
			if (HaveCamera)
			{
				//2) AFFICHE EN CONTINUE UNE IMAGE AVEC LA CAMÉRA
				camera >> matrice;
				//10) ECRIRE LA POSITION DE LA SOURIS SUR L'IMAGE

				int Offset = 50;
				sprintf_s(MouseText, "(%d,%d)", MouseX, MouseY);
				sprintf_s(ColorText, "(%i,%i,%i)", (int)CurrentPixelColor.val[0], (int)CurrentPixelColor.val[1], (int)CurrentPixelColor.val[2]);

				string MT = MouseText;
				string CT = ColorText;
				putText(matrice, MT + "=" + ColorText, Point(Width - (Offset * 8.5), Height - Offset), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255, 0), 1);

				if (key == ENTER)
				{
					//3) PRENDRE UNE PHOTO ET RETOURNER EN MODE PHOTO
					TakePicture(matrice, CameraMode);
				}
				if (key == R)
				{
					TakePicture(matrice, CameraMode);

					//4) DESSINER UNE DIAGONALE ROUGE
					DrawRedDiagonal(matrice, Width, Height);
					imwrite("SnapShot.png", matrice);
				}

				imshow("Lab1", matrice);
			}
			else
			{
				/*Test du changement de mode (lorsque	pas de camera)*/
				matrice = imread("breadfish.png", CV_LOAD_IMAGE_COLOR);
				if (key == ENTER)
					CameraMode = false;
			}
		}
		/******			 *******/

		/****** MODE Image *******/
		else
		{
			matrice = imread("SnapShot.png", CV_LOAD_IMAGE_COLOR);

			if (key == R)
			{
				//4) DESSINER UNE DIAGONALE ROUGE
				DrawRedDiagonal(matrice, Width, Height);
				imwrite("SnapShot.png", matrice);
			}
		}
		/******			 *******/

		imshow("Lab1", matrice);
		/****** MODE Menu *******/
		if (key == M)
		{
			//6) MODE MENU CONSOLE
			string texteEntrer;
			cout << "Nom de l'image a charger? ";
			getline(cin, texteEntrer);
			matrice = imread(texteEntrer, CV_LOAD_IMAGE_COLOR);

			////A voir
			//ReleaseImage();
			imwrite("SnapShot.png", matrice);
		}
		/******			 *******/

		//5) EFFACER L'IMAGE
		if (key == ESPACE)
		{
			ReleaseImage();
		}

		//9) PASSER DU MODE Image AU MODE Camera
		if (key == TAB)
		{
			if (CameraMode)CameraMode = false;
			else CameraMode = true;
		}
	}

	//Liberation des ressources la caméra
	camera.release();
	waitKey(0);
	return 0;
}




