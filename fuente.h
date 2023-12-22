#ifndef _FUENTE_H_
#define _FUENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "ventana.h"

class CFuenteGL
{
	public:
	UINT g_FontListID;          //variable que guardara la display list con la fuente
	HFONT hOldFont;                 //variable que guardara la fuente
	CVentana *myVentana;

	// Se pasa al cosntructor la ventana sobre la que se va a crear la fuente
	CFuenteGL(CVentana *_myVentana){myVentana=_myVentana;}; 

	//Funcion que crea la fuente
	void CreateOpenGLFont(LPSTR strFontName, int height);	

	//Funcion para posicionar el texto en el rectangulo de la ventana, no es muy util,
	//porque esto ya lo hace la glDrawText(), llamando a PosicionText() internamente
	void PositionText( int x, int y );

	//Funcion de dibujado de texto
	void glDrawText(int x, int y, const char *strString, ...);

	//Destruye la fuente
	void DestroyFont();

	//Destructor
	~CFuenteGL(){DestroyFont();};
};


#endif
