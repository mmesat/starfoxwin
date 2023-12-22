#include "fuente.h"



#define MAX_CHARS	256	
#define FONT_HEIGHT	32


void CFuenteGL::CreateOpenGLFont(LPSTR strFontName, int height)	// Funcion para construir nuestra funte
{
	// Aqui se generan las display list para guardar cada caracter de la fuente
	// Esta funcion retorna un puntero que en este caso será 1, puesto que
	// nosostros no hemos creado previamente una lista. Si se creara un ID despues, este sería 257
	// porque al menos ya hemos usado 256 (de los 256 caracter de MAX_CHARS)
	g_FontListID = glGenLists(MAX_CHARS);					// Se generan las listas para la fuente

	// Ahora es necesario crear la fuente, para esto se usa la funcion windows
	// CreateFont() que retorno un manejador de fuentes (HFONT).  Nuestra funcion CreateOpenGLFont()
	// nos permite pasarle el tamaño y nombre.  Para razones de simplificacion, se dejan
	// otras opciones fuera, pero las puede agregar posteriormente (como bold, italic, ancho..)

	hOldFont = CreateFont( height,							// El alto de la fuentes
						0,								// El ancho (0 lo ajusta de acuerdo con el alto)
						0,								// El angulo de escape
						0,								// El angulo de orientacion
						FW_BOLD,						// El grosor de la fuente(BOLD en este caso)
						FALSE,							// Italic, falso en este caso
						FALSE,							// Underline - No se quiere underline
						FALSE,							// Strikeout - no se quiere strikeout
						ANSI_CHARSET,					// Este es el tipo de caracteres (ANSI)
						OUT_TT_PRECIS,					// La presicion de la salida
						CLIP_DEFAULT_PRECIS,			// La presicion de recortado
						ANTIALIASED_QUALITY,			// La calidad de la fuente (suavisada)
						FF_DONTCARE|DEFAULT_PITCH,		// La familia y el tono de la fuente
						strFontName);					// El nombre de la fuente (Arial, Times, etc.)

	// Ahora que se ha creado la fuente, es necesario asociarla con nuestro hdc
	// Se guarda la fuente antigua para luego eliminarla, con el fin de evitar memory leaks
	hOldFont = (HFONT)SelectObject(myVentana->hdc, hOldFont);

	// Esta funcion hace la magia, toma la fuente actual asociada
	// al hdc y hace de cada caracter un bitmap de la display list.  Estos son llamados glyphos.
	// El primer parametro es el hdc que tiene asociada la fuente
	// El segundo parametro es el valor ASCII desde el cual iniciar
	// El segundo parametro es el valor ASCII donde terminar (MAX_CHARS - 1 puesto que la display list empieza de 1 y aqui empezamos de 0)
	// El ultimo parametro es el ID de la display list donde se va a almacenar  la fuente GL

	wglUseFontBitmaps(myVentana->hdc, 0, MAX_CHARS - 1, g_FontListID);	// Construir 256 caracteres bitmap
						
}



void CFuenteGL::PositionText( int x, int y )
{
	// Si usted utiliza este codigo en sus aplicaciones,
	// debe tener presente que la fuente no esta en un espacion 3d
	// lo que significa que no se puede rotar,escalar,etc.
	// Este codigo es util cuando usted quiere mostrar texto que se encuentra
	// encima de todo lo demás.
	// Esto es bueno para los casos en que se mueve la camara a través de un mundo 3d
	// y no es deseable que el texto se mueva, para hacer este pequeño truco, se hace
	// que el texto siempre se dibuje en frente de la camara.
	// Para hacer esto es necesario determinar una "raster position".Esta es la posicion donde Opengl
	// empieza a dibujar.  Debido a que es un punto flotante, esta funcion no es muy intiutiva de usar
	// lo que se hace es crear un nuevo punto de vista (viewport), y siempre se dibuja en (0,0,0)
	// La parte complicada es que como el "y" es una orientacion diferente en Opengl que en Windows 
	// Lo que se hace abajo es matematica sencilla para "voltear" la situacion

	// Antes de crear un nuevo ViewPort es necesario salvar el anterior(donde estan todas las otras cosas que no son este texto)
	// esto salva la correspondiente matriz de transformaciones 
	// y al final de la funcion se restauran
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

	// Aqui se crean las nuevas modelview y transformation matrix para trabajar con ellas
	glMatrixMode( GL_PROJECTION );						// Se elige la matriz de proyeccion
	glPushMatrix();										// se salva
	glLoadIdentity();									// reset a la matriz
	glMatrixMode( GL_MODELVIEW );						// Se selecciona ahora la modelview
	glPushMatrix();										// se salva
	glLoadIdentity();									// reset

	y = myVentana->alto - FONT_HEIGHT - y ;	// Se calcula el "y" windows de pantalla (y=0 es arriba-izquierda en 
	                                    //windows con los positivos hacia abajo  
	                                    //y en opengl es Y=0 abajo-izquierda con los positivos hacia arriba


	glViewport( x - 1, y - 1, 0, 0 );					// Se crea un nuevo viewport donde dibujar


	glRasterPos4f( 0, 0, 0, 1 );	// Se le indica a opengl la nueva posicion inicial de dibujado
                                    //que es 0,0,0 de viewport definido por (x,y) lo que hace que sea  
                                    //justo donde se quiere



	glPopMatrix();										// se restaura la matriz modelview
	glMatrixMode( GL_PROJECTION );						// se elije modo proyeccion
	glPopMatrix();										// se restaura modo proyeccion

	glPopAttrib();										// se restauran los atributos de ambas
}


void CFuenteGL::glDrawText(int x, int y, const char *strString, ...)
{
	char		strText[256];							// esto almacena los caracteres de la cadena
	va_list		argumentPtr;							// aqui se almacenan los punteros a los argumentos


	// Primero se verifica que la cadena no esté vacia
	if (strString == NULL)								
		return;											// no hace nada

	// Se verifican por los argumentos de la cadena
	// Los parametros son los punteros a los argumentos y luego la cadena
	va_start(argumentPtr, strString);				

	
	// Luego se hace el sprintf normal
	vsprintf(strText, strString, argumentPtr);			// Se agregan los argumentos a la cadena

	va_end(argumentPtr);								// Se finalizan y liberan todos los argumentos a la cadena

	// Antes de dibujar el texto se posiciona con la funcion PositionText
	PositionText(x, y);									

	// Se salvan los atributos de la lista que haya en funcionamiento
	glPushAttrib(GL_LIST_BIT);	

	// Se hace de la lista de caracteres la lista base
	glListBase(g_FontListID);							

	// Se llama a nuestras fuentes en modo bitmap
	// y como cada caracter d ela cadena tiene un valor de la lista asociado
	// se llama a las ID de la lista asociadas al valor de la cadenas (los caracteres)
	glCallLists(strlen(strText), GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Se devuelven las propiedades de la display list inicial
}

//funcion que destruye la fuente y libera la memoria
void CFuenteGL::DestroyFont()										
{
	glDeleteLists(g_FontListID, MAX_CHARS);			    	// libera la display lista
	SelectObject(myVentana->hdc, hOldFont);						// se devuelve la vieja fuente a su estado origina (modificado en create font)
}

