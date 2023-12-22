#include "math.h"
#include "ventana.h"
#include "fuente.h"
#include "Input.h"
#include <windows.h>

//float PI = 3.14159f;
float STEP = 0.99999f;

struct Punto2d 
{ 
	float x; 
	float y; 
}; 

struct Puntootro
{
	float x1;
	float y1;
};

/*struct Circulo 
{ 
	Punto2d centro; 
	float radio; 
};*/ 

void RenderScene(void);
/*float distanciaPuntos(Punto2d *puntoA,Punto2d *puntoB); 
bool esPuntoAdentro(Circulo *circulo, Punto2d *punto);
void dibujarCirculo(float radio);*/

CVentana MyVentana;
CInput   MyInput;
CFuenteGL *fuente=NULL;

//Circulo elCirculo;
Punto2d elPunto;
Puntootro mipunto;


int WINAPI WinMain(	HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MyVentana.Crear_Ventana(640,480,120,120,false,32);
	CFuenteGL Myfuente(&MyVentana);
	Myfuente.CreateOpenGLFont("arial",32);
	fuente=&Myfuente;
	
	//Algunas inicializaciones de OpenGL
	gluLookAt(0.0f,0.0f,10.0f,  0.0f,0.0f,0.0f,  0.0,1.0f,0.0f);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glPointSize(4.0f);
	glColor3f(1.0f,1.0f,1.0f);

	//Inicializamos el punto y el circulo
	//elCirculo.centro.x = -1.0f;
	//elCirculo.centro.y = 0.0f;
	//elCirculo.radio    = 2.0f;  //traten de que esto sea > 0 no?

	elPunto.x = 0.0f;
	elPunto.y = 0.0f;
	mipunto.x1 =5.0f;
	mipunto.y1= 5.0f;

	while(MyVentana.Ciclo_Mensaje()==false)
	{
		if(MyInput.Abajo())
		{
			elPunto.y-=STEP;
			mipunto.y1-=STEP;
		}

		if(MyInput.Arriba())
		{
			elPunto.y+=STEP;
		}
		if(MyInput.Derecha())
		{
			elPunto.x+=STEP;
		}
		if(MyInput.Izquierda())
		{
			elPunto.x-=STEP;
		}
		
		RenderScene();

	}
	MyVentana.Destruir_Ventana();
	Myfuente.DestroyFont();
}

/*float distanciaPuntos(Punto2d *puntoA,Punto2d *puntoB) 
{ 
	float distancia_x = puntoB->x - puntoA->x; 
	float distancia_y = puntoB->y - puntoA->y; 
	return sqrt((distancia_x*distancia_x)+(distancia_y*distancia_y)); 
} 

float distanciaPuntosCuadrada(Punto2d *puntoA,Punto2d *puntoB) 
{
	float distancia_x = puntoB->x - puntoA->x; 
	float distancia_y = puntoB->y - puntoA->y; 
	return ((distancia_x*distancia_x)+(distancia_y*distancia_y));
}/*

//bool esPuntoAdentro(Circulo *circulo, Punto2d *punto) 
//{ 
	//Este if muestra el equivalente del inferior, solo que con el calculo de la distancia cuadrada,
	//Para un mejor rendimiento es evidente que el radio cuadrado no debe ser calculado en cada llamado
	//de la funcion
	//if ( distanciaPuntosCuadrada(&(circulo->centro), punto) <= (circulo->radio*circulo->radio)) 

	/*if ( distanciaPuntos(&(circulo->centro), punto) <= circulo->radio) 
	{ 
		return true; 
	} 
	return false; 
} */
float subida =0;
float bajada =10;
void RenderScene(void)
{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_POINTS);
		//PlaySound("sound/corneria.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
		PlaySound("sound/corneria02.wav",NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP);
		glVertex3f(elPunto.x,elPunto.y,0.0f);
		glVertex3f(mipunto.x1,mipunto.y1,0.0f);
		glVertex3f(subida,bajada,0.0f);
		bajada--;
		Sleep(700);
		//delay(100);

		glEnd();
	

		glPushMatrix();
		//glTranslatef(elCirculo.centro.x,elCirculo.centro.y,0.0f);
		//dibujarCirculo(elCirculo.radio);
		glPopMatrix();
		

		/*if(esPuntoAdentro(&elCirculo,&elPunto))
		{
			fuente->glDrawText(10,40,"Esta Adentro");
		}
		else
		{
			fuente->glDrawText(10,40,"Esta Afuera");
		}*/
		

		SwapBuffers(MyVentana.hdc);
}


 
//void dibujarCirculo(float radio)
//{
   /*glBegin(GL_LINE_LOOP);
 
   //for (float i=0.0f; i < 2 * PI ;  i += PI / 20.0f)
   {
      //glVertex2f(cos(i)*radio,sin(i)*radio); 
   }
 
   glEnd();*/
//}