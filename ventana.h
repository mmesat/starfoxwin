#ifndef _CVENTANA_C_
#define _CVENTANA_C_

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#define INACTIVO     0  //Quiere decir que el objeto CVentana no tiene una ventana asociada, esta "vacio"
#define ACTIVO	     1  //El objeto tiene una ventana funcional asociada
#define ESTADO_ERROR 2  //Durante la creacion o destrucccion del objeto se presentó un error



///////////////////////////////////////////////////////////////
////Libreria para crear ventanas de Opengl, las principales ////
////funciones son crear_ventana(...) con los parametros     ////
////alto, ancho, posicion de la ventana en x( solo sirve en ////
////modo "window", en y, fullscreen (true para si fullscreen////
////y la profundidad del color, la libreria contiene avisos ////
////de error para la mayoria de los errores mas comunes de  ////
////creacion de la ventana.  Basada en el código de Jeff    ////
////Moloff de la pagina NEHE. La otra funcion es ciclo_mensa////
////je() que devuelve false si no hay un mensaje de salida  ////
//// y true si lo hay.                                      ////
////Camilo E. Alvarez B. Jotatsu PENTEX 2005                ////
////                                                        ////
////////////////////////////////////////////////////////////////


class CVentana
{
	//Acceso privado a estado, que solo se pueda modificar por acciones
	//dentro de la clase
private:
	int estado;
	//Acceso publico a las variables que se necesitan por fuera de la clase
	//con regularidad
public:
	WNDCLASS  wnd;
	HWND      hwnd;
	MSG       msg;
	HINSTANCE hinstance;
	HDC       hdc;
	HGLRC     hrc;
	bool fullscreen;
	int alto;
	int ancho;

	//Constructor, inicializa el estado del objeto
	CVentana()	{
					estado=INACTIVO;
				};

	int Crear_Ventana(int _ancho,int _alto,int pos_x,int pos_y,bool _fullscreen,int profundidad_color);

	//Cambia el tamaño de la ventna de gl
	void Cambiar_Tamano_GL(GLsizei ancho, GLsizei alto);

	//Inicializa gl, se usa en la creacion de la ventana
	bool Inicializar_GL(GLvoid);

	//Funcion que libera los recursos de la ventana
	void Destruir_Ventana(void)	;

	//Ciclo de observacion de mensajes, el único mensaje especial que toma es WM_QUIT,de encontrarlo retorna
	//true, si no, manda el mensaje al winproc()
	bool Ciclo_Mensaje(void);

	//Acceso al estado del objeto
	int Obtener_Estado(void){return estado;};

	//Limpia el objeto en caso de ESTADO_ERROR,y lo devuelve a INACTIVO 
	//OJO, no es segura la liberacion de memoria
	void Limpiar_Objeto(void);

};

#endif

//Notas adicionales.
//1.Winproc() no se incluyo como procedimiento de la clase porque para que funcione (paso como parámetro a 
//  wnd.lpfnWndProc necesita ser static, lo que conlleva grandes limitaciones, para el futuro, si se necesita un
//  Winproc() personalizado, sobreescribir el de ventana.cpp, o pasar un puntero a una funcion propia a un constructor
//  de la clase ventana
//2.Tener cuidado con Limpiar_Objeto(), no es una funcion muy segura que digamos
//3.Observar el comportamiento de la clase, si existen problemas de escritura de variables
//  por que la mayoria tiene acceso publico, cambiar su acceso, a través de funciones, y
//  hacerlo dependiente del estado del objeto
//4.Cambiar el constructor para pedir el nombre de la clase de la ventana y el título de la misma.

/////////////////

//Cambios con respecto a version 0.2
// 	- Se arreglo un pequeño error Inicializar_GL(...)
// 	  no funcionaba en la funcion de Crear_Ventana(..) porque esta chequeaba si la ventana
//	  estaba activa, pero esta no estaba en este estado si no despues de que se llamaba la funcion
//	  Inicializar_GL(...)		









