#include "ventana.h"



LRESULT CALLBACK winproc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

int CVentana::Crear_Ventana(int _ancho,int _alto,int pos_x,int pos_y,bool _fullscreen,int profundidad_color)
{
	if(estado == INACTIVO)
	{
		DWORD dwExStyle;
	    DWORD dwStyle;
	    RECT  window_rect;
	    GLuint PixelFormat;
	
	    fullscreen=_fullscreen;
	    ancho=_ancho;   //Se copia el alto y el ancho para mantenerlo como variables globales
	    alto=_alto;     //alto
	
   		window_rect.left=(long)0;		
		window_rect.right=(long)ancho;	//Se inicializa el rect_angulo de la ventana	
		window_rect.top=(long)0;		
		window_rect.bottom=(long)alto;	
	  
	    
	    hinstance = GetModuleHandle(NULL);
	    
	    wnd.style          =CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  //Aqui se llenan los datos de la clase ventana
		wnd.lpfnWndProc    =(WNDPROC) winproc;
	    wnd.cbClsExtra     =0;
	    wnd.cbWndExtra     =0;
	    wnd.hInstance      =hinstance;
	    wnd.hIcon          =LoadIcon(NULL,IDI_WINLOGO);
	    wnd.hCursor        =LoadCursor(NULL,IDC_ARROW);
	    wnd.hbrBackground  =NULL;
	    wnd.lpszMenuName   =NULL;
	    wnd.lpszClassName  ="Pong";
	    
		if(!RegisterClass(&wnd)) //Se registra la clase y se verifican ESTADO_ERRORes
		{
			MessageBox(NULL,"Falla al registrar la clase de la ventana.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
	    
		if(fullscreen) //se verifica el parametro fullscreen y se ajustan los parametros
		{
			DEVMODE dmScreenSettings;								
    		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
			dmScreenSettings.dmPelsWidth	= ancho;				
			dmScreenSettings.dmPelsHeight	= alto;			
			dmScreenSettings.dmBitsPerPel	= profundidad_color;				
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)//se pide el cambio a fullscreen y se verifica ESTADO_ERRORes
			{
				if (MessageBox(NULL,"El modo No es soportado por la tarjeta | opengl, pasar a modo window?","Ventana gl",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=FALSE;		
				}
				else
				{
					MessageBox(NULL,"El programa se cerrará.","ESTADO_ERROR",MB_OK|MB_ICONSTOP); //no se pudo cambiar el modo grafico a pantalla completa
					estado=ESTADO_ERROR;
					return estado;
				}
			}
		}    
		if(fullscreen)//si el parametro fullscreen sigue siendo true
		{
			dwExStyle=WS_EX_APPWINDOW;
			dwStyle=WS_POPUP;
			pos_x=0;
			pos_y=0;
			 ShowCursor(FALSE);
		}
		else  // si no se solicita una ventana en modo normal
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle=WS_OVERLAPPEDWINDOW;
			 ShowCursor(FALSE);
	    }
	    AdjustWindowRectEx(&window_rect, dwStyle, FALSE, dwExStyle); //se ajusta el tamaño y estilo del borde	
	    
		if (!(hwnd=CreateWindowEx(	dwExStyle,	                 //Funcion de creacion de la ventana con verificacion de ESTADO_ERROR					
									"Pong",					
									"Pong",					
									dwStyle |							
									WS_CLIPSIBLINGS |					
									WS_CLIPCHILDREN,					
									pos_x, pos_y,						
									window_rect.right-window_rect.left,	
									window_rect.bottom-window_rect.top,	
									NULL,								
									NULL,								
									hinstance,							
									NULL)))								
		{
			Destruir_Ventana();	  //Si fallo se destruye la ventana y se muestra mensaje de ESTADO_ERROR									 								
			MessageBox(NULL,"ESTADO_ERROR de creacion de la ventana.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
		
		static	PIXELFORMATDESCRIPTOR pfd=	 //se eligen los parametros del pixel format descriptor			
		{
			sizeof(PIXELFORMATDESCRIPTOR),				
			1,											
			PFD_DRAW_TO_WINDOW |						
			PFD_SUPPORT_OPENGL |				//creacion de una ventana doble buffer, rgba, con el 		
			PFD_DOUBLEBUFFER,					//parametro profundidad_color que recibe la funcion		
			PFD_TYPE_RGBA,						//se solicita compatible con gl,etc,etc		
			profundidad_color,							
			0, 0, 0, 0, 0, 0,							
			0,											
			0,											
			0,											
			0, 0, 0, 0,									
			16,									//profundidad del buffer stencil		
			0,											
			0,											
			PFD_MAIN_PLANE,								
			0,											
			0, 0, 0										
		};
		
		if (!(hdc=GetDC(hwnd)))	//se solicita un dc para la ventana					
		{
			this->Destruir_Ventana();			//si falla mensaje de ESTADO_ERROR y destruye la ventana			
			MessageBox(NULL,"No se pudo crear un contexto gl.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
	
		if (!(PixelFormat=ChoosePixelFormat(hdc,&pfd)))	//se solicitan los parametros para el dc con el pdf
		{
			this->Destruir_Ventana();						//si falla mensaje de ESTADO_ERROR y destruye la ventana
			MessageBox(NULL,"No se pudo encontrar el formato de pixeles.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
	
		if(!SetPixelFormat(hdc,PixelFormat,&pfd))		//se establecen los parametros para el dc
		{
			this->Destruir_Ventana();				//si falla mensaje de ESTADO_ERROR y destruye la ventana		
			MessageBox(NULL,"No se pudo establecer el formato de pixeles.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
	
		if (!(hrc=wglCreateContext(hdc)))	   		//creacion del contexto de Opengl contenido en el hglrc hrc	
		{
		    this->Destruir_Ventana();						//si falla mensaje de ESTADO_ERROR y destruye la ventana
			MessageBox(NULL,"No se pudo establecer un contexto gl.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;
		}
	
		if(!wglMakeCurrent(hdc,hrc))					//Se trata de activar el contexto gl hrc 
		{
			this->Destruir_Ventana();				            //si falla mensaje de ESTADO_ERROR y destruye la ventana
			MessageBox(NULL,"No se pudo activar el contexto gl.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;					
		}
	
		ShowWindow(hwnd,SW_SHOW);			//se muestra la ventana ya creada
		SetForegroundWindow(hwnd);			
		SetFocus(hwnd);						//se pide foco del usuario sobre la ventana
		
		estado=ACTIVO;  // Se concluyo exitosamente la creacion de la ventana
		
		Cambiar_Tamano_GL(ancho,alto);			//se ajusta el hrc de gl a la ventana
	
		if (!Inicializar_GL())									// Inicializacion de opengl
		{
			this->Destruir_Ventana();								//si falla mensaje de ESTADO_ERROR y destruye la ventana
			MessageBox(NULL,"Inicializacion fallida.","ESTADO_ERROR",MB_OK|MB_ICONEXCLAMATION);
			estado=ESTADO_ERROR;
			return estado;						
		}
	}
	return estado;

}



//Funcion encargada de procesar los mensajes de windows

LRESULT CALLBACK winproc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)			
{
	switch (uMsg)									// Evalua los mensajes entrantes
	{
		case WM_SYSCOMMAND:							// Intercepta comandos del sistema
		{
			switch (wParam)							// Verifica el mensaje
			{
				case SC_SCREENSAVE:					// screensaver intentando aparecer
				case SC_MONITORPOWER:				// Monitor entrando a modo bajo consumo energia
				return 0;							// evitar que ocurra
			}
			break;									// salir
		}

		case WM_CLOSE:								// Un mensaje de salida
		{
  			PostQuitMessage(0);					// Enviando un mensaje de salida
//			destruir_ventana();                 // Esto hay que hacerlo manualmente, NO OLVIDAR
     		return 0;								// Devolverse
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);  //Interprete por defecto de windows
}

void CVentana::Cambiar_Tamano_GL(GLsizei ancho, GLsizei alto)	//funcion que cambia el viewport de gl segun ancho,alto
{
	if(estado == ACTIVO)
	{
		if (alto==0)									
		{
			alto=1;										
		}
		glViewport(0,0,ancho,alto);			
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();			
		gluPerspective(45.0f,(GLfloat)ancho/(GLfloat)alto,0.1f,100.0f);
		glMatrixMode(GL_MODELVIEW);							
		glLoadIdentity();									
	}
}

bool CVentana::Inicializar_GL(GLvoid)			//Funcion de inicializacion de Opengl interna de la libreria, para propositos mas especificos, cambiar los parametros por fuera de esta funcion					
{

		glShadeModel(GL_SMOOTH);							
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
		glClearDepth(1.0f);									
		glEnable(GL_DEPTH_TEST);							
		glDepthFunc(GL_LEQUAL);								
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
		return TRUE;									

}



//Funcion de Destruccion de ventana y liberacion de recursos
void CVentana::Destruir_Ventana(void)								
{
	if(estado== ACTIVO)
	{
		if (fullscreen)										// Esta la ventana en modo fullscreen
		{
			ChangeDisplaySettings(NULL,0);					// Se regresa al modo normal de Windows
			ShowCursor(FALSE);								// Se muestra el puntero del mouse
		}

		if (hrc)											// Si se establecio un  contexto de render
		{	
			if (!wglMakeCurrent(NULL,NULL))					// Se hace "actual"
			{
				MessageBox(NULL,"Liberacion de rc y dc fallo.","SHUTDOWN ESTADO_ERROR",MB_OK | MB_ICONINFORMATION);
				estado=ESTADO_ERROR;
			}
	
			if (!wglDeleteContext(hrc))						// Se libera y se revisa por ESTADO_ERROR
			{
				MessageBox(NULL,"Fallo liberando el contexto de render.","SHUTDOWN ESTADO_ERROR",MB_OK | MB_ICONINFORMATION);
				estado=ESTADO_ERROR;
			}
			hrc=NULL;										// Se estable la variable como nula
		}	

		if (hdc && !ReleaseDC(hwnd,hdc))					// Se trata de liberar el dc y se revisa por ESTADO_ERROR
		{
			MessageBox(NULL,"Fallo liberando el Device Context .","SHUTDOWN ESTADO_ERROR",MB_OK | MB_ICONINFORMATION);
			hdc=NULL;										// Se hace la variable hdc nula
			estado=ESTADO_ERROR;
		}
		if (hwnd && !DestroyWindow(hwnd))					// Se destruye la instancia de la ventana y se revisa por ESTADO_ERROR
		{
			MessageBox(NULL,"No se pudo liberar el HWND.","SHUTDOWN ESTADO_ERROR",MB_OK | MB_ICONINFORMATION);
			hwnd=NULL;										// Se deja la variable tipo HWND en nulo
			estado=ESTADO_ERROR;
		}
	
		if (!UnregisterClass("Pong",hinstance))			//Se quita el registro de la clase de la ventana
		{
			MessageBox(NULL,"No se pudo quitar el registro de la ventana.","SHUTDOWN ESTADO_ERROR",MB_OK | MB_ICONINFORMATION);
			hinstance=NULL;									// Se hace nula la instancia de la ventana
			estado=ESTADO_ERROR;
		}
		
		estado=INACTIVO;
	}
	
}

//Ciclo de mensaje, a diferencia del winproc, esta no es una funcion Callback
//y permite el paso al render de gl y a otras funciones del videojuego

bool CVentana::Ciclo_Mensaje(void)
{
        MSG msg;
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa un mensje y lo remueve de la lista de espera de mensajes
		{
			if (msg.message==WM_QUIT)				// se pregunta por el mensaje de salida
			{
				return true;							// retorna true
			}
			else									// Si no 
			{
				TranslateMessage(&msg);				// traduce el mensaje
				DispatchMessage(&msg);             // Y lo envia a nuesro despachador de mensajes
                return false;				        //retorna false
			}
		}
		else										// Si no hay mensaje
		{
		          
			return false;                            //retorna false
			
		}
}

void CVentana::Limpiar_Objeto(void)
{
	if(estado==ERROR)
	{
		alto=0;
		ancho=0;
		fullscreen=false;
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(hrc);
		hrc=NULL;
		ReleaseDC(hwnd,hdc);	
		DestroyWindow(hwnd);
		UnregisterClass("Pong",hinstance);
	
		estado=INACTIVO;
	}
}