//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "util.h"

using namespace std;
#define MAX(A, B) ((A) > (B) ? (A) : (B)) // defining single line functions....
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define ABS(A) ((A) < (0) ? -(A) : (A))
#define FPS 10

string *dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
int positions[2][15]; //Making array to store position 
int arr1[2][15]; //Making array to store the alphabets in the rows
int num[30]; //Making array to store the alphabets in the shooter
int random_digit = 0; //to change the alphabet 
float time_game = 120; //making time of 2 min

enum alphabets
{
	AL_A,
	AL_B,
	AL_C,
	AL_D,
	AL_E,
	AL_F,
	AL_G,
	AL_H,
	AL_I,
	AL_J,
	AL_K,
	AL_L,
	AL_M,
	AL_N,
	AL_O,
	AL_P,
	AL_Q,
	AL_R,
	AL_S,
	AL_T,
	AL_U,
	AL_W,
	AL_X,
	AL_y,
	AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = {"a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
				   "k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
				   "x.bmp", "y.bmp", "z.bmp"};
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i)
	{

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0)
		{
			int length = data.size();
			ofile.write((char *)&length, sizeof(int));
		}
		ofile.write((char *)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();
}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile)
	{
		///	cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char *)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i)
	{
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char *)&data[0], sizeof(char) * length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
				  int cheight = 60)
/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight / height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet()
{
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int &cx, int &cy)
{
}
void Cell2Pixels(int cx, int cy, int &px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight / height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */
void DisplayFunction()
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	time_game -= 0.1; //decrementing the time
	glClearColor(1 /*Red Component*/, 1.0 /*Green Component*/,
				 1.0 /*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT);								 //Update the colors

	//write your drawing commands here or call your drawing functions...

	bool shot_hit = false; //Making a bool for shot hit
	int diameter = 60; //As the diameter of every circle is 60 that why
	int j = 15; //For the new row
	for (int i = 0; i < 15; i++) 
	{
		if (bsx > diameter * i && bsx < diameter * i + 60) //The x-axis value of every circle so that when mouse clicked only in the circle it should burst
		{

			if (num[random_digit] == arr1[0][i] && (bsy > 70 && bsy < 130)) //The y-axis of every circle of row 1 so that when mouse clicked only in the circle it should burst and burst if number of shooter alphabet == the alphabet circle on top
			{
				positions[0][i] = 100000; //Making the postion of circle a number out of the board assuming it burst
				shot_hit = true; //Shot hit is true
				score++; //Incrememnting the score after every burst
				break; //Breaking the statement
			}
			else if (bsy > 130 && bsy < 190)//The y-axis of every circle of row 2 so that when mouse clicked only in the circle it should burst 
			{
				if (num[random_digit] == arr1[1][i]) //burst if number of shooter alphabet == the alphabet circle on top
				{
					positions[1][i] = 100000;//Making the postion of circle a number out of the board assuming it burst
					shot_hit = true; //Shot hit is true
					score++; //Incrememnting the score after every burst
					break; //Breaking the statement
				}

				j++; //to increment next circle
			}
		}
	}

	bsx = 99999;
	bsy = 99999;

	for (int i = 0; i < 15; i++) //Displaying the first row
	{
		DrawAlphabet((alphabets)arr1[0][i], positions[0][i], (height * 8) / 10, awidth, aheight); //Display function

	}

	for (int i = 0; i < 15; i++) //Displaying the second row
	{
		DrawAlphabet((alphabets)arr1[1][i], positions[1][i], (height * 7) / 10, awidth, aheight); //Display function
	}


	if (shot_hit == true) //If alphabet burst new alphabate appear through this
	{
		//change alphabet
		DrawAlphabet((alphabets)num[random_digit], 460, 10, awidth, aheight); //Displaying shooter alphabet
		random_digit++; //Incrementing the alphabet
		shot_hit = false; //Shot hit is false
	}
	else //If alphabet doesnt burst same alphabet appear
	{
		DrawAlphabet((alphabets)num[random_digit], 460, 10, awidth, aheight); ////Displaying shooter alphabet
	}

	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(width / 2 - 30, height - 25, width, height,
			   "Time Left:" + Num2Str(time_game) + " secs", colors[RED]); //time_game implementing overhere 
	

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height)
{
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
	{
		// what to do when left key is pressed...
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/)
	{
	}
	else if (key == GLUT_KEY_UP /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/)
	{
	}
	else if (key == GLUT_KEY_DOWN /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/)
	{
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y)
{
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
	//
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_DOWN)
		{
			bsx = x; //Storing x-axis of board in bsx variable
			bsy = y; //Storing y-axis of board in bsx variable
		}
		else
		{
			bsx = 100000;
			bsy = 100000;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		
	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == KEY_ESC /* Escape key ASCII*/)
	{
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m)
{

	
	if(score == 30)//When score is 30 end game
	{
		cout<<endl;
		cout<<"----GAME OVER----"<<endl; //Displaying gameover 
		cout<<"----Score = "<<score<<"----"<<endl; //Displaying the score
		exit(0); //exiting code
	}
	if(time_game <= 0)//When time is 120 end game
	{
		cout<<endl;
		cout<<"----GAME OVER-----"<<endl; //Displaying gameover 
		cout<<"----Score = "<<score<<"----"<<endl;  //Displaying the score
		exit(0); //exitng code
	}



	glutPostRedisplay();
	glutTimerFunc(1000.0/FPS, Timer, m);

}

/*
* our gateway main function>
* */
int main(int argc, char *argv[])
{

	for (int j = 0; j < 2; j++)//Storing the y-axis location of the alphabets in array
	{
		positions[j][0] = 0; 
		for (int i = 1; i < 15; i++)//Storing the y-axis location of the alphabets in array
		{
			positions[j][i] = 60 + positions[j][i - 1];
		}
	}
	srand(time(0)); // So that the alphabets are stored not in a pattern
	for (int j = 0; j < 2; j++) //Storing the alphabets in array
	{

		for (int i = 0; i < 15; i++)
		{

			arr1[j][i] = rand() % 26; //storing random values in array
		}
	}
	for (int i = 0; i < 15; i++)//Storing the values of 1st row in the shooter array
	{
		num[i] = arr1[0][i];
	}
	int j = 15;
	for (int i = 0; i < 15; i++)//Storing the values of 1st row in the shooter array
	{
		num[j] = arr1[1][i];
		j++;
	}

	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary

	//for (int i = 0; i < 5; ++i)
	//	cout << " word " << i << " =" << dictionary[i] << endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets

	glutInit(&argc, argv);						  // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50);				  // set the initial position of our window
	glutInitWindowSize(width, height);			  // set the size of our window
	glutCreateWindow("ITCs Word Shooter");		  // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction);  // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys);   // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0 / FPS, Timer, time_game);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
