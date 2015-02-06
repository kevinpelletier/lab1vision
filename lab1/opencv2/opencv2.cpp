// OpenCVDefault.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "device_launch_parameters.h"
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
Scalar CouleurPipette;
Scalar CouleurPixelEnCours;

//Position Souris
int MouseX;
int MouseY;
char PositionColorSouris[500];

//affiche text
char TexteSouris[100];
char TexteCouleur[100];

//Camera
bool ModeCamera = true;

//7,8,10) Capteur de la souris
void CallBackFunc(int event, int x, int y, int flags,void *userdata)
{
	MouseX = x;
	MouseY = y;

	if(event == EVENT_LBUTTONDOWN)
	{
		//7) MÉMORISER LA COULEUR DE LA SOURIS
		CouleurPipette = CouleurPixelEnCours;
		//cout << "Left button of the mouse is clicked - position(" << x << "," << y  << ")" << endl;
	}
	else if(event == EVENT_RBUTTONDOWN)
	{
		//8) DESSINER UNE CROIX AVEC LA PIPETTE
		int CrossSize = 20;
		
		//Ligne horizontal
		Point P1 (x - CrossSize/2, y);
		Point P2 (x + CrossSize/2, y);

		//Ligne Verticale
		Point P3 (x, y - CrossSize/2);
		Point P4 (x, y + CrossSize/2);

		line(matrice,P1,P2,CouleurPipette);
		line(matrice,P3,P4,CouleurPipette);
		
		ModeCamera = false;
		imwrite("ImageEnCours.png",matrice);
		
		//cout << "Right button of the mouse is clicked - position(" << x << "," << y  << ")" << endl;
	}
	else if(event == EVENT_MOUSEMOVE)
	{
		//cout << " Mouse over this position(" << x << "," << y  << ")" << endl;

		Scalar Color(
			matrice.at<cv::Vec3b>(MouseY,MouseX)[0],
			matrice.at<cv::Vec3b>(MouseY,MouseX)[1],
			matrice.at<cv::Vec3b>(MouseY,MouseX)[2],
			255
			);
		CouleurPixelEnCours = Color;
	}
}

//Prendre une image
void PrendrePhoto(Mat matrice, bool &ModeCamera)
{
	imwrite("ImageEnCours.png",matrice);
	ModeCamera = false;
}

//4) DESSINER UNE DIAGONALE ROUGE PARTANT DUNE EXTREMITÉ
//   VERT L'AUTRE
void DessineDiagoRouge(Mat matrice, int Largeur, int Hauteur)
{
	//Prends les 2 Extremite
	Point P1(0,0);
	Point P2(Largeur,Hauteur);

	//Couleur Rouge
	Scalar Color(0,0,255);

	line(matrice,P1,P2,Color);	
};

int main(int argc, char** argv)
{
	//Image
	int Hauteur;
	int Largeur;

	//camera
	VideoCapture camera(0);
	bool CameraTrouver = camera.isOpened();
	namedWindow("Lab1",1);

	//key Keyboard
	int key;
	bool Loop = true;

	//1) LOADER UNE SIMPLE IMAGE
	//matrice = imread("breadfish.png",CV_LOAD_IMAGE_COLOR);
	//imshow("LoadSimpleImage",matrice);

	while(Loop)
	{
		key = waitKey(10);
		
		//capte la souris
		setMouseCallback("Lab1",CallBackFunc,NULL);

		//Avoir la longueur et la largeur de la matrice(Image) en cours
		Hauteur = matrice.rows;
		Largeur = matrice.cols;
		
		/****** MODE Camera *******/
		if(ModeCamera)
		{
			if(CameraTrouver)
			{
				//2) AFFICHE EN CONTINUE UNE IMAGE AVEC LA CAMÉRA
				camera >> matrice;
					//10) ECRIRE LA POSITION DE LA SOURIS SUR L'IMAGE

				int Decalage = 50;
				sprintf(TexteSouris, "(%d,%d)", MouseX,MouseY);
				sprintf(TexteCouleur, "(%i,%i,%i)", (int)CouleurPixelEnCours.val[0],(int)CouleurPixelEnCours.val[1],(int)CouleurPixelEnCours.val[2]);
				string MT = TexteSouris;
				string CT = TexteCouleur;
				putText(matrice,MT + "=" + TexteCouleur ,Point(Largeur - (Decalage * 8.5), Hauteur - Decalage),FONT_HERSHEY_COMPLEX,1,Scalar(255,255,255,0),1);

				if(key == ENTER)
				{
					//3) PRENDRE UNE PHOTO ET RETOURNER EN MODE PHOTO
					PrendrePhoto(matrice,ModeCamera);
				}
				if(key == R)
				{
					PrendrePhoto(matrice,ModeCamera);

					//4) DESSINER UNE DIAGONALE ROUGE
					DessineDiagoRouge(matrice,Largeur,Hauteur);
					imwrite("ImageEnCours.png",matrice);
				}

			
				imshow("Lab1",matrice);
			}
			else
			{
				/*Test du changement de mode (lorsque	pas de camera)*/
				matrice = imread("breadfish.png",CV_LOAD_IMAGE_COLOR);
				if(key == ENTER)
					ModeCamera = false;
			}
		}
		/******			 *******/
		
		/****** MODE Image *******/
		else
		{
			matrice = imread("ImageEnCours.png",CV_LOAD_IMAGE_COLOR);

			if(key == R)
			{
				//4) DESSINER UNE DIAGONALE ROUGE
				DessineDiagoRouge(matrice,Largeur,Hauteur);
				imwrite("ImageEnCours.png",matrice);
			}
		}
		/******			 *******/

		/****** MODE Menu *******/
		if(key == M)
		{
			//6) MODE MENU CONSOLE
			string texteEntrer;
			cout << "Nom de l'image a charger? ";
			getline(cin,texteEntrer);
			matrice = imread(texteEntrer, CV_LOAD_IMAGE_COLOR);
			imwrite("ImageEnCours.png",matrice);
			ModeCamera = false;
		}
		/******			 *******/

		//5) EFFACER L'IMAGE
		if(key == ESPACE)
		{
			matrice = NULL;
			imwrite("ImageEnCours.png",matrice);
		}

		//9) PASSER DU MODE Image AU MODE Camera
		if(key == TAB)
		{
			if(ModeCamera)ModeCamera = false;
			else ModeCamera = true;
		}

		imshow("Lab1",matrice);
	}

	//Liberation des ressources la caméra
	camera.release();
	waitKey(0);
	return 0;
}




