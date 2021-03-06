//Snow Bros

#include "Main.h"
#include "3ds.h"

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa
BOOL	done=FALSE;			// Variable booleana para salir del ciclo

//int glWidth;
//int glHeight;
infoGame infGame;

CVector posCamV;
CVector dirCamV;
CVector vecCamVPl;
int detectado=0;

//Apuntador para primitivas de cuadricas
GLUquadricObj	*e;

void manejaEventosMain();
void cargaConfiguracion();
void guardaModelo();
void cargaModelo();
void actualizaCajaColPersonaje();

//PART�CULAS -- cambio 1

//Se crea el contenedor para almacenar los datos de cada part�cula (ver estructura particles)
//Al tener almacenados los datos para cada part�cula se puede definir un patron de movimiento y tiempo
//de desvanecimiento que puede ser igual, distinto o ligeramente distinto para cada part�cula
//Contenedor de particulas
particles particle[MAX_PARTICULAS];	// Arreglo de part�culas

//Nombre y ubicaci�n del modelo
#define FILE_NAME1c  "Modelos/stewie_torso.3ds"
#define FILE_NAME2c  "Modelos/stewie_cabeza.3ds"
#define FILE_NAME3c  "Modelos/stewie_piernader.3ds"
#define FILE_NAME4c  "Modelos/stewie_piernaizq.3ds"
#define FILE_NAME5c  "Modelos/stewie_brazoder_a.3ds"
#define FILE_NAME6c  "Modelos/stewie_brazoder_b.3ds"
#define FILE_NAME7c  "Modelos/stewie_brazoizq_a.3ds"
#define FILE_NAME8c  "Modelos/stewie_brazoizq_b.3ds"
#define FILE_NAME9c  "Modelos/stewie_bota.3ds"

//Contenedor de texturas del modelo
CTga textureModel1c[20];
CTga textureModel2c[20];
CTga textureModel3c[20];
CTga textureModel4c[20];
CTga textureModel5c[20];
CTga textureModel6c[20];
CTga textureModel7c[20];
CTga textureModel8c[20];
CTga textureModel9c[20];

CTga textureModel1e[5];

CLoad3DS g_Load3ds;

//Acceso a la estructura que almacena los datos del modelo
t3DModel g_3DModel1c;
t3DModel g_3DModel2c;
t3DModel g_3DModel3c;
t3DModel g_3DModel4c;
t3DModel g_3DModel5c;
t3DModel g_3DModel6c;
t3DModel g_3DModel7c;
t3DModel g_3DModel8c;
t3DModel g_3DModel9c;

t3DModel g_3DModel1e;

//Objeto para acceder a las variables de control del personaje
paramObj player1;

//Objeto para manejo de la c�mara
paramCam camara1;

//Objeto para acceder a la selecci�n de materiales
CMateriales Material;

//- SPLINES -//
CVector puntosdeControl[20]; //Aqu� se almacenar� la posici�n de cada punto de control
int resolucion=60; //Entre cada par de puntos de control se calcularan puntos intermedios para dibujar 5 segmentos
int pSpAct; //Punto calculado actual (indice)
int totalP; //Total de puntos a calcular
int pctAct;  //Punto de control actual
CVector pAct;  //Punto actual calculado en la curva
CVector pSig;  //Punto siguiente a calcular en la curva
CVector DirSp; //Direcci�n del vector sobre la curva

CVector camSpline;
CVector objSpline;

cajaCol cajaPersonaje;
cajaCol cajaEscenario[7];
int numCajas=7;
float altPiso=0.0f;
float altMin=-30.0f;
esferaCol esferaPersonaje;
esferaCol esferaEnemigo;

//Variables del personaje
float Angt1;
float Angt2;
float Angc1;
float Angc2;
float Angbi1;
float Angbi2;
float Angbib;
float Angbd1;
float Angbd2;
float Angbdb;
float Angpizq;
float Angpder;
float Angpi;
float Angpd;

float Xtor;
float Ytor;
float Ztor;

//Contenedor de texturas para el escenario
CTga textura[36];

//Variables para animaciones
const int maxKF1=3; //Num. total de KeyFrames para la secuencia 1 (caminar)
const int maxKF2=4; //Num. total de KeyFrames para la secuencia 2 (patada)
const int maxKF3=3; //Num. total de KeyFrames para la secuencia 3 (saltar)

FRAME KeyFrame1[maxKF1]; //Contenedor para almacenar cada keyframe de la secuencia 1
FRAME KeyFrame2[maxKF2]; //Contenedor para almacenar cada keyframe de la secuencia 2
FRAME KeyFrame3[maxKF3]; //Contenedor para almacenar cada keyframe de la secuencia 3

//Variables auxiliares para animacion
bool play=false; //Bandera para iniciar la animaci�n
int  playIndex=0; //Auxiliar para leer la informaci�n del contenedor de keyframes
int  tipoAnim=1; //Indicador del tipo de animaci�n: 1-caminando

//Variables para iluminacion
//GLfloat LightPos[] = {-250.0f, 80.0f, 5.0f, 1.0f};		// Posici�n de la luz
GLfloat LightPos[] = {-50.0f, 80.0f, 50.0f, 1.0f};		// Posici�n de la luz
GLfloat LightAmb[] = { 0.8f,  0.8f, 0.8f, 1.0f};		// Valores de la componente ambiente
GLfloat LightDif[] = { 0.9f,  0.9f, 0.9f, 1.0f};		// Valores de la componente difusa
GLfloat LightSpc[] = { 0.5f,  0.5f, 0.5f, 1.0f};		// Valores de la componente especular
CVector lightPosition;

GLfloat LightAmb2[] = { 0.3f,  0.3f, 0.3f, 1.0f};		// Valores de la componente ambiente
GLfloat LightDif2[] = { 0.4f,  0.4f, 0.4f, 1.0f};		// Valores de la componente difusa
GLfloat LightSpc2[] = { 0.2f,  0.2f, 0.2f, 1.0f};		// Valores de la componente especular

//Cambios para Fuentes
//Buscar el comentario anterior para ver todos los cambios necesarios
//Acceso a la clase CFont
CFont Font;
CControl controlFunc;
CAudio audioFunc;
CSonido sonidoF[3];
CTimer timerEstados;

modelo datosModelo;

//Cambios para FPS
//Buscar el comentario anterior para ver todos los cambios necesarios
//Variables para el control de fps
static int FPS = 0;
//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

//Cambios para FPS
void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}


GLvoid ReDimensionaEscenaGL(GLsizei width, GLsizei height)	// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	infGame.glWidth=width;
	infGame.glHeight=height;
}

int CargaModelos()
{
	if(!g_Load3ds.Load3DSFile(FILE_NAME1c, &g_3DModel1c, textureModel1c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2c, &g_3DModel2c, textureModel2c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3c, &g_3DModel3c, textureModel3c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4c, &g_3DModel4c, textureModel4c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5c, &g_3DModel5c, textureModel5c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6c, &g_3DModel6c, textureModel6c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7c, &g_3DModel7c, textureModel7c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8c, &g_3DModel8c, textureModel8c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9c, &g_3DModel9c, textureModel9c))
		return 0;
	
	return TRUE;
}

void DescargaModelos()
{
	g_Load3ds.UnLoad3DSFile(&g_3DModel1c, textureModel1c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2c, textureModel2c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3c, textureModel3c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4c, textureModel4c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5c, textureModel5c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6c, textureModel6c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7c, textureModel7c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8c, textureModel8c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9c, textureModel9c);
	
}

void InicializaParametrosdeControl()
{
	//Esta funci�n establece los par�metros como velocidad del objeto y distancia de la c�mara as� como la posici�n y direcci�n iniciales

	player1.visible=true;
	player1.cayendo=true;
	player1.caminando=false;
	player1.dirX=0;
	player1.dirZ=0;

	player1.VelocidadObj=0.3f;
	player1.DistanciaCam=50.0f;
	
	player1.CamaraPosAlt=15.0f;	//Posici�n en y de la c�mara (altura a la que se situa la c�mara)
	player1.CamaraObjAlt=8.0f;	//Posici�n en y del objetivo de la c�mara (altura a la que ve la c�mara)
	player1.AngDir=0.0f;		//Este �ngulo inicial hace que la direcci�n inicial sea paralela al eje Z y con sentido negativo
	player1.AngDirCam=0.0f;		//Este es el �ngulo inicial que define la posici�n de la camara respecto al personaje
	player1.AngObj=90.0f;		//Este valor se elige dependiendo de la orientaci�n con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformaci�n (hacia adonde est� volteando). Se elige un �ngulo tal que al aplicarle
								//una rotaci�n inicial con respecto al eje Y est� viendo hacia la misma direcci�n que la definida por AngDir
		
	player1.PosicionObj=CVector(-80.0f, 0.0f, 10.0f); //Esta es la posici�n inicial del objeto en la escena
	player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
	player1.Direccion.y=0.0f;
	player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

	player1.DireccionCam.x=(float)cos(player1.AngDirCam*PI/180.0f);
	player1.DireccionCam.y=0.0f;
	player1.DireccionCam.z=(float)sin(player1.AngDirCam*PI/180.0f);
	player1.PosicionCam=player1.PosicionObj-player1.DireccionCam*player1.DistanciaCam;
	player1.PosicionCam.y=player1.CamaraPosAlt;

	player1.ObjetivoCam=player1.PosicionObj;		//La c�mara ve siempre al objeto
	player1.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1.escalaX=0.8f;
	player1.escalaY=0.8f;
	player1.escalaZ=0.8f;

	player1.kick=false;
	player1.contAuxAnim=0;
		
}

void InicializaCamara()
{
	camara1.VariacionCam=2.0f;		//simetrico de -1.0 a 1.0)
	camara1.VariacionDist=300.0f;	//simetrico de -150.0 a 150.0

	//como no es simetrico porque va de -200.0 a 100.0 se usa un desplazamiento para la distancia
	camara1.VariacionDespDist=-50.0f;
	camara1.VariacionDespCam=0.0f;	//la variaciond de la camara se mantiene sim�trica

	camara1.FactorCam=camara1.VariacionCam/camara1.VariacionDist;

	float aux=(player1.PosicionObj.x-camara1.VariacionDespDist)*camara1.FactorCam+camara1.VariacionDespCam;

	camara1.PosicionCam.x=player1.PosicionObj.x-30.0f*aux;
	camara1.PosicionCam.y=12.0f;
	camara1.PosicionCam.z=40.0f;

	camara1.ObjetivoCam=player1.PosicionObj;
	camara1.ObjetivoCam.y=player1.PosicionObj.y+5.0f;
}

void IniVarsPlayer()
{
	Angt1=25.0f;   //Torso: rotaci�n en X
	Angt2=0.0f;   //Torso: rotaci�n en Z
	Angc1=-25.0f;   //Cabeza: rotaci�n en X
	Angc2=0.0f;   //Cabeza: rotaci�n en Y
	Angbi1=-100.0f;  //Brazo Izq: rotaci�n en X
	Angbi2=0.0f;  //Brazo Izq: rotaci�n en Z
	Angbib=-30.0f;  //Antebrazo Izq: rotaci�n en X
	Angbd1=120.0f;  //Brazo Der: rotaci�n en X
	Angbd2=0.0f;  //Brazo Der: rotaci�n en Z
	Angbdb=-80.0f;  //Antebrazo Der: rotaci�n en X
	Angpizq=40.0f; //Pierna Izq: rotaci�n en X
	Angpder=-60.0f; //Pierna Der: rotaci�n en X
	Angpi=0.0f;   //Pie Izq: rotaci�n en X
	Angpd=0.0f;   //Pie Der: rotaci�n en X

	Xtor=0.0f;    //Torso: posici�n en X
	Ytor=0.0f;    //Torso: posici�n en Y
	Ztor=0.0f;    //Torso: posici�n en Z
}

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/t1.tga");
	textura[1].LoadTGA("Texturas/t2.tga");
	textura[2].LoadTGA("Texturas/t3.tga");
	textura[3].LoadTGA("Texturas/t4.tga");
	textura[4].LoadTGA("Texturas/t5.tga");
	textura[5].LoadTGA("Texturas/t6.tga");
	textura[6].LoadTGA("Texturas/t7.tga");
	textura[7].LoadTGA("Texturas/t8.tga");
	textura[8].LoadTGA("Texturas/t9.tga");
	textura[9].LoadTGA("Texturas/t10.tga");
	textura[10].LoadTGA("Texturas/t11.tga");
	textura[11].LoadTGA("Texturas/t12.tga");
	textura[12].LoadTGA("Texturas/t13.tga");
	textura[13].LoadTGA("Texturas/t14.tga");
	textura[14].LoadTGA("Texturas/t15.tga");
	textura[15].LoadTGA("Texturas/t16.tga");	
	textura[16].LoadTGA("Texturas/t17.tga");
	textura[17].LoadTGA("Texturas/t18.tga");
	textura[18].LoadTGA("Texturas/t19.tga");
	textura[19].LoadTGA("Texturas/t20.tga");
	textura[20].LoadTGA("Texturas/t21.tga");
	textura[21].LoadTGA("Texturas/t22.tga");
	textura[22].LoadTGA("Texturas/t23.tga");
	textura[23].LoadTGA("Texturas/t24.tga");
	textura[24].LoadTGA("Texturas/t25.tga");
	textura[25].LoadTGA("Texturas/t26.tga");
	textura[26].LoadTGA("Texturas/t27.tga");
	textura[27].LoadTGA("Texturas/t28.tga");
	textura[28].LoadTGA("Texturas/t29.tga");
	textura[29].LoadTGA("Texturas/t30.tga");
	textura[30].LoadTGA("Texturas/img0.tga");
	textura[31].LoadTGA("Texturas/particula.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
	textura[1].Elimina();
	textura[2].Elimina();
	textura[3].Elimina();
	textura[4].Elimina();
	textura[5].Elimina();
	textura[6].Elimina();
	textura[6].Elimina();
	textura[7].Elimina();
	textura[8].Elimina();
	textura[10].Elimina();
	textura[11].Elimina();
	textura[12].Elimina();
	textura[13].Elimina();
	textura[14].Elimina();
	textura[15].Elimina();
	textura[16].Elimina();
	textura[17].Elimina();
	textura[18].Elimina();
	textura[19].Elimina();
	textura[20].Elimina();
	textura[21].Elimina();
	textura[22].Elimina();
	textura[23].Elimina();
	textura[24].Elimina();
	textura[25].Elimina();
	textura[26].Elimina();
	textura[27].Elimina();
	textura[28].Elimina();
	textura[29].Elimina();
	textura[30].Elimina();
	textura[31].Elimina();
}

void InicializaAnim()
{
	//Se inicializan las variables de la secuencia 1
	for(int i=0; i<maxKF1; i++)
	{
		KeyFrame1[i].Angt1=0.0f;
		KeyFrame1[i].Angt2=0.0f;
		KeyFrame1[i].Angc1=0.0f;
		KeyFrame1[i].Angc2=0.0f;
		KeyFrame1[i].Angbi1=0.0f;
		KeyFrame1[i].Angbi2=0.0f;
		KeyFrame1[i].Angbib=0.0f;
		KeyFrame1[i].Angbd1=0.0f;
		KeyFrame1[i].Angbd2=0.0f;
		KeyFrame1[i].Angbdb=0.0f;
		KeyFrame1[i].Angpizq=0.0f;
		KeyFrame1[i].Angpder=0.0f;
		KeyFrame1[i].Angpi=0.0f;
		KeyFrame1[i].Angpd=0.0f;
		KeyFrame1[i].Xtor=0.0f;
		KeyFrame1[i].Ytor=0.0f;
		KeyFrame1[i].Ztor=0.0f;

		KeyFrame1[i].incAngt1=0.0f;
		KeyFrame1[i].incAngt1=0.0f;
		KeyFrame1[i].incAngc1=0.0f;
		KeyFrame1[i].incAngc2=0.0f;
		KeyFrame1[i].incAngbi1=0.0f;
		KeyFrame1[i].incAngbi2=0.0f;
		KeyFrame1[i].incAngbib=0.0f;
		KeyFrame1[i].incAngbd1=0.0f;
		KeyFrame1[i].incAngbd2=0.0f;
		KeyFrame1[i].incAngbdb=0.0f;
		KeyFrame1[i].incAngpizq=0.0f;
		KeyFrame1[i].incAngpder=0.0f;
		KeyFrame1[i].incAngpi=0.0f;
		KeyFrame1[i].incAngpd=0.0f;
		KeyFrame1[i].incXtor=0.0f;
		KeyFrame1[i].incYtor=0.0f;
		KeyFrame1[i].incZtor=0.0f;
	}

	for(int i=0; i<maxKF2; i++)
	{
		KeyFrame2[i].Angt1=0.0f;
		KeyFrame2[i].Angt2=0.0f;
		KeyFrame2[i].Angc1=0.0f;
		KeyFrame2[i].Angc2=0.0f;
		KeyFrame2[i].Angbi1=0.0f;
		KeyFrame2[i].Angbi2=0.0f;
		KeyFrame2[i].Angbib=0.0f;
		KeyFrame2[i].Angbd1=0.0f;
		KeyFrame2[i].Angbd2=0.0f;
		KeyFrame2[i].Angbdb=0.0f;
		KeyFrame2[i].Angpizq=0.0f;
		KeyFrame2[i].Angpder=0.0f;
		KeyFrame2[i].Angpi=0.0f;
		KeyFrame2[i].Angpd=0.0f;
		KeyFrame2[i].Xtor=0.0f;
		KeyFrame2[i].Ytor=0.0f;
		KeyFrame2[i].Ztor=0.0f;

		KeyFrame2[i].incAngt1=0.0f;
		KeyFrame2[i].incAngt1=0.0f;
		KeyFrame2[i].incAngc1=0.0f;
		KeyFrame2[i].incAngc2=0.0f;
		KeyFrame2[i].incAngbi1=0.0f;
		KeyFrame2[i].incAngbi2=0.0f;
		KeyFrame2[i].incAngbib=0.0f;
		KeyFrame2[i].incAngbd1=0.0f;
		KeyFrame2[i].incAngbd2=0.0f;
		KeyFrame2[i].incAngbdb=0.0f;
		KeyFrame2[i].incAngpizq=0.0f;
		KeyFrame2[i].incAngpder=0.0f;
		KeyFrame2[i].incAngpi=0.0f;
		KeyFrame2[i].incAngpd=0.0f;
		KeyFrame2[i].incXtor=0.0f;
		KeyFrame2[i].incYtor=0.0f;
		KeyFrame2[i].incZtor=0.0f;
	}
	
	for(int i=0; i<maxKF3; i++)
	{
		KeyFrame3[i].Angt1=0.0f;
		KeyFrame3[i].Angt2=0.0f;
		KeyFrame3[i].Angc1=0.0f;
		KeyFrame3[i].Angc2=0.0f;
		KeyFrame3[i].Angbi1=0.0f;
		KeyFrame3[i].Angbi2=0.0f;
		KeyFrame3[i].Angbib=0.0f;
		KeyFrame3[i].Angbd1=0.0f;
		KeyFrame3[i].Angbd2=0.0f;
		KeyFrame3[i].Angbdb=0.0f;
		KeyFrame3[i].Angpizq=0.0f;
		KeyFrame3[i].Angpder=0.0f;
		KeyFrame3[i].Angpi=0.0f;
		KeyFrame3[i].Angpd=0.0f;
		KeyFrame3[i].Xtor=0.0f;
		KeyFrame3[i].Ytor=0.0f;
		KeyFrame3[i].Ztor=0.0f;

		KeyFrame3[i].incAngt1=0.0f;
		KeyFrame3[i].incAngt1=0.0f;
		KeyFrame3[i].incAngc1=0.0f;
		KeyFrame3[i].incAngc2=0.0f;
		KeyFrame3[i].incAngbi1=0.0f;
		KeyFrame3[i].incAngbi2=0.0f;
		KeyFrame3[i].incAngbib=0.0f;
		KeyFrame3[i].incAngbd1=0.0f;
		KeyFrame3[i].incAngbd2=0.0f;
		KeyFrame3[i].incAngbdb=0.0f;
		KeyFrame3[i].incAngpizq=0.0f;
		KeyFrame3[i].incAngpder=0.0f;
		KeyFrame3[i].incAngpi=0.0f;
		KeyFrame3[i].incAngpd=0.0f;
		KeyFrame3[i].incXtor=0.0f;
		KeyFrame3[i].incYtor=0.0f;
		KeyFrame3[i].incZtor=0.0f;
	}
}

void DatosAnimacion()
{
	//Secuencia 1 (caminar)
	KeyFrame1[0].Angt1=10.0f;
	KeyFrame1[0].Angt2=0.0f;
	KeyFrame1[0].Angc1=-10.0f;
	KeyFrame1[0].Angc2=0.0f;
	KeyFrame1[0].Angbi1=-50.0f;
	KeyFrame1[0].Angbi2=0.0f;
	KeyFrame1[0].Angbib=0.0f;
	KeyFrame1[0].Angbd1=60.0f;
	KeyFrame1[0].Angbd2=0.0f;
	KeyFrame1[0].Angbdb=0.0f;
	KeyFrame1[0].Angpizq=40.0f;
	KeyFrame1[0].Angpder=-60.0f;
	KeyFrame1[0].Angpi=30.0f;
	KeyFrame1[0].Angpd=40.0f;
	KeyFrame1[0].Xtor=0.0f;
	KeyFrame1[0].Ytor=0.0f;
	KeyFrame1[0].Ztor=0.0f;

	KeyFrame1[1].Angt1=10.0f;
	KeyFrame1[1].Angt2=0.0f;
	KeyFrame1[1].Angc1=-10.0f;
	KeyFrame1[1].Angc2=0.0f;
	KeyFrame1[1].Angbi1=60.0f;
	KeyFrame1[1].Angbi2=0.0f;
	KeyFrame1[1].Angbib=0.0f;
	KeyFrame1[1].Angbd1=-50.0f;
	KeyFrame1[1].Angbd2=0.0f;
	KeyFrame1[1].Angbdb=0.0f;
	KeyFrame1[1].Angpizq=-60.0f;
	KeyFrame1[1].Angpder=40.0f;
	KeyFrame1[1].Angpi=40.0f;
	KeyFrame1[1].Angpd=30.0f;
	KeyFrame1[1].Xtor=0.0f;
	KeyFrame1[1].Ytor=0.0f;
	KeyFrame1[1].Ztor=0.0f;

	KeyFrame1[2].Angt1=10.0f;
	KeyFrame1[2].Angt2=0.0f;
	KeyFrame1[2].Angc1=-10.0f;
	KeyFrame1[2].Angc2=0.0f;
	KeyFrame1[2].Angbi1=-50.0f;
	KeyFrame1[2].Angbi2=0.0f;
	KeyFrame1[2].Angbib=0.0f;
	KeyFrame1[2].Angbd1=60.0f;
	KeyFrame1[2].Angbd2=0.0f;
	KeyFrame1[2].Angbdb=0.0f;
	KeyFrame1[2].Angpizq=40.0f;
	KeyFrame1[2].Angpder=-40.0f;
	KeyFrame1[2].Angpi=30.0f;
	KeyFrame1[2].Angpd=20.0f;
	KeyFrame1[2].Xtor=0.0f;
	KeyFrame1[2].Ytor=0.0f;
	KeyFrame1[2].Ztor=0.0f;

	//Secuencia 2 (Hurricane kick)
	KeyFrame2[0].Angt1=0.0f;
	KeyFrame2[0].Angt2=0.0f;
	KeyFrame2[0].Angc1=-20.0f;
	KeyFrame2[0].Angc2=0.0f;
	KeyFrame2[0].Angbi1=90.0f;
	KeyFrame2[0].Angbi2=30.0f;
	KeyFrame2[0].Angbib=0.0f;
	KeyFrame2[0].Angbd1=-100.0f;
	KeyFrame2[0].Angbd2=-50.0f;
	KeyFrame2[0].Angbdb=0.0f;
	KeyFrame2[0].Angpizq=0.0f;
	KeyFrame2[0].Angpder=-90.0f;
	KeyFrame2[0].Angpi=0.0f;
	KeyFrame2[0].Angpd=0.0f;
	KeyFrame2[0].Xtor=0.0f;
	KeyFrame2[0].Ytor=8.0f;
	KeyFrame2[0].Ztor=0.0f;

	KeyFrame2[1].Angt1=0.0f;
	KeyFrame2[1].Angt2=90.0f;
	KeyFrame2[1].Angc1=0.0f;
	KeyFrame2[1].Angc2=0.0f;
	KeyFrame2[1].Angbi1=90.0f;
	KeyFrame2[1].Angbi2=30.0f;
	KeyFrame2[1].Angbib=0.0f;
	KeyFrame2[1].Angbd1=-100.0f;
	KeyFrame2[1].Angbd2=-50.0f;
	KeyFrame2[1].Angbdb=0.0f;
	KeyFrame2[1].Angpizq=0.0f;
	KeyFrame2[1].Angpder=-90.0f;
	KeyFrame2[1].Angpi=0.0f;
	KeyFrame2[1].Angpd=0.0f;
	KeyFrame2[1].Xtor=0.0f;
	KeyFrame2[1].Ytor=8.0f;
	KeyFrame2[1].Ztor=0.0f;

	KeyFrame2[2].Angt1=0.0f;
	KeyFrame2[2].Angt2=180.0f;
	KeyFrame2[2].Angc1=-20.0f;
	KeyFrame2[2].Angc2=0.0f;
	KeyFrame2[2].Angbi1=90.0f;
	KeyFrame2[2].Angbi2=30.0f;
	KeyFrame2[2].Angbib=0.0f;
	KeyFrame2[2].Angbd1=-100.0f;
	KeyFrame2[2].Angbd2=-50.0f;
	KeyFrame2[2].Angbdb=0.0f;
	KeyFrame2[2].Angpizq=0.0f;
	KeyFrame2[2].Angpder=-90.0f;
	KeyFrame2[2].Angpi=0.0f;
	KeyFrame2[2].Angpd=0.0f;
	KeyFrame2[2].Xtor=0.0f;
	KeyFrame2[2].Ytor=8.0f;
	KeyFrame2[2].Ztor=0.0f;

	KeyFrame2[3].Angt1=0.0f;
	KeyFrame2[3].Angt2=270.0f;
	KeyFrame2[3].Angc1=0.0f;
	KeyFrame2[3].Angc2=0.0f;
	KeyFrame2[3].Angbi1=0.0f;
	KeyFrame2[3].Angbi2=30.0f;
	KeyFrame2[3].Angbib=0.0f;
	KeyFrame2[3].Angbd1=-100.0f;
	KeyFrame2[3].Angbd2=-50.0f;
	KeyFrame2[3].Angbdb=0.0f;
	KeyFrame2[3].Angpizq=0.0f;
	KeyFrame2[3].Angpder=-90.0f;
	KeyFrame2[3].Angpi=0.0f;
	KeyFrame2[3].Angpd=0.0f;
	KeyFrame2[3].Xtor=0.0f;
	KeyFrame2[3].Ytor=8.0f;
	KeyFrame2[3].Ztor=0.0f;

	//Secuencia 3 (salto)
	KeyFrame3[0].Angt1=6.0f;
	KeyFrame3[0].Angt2=0.0f;
	KeyFrame3[0].Angc1=-6.0f;
	KeyFrame3[0].Angc2=0.0f;
	KeyFrame3[0].Angbi1=10.0f;
	KeyFrame3[0].Angbi2=0.0f;
	KeyFrame3[0].Angbib=0.0f;
	KeyFrame3[0].Angbd1=10.0f;
	KeyFrame3[0].Angbd2=0.0f;
	KeyFrame3[0].Angbdb=0.0f;
	KeyFrame3[0].Angpizq=-26.0f;
	KeyFrame3[0].Angpder=0.0f;
	KeyFrame3[0].Angpi=11.0f;
	KeyFrame3[0].Angpd=0.0f;
	KeyFrame3[0].Xtor=0.0f;
	KeyFrame3[0].Ytor=0.0f;
	KeyFrame3[0].Ztor=0.0f;

	KeyFrame3[1].Angt1=18.0f;
	KeyFrame3[1].Angt2=0.0f;
	KeyFrame3[1].Angc1=-18.0f;
	KeyFrame3[1].Angc2=0.0f;
	KeyFrame3[1].Angbi1=30.0f;
	KeyFrame3[1].Angbi2=5.0f;
	KeyFrame3[1].Angbib=0.0f;
	KeyFrame3[1].Angbd1=30.0f;
	KeyFrame3[1].Angbd2=-5.0f;
	KeyFrame3[1].Angbdb=0.0f;
	KeyFrame3[1].Angpizq=-75.0f;
	KeyFrame3[1].Angpder=-10.0f;
	KeyFrame3[1].Angpi=55.0f;
	KeyFrame3[1].Angpd=0.0f;
	KeyFrame3[1].Xtor=0.0f;
	KeyFrame3[1].Ytor=0.0f;
	KeyFrame3[1].Ztor=0.0f;

	KeyFrame3[2].Angt1=20.0f;
	KeyFrame3[2].Angt2=0.0f;
	KeyFrame3[2].Angc1=-20.0f;
	KeyFrame3[2].Angc2=0.0f;
	KeyFrame3[2].Angbi1=60.0f;
	KeyFrame3[2].Angbi2=15.0f;
	KeyFrame3[2].Angbib=0.0f;
	KeyFrame3[2].Angbd1=60.0f;
	KeyFrame3[2].Angbd2=-15.0f;
	KeyFrame3[2].Angbdb=5.0f;
	KeyFrame3[2].Angpizq=-105.0f;
	KeyFrame3[2].Angpder=-20.0f;
	KeyFrame3[2].Angpi=100.0f;
	KeyFrame3[2].Angpd=15.0f;
	KeyFrame3[2].Xtor=0.0f;
	KeyFrame3[2].Ytor=0.0f;
	KeyFrame3[2].Ztor=0.0f;

}

void inicializaPuntosdeControl()
{
	totalP=(20-3)*resolucion;
	pSpAct=0;

	puntosdeControl[0]=CVector(-40.0f, 6.0f, 50.0f);
	puntosdeControl[1]=CVector(-30.0f, 16.0f, 40.0f);
	puntosdeControl[2]=CVector(-30.0f, 23.0f, 30.0f);
	puntosdeControl[3]=CVector(-25.0f, 36.0f, 25.0f);
	puntosdeControl[4]=CVector(-22.0f, 44.0f, 21.0f);
	puntosdeControl[5]=CVector(-18.0f, 50.0f, 17.0f);
	puntosdeControl[6]=CVector(-12.0f, 17.0f, 15.0f);
	puntosdeControl[7]=CVector(-4.0f, 16.0f, 23.0f);
	puntosdeControl[8]=CVector(2.0f, 10.0f, 29.0f);
	puntosdeControl[9]=CVector(4.0f, 25.0f, 32.0f);
	puntosdeControl[10]=CVector(10.0f, 27.0f, 20.0f);
	puntosdeControl[11]=CVector(20.0f, 40.0f, 10.0f);
	puntosdeControl[12]=CVector(24.0f, 52.0f, 4.0f);
	puntosdeControl[13]=CVector(28.0f, 35.0f, -10.0f);
	puntosdeControl[14]=CVector(-35.0f, 40.0f, -19.0f);
	puntosdeControl[15]=CVector(-100.0f, 50.0f, -7.0f);
	puntosdeControl[16]=CVector(-120.0f, 50.0f, 10.0f);
	puntosdeControl[17]=CVector(-110.0f, 40.0f, 10.0f);
	puntosdeControl[18]=CVector(-90.0f, 30.0f, 50.0f);
	puntosdeControl[19]=CVector(-80.0f, 18.0f, 10.0f);
}

void iniciaCajasdeColision()
{
	cajaPersonaje.pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+3.0f, player1.PosicionObj.z);
	cajaPersonaje.tama�o=CVector(6.0f, 6.0f, 6.0f);
	cajaPersonaje.xMin=cajaPersonaje.pos.x-cajaPersonaje.tama�o.x*0.5f;
	cajaPersonaje.xMax=cajaPersonaje.pos.x+cajaPersonaje.tama�o.x*0.5f;
	cajaPersonaje.yMin=cajaPersonaje.pos.y-cajaPersonaje.tama�o.y*0.5f;
	cajaPersonaje.yMax=cajaPersonaje.pos.y+cajaPersonaje.tama�o.y*0.5f;
	cajaPersonaje.zMin=cajaPersonaje.pos.z-cajaPersonaje.tama�o.z*0.5f;
	cajaPersonaje.zMax=cajaPersonaje.pos.z+cajaPersonaje.tama�o.z*0.5f;

	cajaEscenario[0].pos=CVector(-75.0f, -30.0f, 10.0f);
	cajaEscenario[0].tama�o=CVector(30.0f, 60.0f, 20.0f);

	cajaEscenario[1].pos=CVector(-67.5f, 7.5f, -10.0f);
	cajaEscenario[1].tama�o=CVector(45.0f, 15.0f, 20.0f);

	cajaEscenario[2].pos=CVector(-67.5f, 22.5f, -20.0f);
	cajaEscenario[2].tama�o=CVector(45.0f, 15.0f, 20.0f);

	cajaEscenario[3].pos=CVector(-52.5f, -25.0f, 5.0f);
	cajaEscenario[3].tama�o=CVector(15.0f, 60.0f, 10.0f);

	cajaEscenario[4].pos=CVector(-22.5f, 17.5f, -5.0f);
	cajaEscenario[4].tama�o=CVector(45.0f, 25.0f, 10.0f);

	cajaEscenario[5].pos=CVector(-22.5f, 4.0f, 11.0f);
	cajaEscenario[5].tama�o=CVector(45.0f, 8.0f, 22.0f);

	cajaEscenario[6].pos=CVector(-22.5f, -6.0f, 12.5f);
	cajaEscenario[6].tama�o=CVector(45.0f, 20.0f, 25.0f);

	for(int i=0; i<7; i++)
	{
		cajaEscenario[i].xMin=cajaEscenario[i].pos.x-cajaEscenario[i].tama�o.x*0.5f;
		cajaEscenario[i].xMax=cajaEscenario[i].pos.x+cajaEscenario[i].tama�o.x*0.5f;
		cajaEscenario[i].yMin=cajaEscenario[i].pos.y-cajaEscenario[i].tama�o.y*0.5f;
		cajaEscenario[i].yMax=cajaEscenario[i].pos.y+cajaEscenario[i].tama�o.y*0.5f;
		cajaEscenario[i].zMin=cajaEscenario[i].pos.z-cajaEscenario[i].tama�o.z*0.5f;
		cajaEscenario[i].zMax=cajaEscenario[i].pos.z+cajaEscenario[i].tama�o.z*0.5f;
	}
}

void dibujaCajaColision(cajaCol *caja)
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 0.0f);
	glLineWidth(3.0f);

	glBegin(GL_LINE_LOOP);
		glVertex3f(caja->xMin, caja->yMin, caja->zMax);
		glVertex3f(caja->xMax, caja->yMin, caja->zMax);
		glVertex3f(caja->xMax, caja->yMax, caja->zMax);
		glVertex3f(caja->xMin, caja->yMax, caja->zMax);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(caja->xMin, caja->yMin, caja->zMin);
		glVertex3f(caja->xMax, caja->yMin, caja->zMin);
		glVertex3f(caja->xMax, caja->yMax, caja->zMin);
		glVertex3f(caja->xMin, caja->yMax, caja->zMin);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(caja->xMin, caja->yMax, caja->zMin);
		glVertex3f(caja->xMin, caja->yMax, caja->zMax);

		glVertex3f(caja->xMax, caja->yMax, caja->zMin);
		glVertex3f(caja->xMax, caja->yMax, caja->zMax);

		glVertex3f(caja->xMin, caja->yMin, caja->zMin);
		glVertex3f(caja->xMin, caja->yMin, caja->zMax);

		glVertex3f(caja->xMax, caja->yMin, caja->zMin);
		glVertex3f(caja->xMax, caja->yMin, caja->zMax);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glEnable(GL_LIGHTING);
}

void iniciaEsferasdeColision()
{
	esferaPersonaje.estado=0;
	esferaPersonaje.radio=3.0f;
	esferaPersonaje.pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+3.0f, player1.PosicionObj.z);

	esferaEnemigo.estado=0;
	esferaEnemigo.radio=5.0f;
	esferaEnemigo.pos=CVector(-70.0f, 5.0f, 15.0f);
}

//PART�CULAS -- cambio 3
//Se definen los valores iniciales para las propiedades de cada part�cula
//Por ejemplo, si se desea que todas las part�culas surjan de un punto la posici�n inicial debe ser 0
//en cada eje para todas las part�culas.
//Si se desea que el punto de origen est� dentro de un �rea mas amplia que un punto, como una zona esf�rica
//la posici�n inicial de cada particula se puede calcular de manera pseudoaleatoria limitando a un rango para
//cada eje como se hace en la asignaci�n inicial de velocidad en esta funci�n

//fade es la cantidad que se le restar� al valor alpha del color de la part�cula en cada ciclo de dibujo y
//determina el tiempo de vida de la part�cula. Si se quiere que todas las particulas duren lo mismo se le da un
//valor constante y todas las part�culas desapareceran antes de "revivir". Si se da un valor aleatorio algunas 
//particulas moriran mientras otras continuan vivas, de esta forma el efecto se desvanece poco a poco mientras 
//se sigue generando en la posici�n inicial en vez de terminar de manera abrupta y reiniciarse.

//Tambi�n se puede definir la gravedad para cada part�cula. Dependiendo que tan grande sea el valor que se le da
//se puede hacer que esa sea la direcci�n de movimiento o que sea la definida por la velocidad en cada eje pero
//desviandose un poco al final hacia el eje con mas gravedad (se hace una curva)

void InicializaParticulas()
{
	//Se inicializan los valores de la estructura de part�culas
	for (int loop=0; loop<MAX_PARTICULAS; loop++)				
	{
  		particle[loop].life=1.0f;
  		particle[loop].fade=float(rand()%100)/1000.0f+0.005f;

  		//Posicion inicial
  		particle[loop].x = 0;
  		particle[loop].y = 0;
  		particle[loop].z = 0;

  		//"Gravedad" inicial
  		particle[loop].xg = 0;
  		particle[loop].yg = 2.0f;
  		particle[loop].zg = 0;

      	particle[loop].xi = float((rand()%10)-5.0f);  // Direccion y velocidad en X
  		particle[loop].yi = float((rand()%10)-5.0f);  // Direccion y velocidad en Y
  		particle[loop].zi = float((rand()%10)-5.0f);  // Direccion y velocidad en Z
	}
}

int IniGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Fondo negro
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de c�lculos de perspectiva

	glCullFace(GL_BACK);								// Configurado para eliminar caras traseras
	glEnable(GL_CULL_FACE);								// Activa eliminacion de caras ocultas

	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	
	glEnable(GL_LIGHT0);								// Activa luz0

	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT1, GL_AMBIENT,  LightAmb2);		// Componente ambiente
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightDif2);		// Componente difusa
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc2);		// Componente especular

	glEnable(GL_LIGHTING);								// Habilita la iluminaci�n

	e=gluNewQuadric();

	CargaModelos();
	IniVarsPlayer();
	InicializaParametrosdeControl();
	InicializaCamara();
	
	CargaTexturas();
	InicializaAnim();
	DatosAnimacion();

	//Cambios para Fuentes
	Font.BuildFont();
	controlFunc.inicializaControl();

	infGame.estadoJuego=6; //

	posCamV=CVector(0.0f, 30.0f, 0.0f);
	dirCamV=CVector(0.0f, 0.0f, 1.0f);

	infGame.opcionMenuSelec=0;

	infGame.opcionMenuPausa=0;
	infGame.pausa=0;
	
	infGame.opMenuPrinc.dificultad=1;
	infGame.opMenuPrinc.numVidas=5;
	infGame.opMenuPrinc.sonido=0;

	cargaConfiguracion();

	sonidoF[0].cargaSonido("Audio/bgm.mp3", 1);
	sonidoF[1].cargaSonido("Audio/sonido.mp3", 0);
	sonidoF[2].cargaSonido("Audio/intro.mp3", 0);

	inicializaPuntosdeControl();
	iniciaCajasdeColision();
	iniciaEsferasdeColision();
	/*
	datosModelo.numObjetos=1;
	datosModelo.obj=new objeto[datosModelo.numObjetos];

	datosModelo.obj[0].numPlanos=2;
	datosModelo.obj[0].pl=new plano[datosModelo.obj[0].numPlanos];

	datosModelo.obj[0].pl[0].normal=CVector(0.0f, 0.0f, 1.0f);
	datosModelo.obj[0].pl[0].vert[0].pos=CVector(-60.0f,  5.0f, 5.0f);
	datosModelo.obj[0].pl[0].vert[1].pos=CVector(-40.0f,  5.0f, 5.0f);
	datosModelo.obj[0].pl[0].vert[2].pos=CVector(-40.0f, 15.0f, 5.0f);
	datosModelo.obj[0].pl[0].vert[3].pos=CVector(-60.0f, 15.0f, 5.0f);

	datosModelo.obj[0].pl[1].normal=CVector(-1.0f, 0.0f, 0.0f);
	datosModelo.obj[0].pl[1].vert[0].pos=CVector(-40.0f,  5.0f,  5.0f);
	datosModelo.obj[0].pl[1].vert[1].pos=CVector(-40.0f,  5.0f, 15.0f);
	datosModelo.obj[0].pl[1].vert[2].pos=CVector(-40.0f, 15.0f, 15.0f);
	datosModelo.obj[0].pl[1].vert[3].pos=CVector(-40.0f, 15.0f,  5.0f);

	guardaModelo();
	*/
	cargaModelo();

	timerEstados.iniciaTimer();
	InicializaParticulas();

	return TRUE;										
}

void colisionEsferas()
{
	int numEnemigos=1;

	//for(int i=0; i<numEnemigos; i++)
	//{
	CVector vecCentros=esferaEnemigo.pos-esferaPersonaje.pos;
	float dist=Magnitud(vecCentros);

	if(dist < esferaEnemigo.radio+esferaPersonaje.radio)
	{
		esferaPersonaje.estado=1;
		if(esferaEnemigo.estado == 0)
		{
			esferaEnemigo.estado=1;
			sonidoF[1].reproduceSonido();
		}
	}
	//}
}

void dibujaEsferadeColision(esferaCol *esfera)
{
	CVector vert[2];
	CVector centro;

	float ang, deltaAng;

	deltaAng=360.0f/16;

	glDisable(GL_LIGHTING);
	if(esfera->estado == 0)
		glColor3f(0.0f, 1.0f, 0.0f);
	else if(esfera->estado == 1)
		glColor3f(1.0f, 1.0f, 1.0f);

	glLineWidth(3.0f);

	glBegin(GL_LINE_STRIP);

	for(int i=0; i<16; i++)
	{
		ang=i*deltaAng;
		vert[0].x=esfera->pos.x+esfera->radio*cos(ang*PI/180.0f);
		vert[0].y=esfera->pos.y+esfera->radio*sin(ang*PI/180.0f);
		vert[0].z=esfera->pos.z;

		glVertex3f(vert[0].x, vert[0].y, vert[0].z);

		ang=(i+1)*deltaAng;
		vert[1].x=esfera->pos.x+esfera->radio*cos(ang*PI/180.0f);
		vert[1].y=esfera->pos.y+esfera->radio*sin(ang*PI/180.0f);
		vert[1].z=esfera->pos.z;

		centro=esfera->pos;
		
		
	}

	glEnd();

	glEnable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
}

void actualizaEsferasdeColision()
{
	esferaPersonaje.pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+3.0f, player1.PosicionObj.z);
}

void obtieneAltPiso()
{
	float izq, der, ade, atr;

	for(int i=0; i<numCajas; i++)
	{
		izq=cajaPersonaje.xMin;
		der=cajaPersonaje.xMax;
		ade=cajaPersonaje.zMax;
		atr=cajaPersonaje.zMin;
		
		if(cajaPersonaje.yMin >= cajaEscenario[i].yMax
			&& ((izq > cajaEscenario[i].xMin && izq < cajaEscenario[i].xMax)
			|| (der > cajaEscenario[i].xMin && der < cajaEscenario[i].xMax))
			&& ((ade > cajaEscenario[i].zMin && ade < cajaEscenario[i].zMax)
			|| (atr > cajaEscenario[i].zMin && atr < cajaEscenario[i].zMax)))
		{
			//Comprueba que sea la m�s cercana en y (de otra forma podr�a tomar la altura de la caja m�s baja si se encuentra una caja debajo de otra)
			if(fabs(cajaPersonaje.pos.y-cajaEscenario[i].pos.y) <= cajaPersonaje.tama�o.y*0.5f+cajaEscenario[i].tama�o.y*0.5f+player1.VelocidadObj)
			{
				altPiso=cajaEscenario[i].yMax;
				break;
			}
		}
		else
			altPiso=altMin;
	}
}

bool colisionCajas(int dir)
{
	bool col=false;
	CVector pSig;
	CVector A, B, C, D, E, F, G, H;

	for(int i=0; i<numCajas; i++)
	{
		if(dir == 1) //x+
		{
			pSig.x=player1.PosicionObj.x+player1.VelocidadObj;
			pSig.y=player1.PosicionObj.y;
			pSig.z=player1.PosicionObj.z;
		}
		else if(dir == 2) //x-
		{
			pSig.x=player1.PosicionObj.x-player1.VelocidadObj;
			pSig.y=player1.PosicionObj.y;
			pSig.z=player1.PosicionObj.z;
		}
		else if(dir == 3) //z+
		{
			pSig.x=player1.PosicionObj.x;
			pSig.y=player1.PosicionObj.y;
			pSig.z=player1.PosicionObj.z+player1.VelocidadObj;
		}
		else if(dir == 4) //z-
		{
			pSig.x=player1.PosicionObj.x;
			pSig.y=player1.PosicionObj.y;
			pSig.z=player1.PosicionObj.z-player1.VelocidadObj;
		}
		else if(dir == 5) //y+
		{
			pSig.x=player1.PosicionObj.x;
			pSig.y=player1.PosicionObj.y+player1.VelocidadObj;
			pSig.z=player1.PosicionObj.z;
		}
		else if(dir == 6) //y-
		{
			pSig.x=player1.PosicionObj.x;
			pSig.y=player1.PosicionObj.y-player1.VelocidadObj;
			pSig.z=player1.PosicionObj.z;
		}

		cajaPersonaje.pos=CVector(pSig.x, pSig.y+3.0f, pSig.z);
		cajaPersonaje.xMin=cajaPersonaje.pos.x-cajaPersonaje.tama�o.x*0.5f;
		cajaPersonaje.xMax=cajaPersonaje.pos.x+cajaPersonaje.tama�o.x*0.5f;
		cajaPersonaje.yMin=cajaPersonaje.pos.y-cajaPersonaje.tama�o.y*0.5f;
		cajaPersonaje.yMax=cajaPersonaje.pos.y+cajaPersonaje.tama�o.y*0.5f;
		cajaPersonaje.zMin=cajaPersonaje.pos.z-cajaPersonaje.tama�o.z*0.5f;
		cajaPersonaje.zMax=cajaPersonaje.pos.z+cajaPersonaje.tama�o.z*0.5f;

		//Inf-izq-front
		A.x=cajaPersonaje.xMin;
		A.y=cajaPersonaje.yMin;
		A.z=cajaPersonaje.zMax;

		//Inf-der-front
		B.x=cajaPersonaje.xMax;
		B.y=cajaPersonaje.yMin;
		B.z=cajaPersonaje.zMax;

		//Sup-der-front
		C.x=cajaPersonaje.xMax;
		C.y=cajaPersonaje.yMax;
		C.z=cajaPersonaje.zMax;

		//Sup-izq-front
		D.x=cajaPersonaje.xMin;
		D.y=cajaPersonaje.yMax;
		D.z=cajaPersonaje.zMax;

		//Inf-izq-tras
		E.x=cajaPersonaje.xMin;
		E.y=cajaPersonaje.yMin;
		E.z=cajaPersonaje.zMin;

		//Inf-der-tras
		F.x=cajaPersonaje.xMax;
		F.y=cajaPersonaje.yMin;
		F.z=cajaPersonaje.zMin;

		//Sup-der-tras
		G.x=cajaPersonaje.xMax;
		G.y=cajaPersonaje.yMax;
		G.z=cajaPersonaje.zMin;

		//Sup-izq-tras
		H.x=cajaPersonaje.xMin;
		H.y=cajaPersonaje.yMax;
		H.z=cajaPersonaje.zMin;

		if((A.x > cajaEscenario[i].xMin && A.x < cajaEscenario[i].xMax && A.y > cajaEscenario[i].yMin && A.y < cajaEscenario[i].yMax && A.z > cajaEscenario[i].zMin && A.z < cajaEscenario[i].zMax)
			|| (B.x > cajaEscenario[i].xMin && B.x < cajaEscenario[i].xMax && B.y > cajaEscenario[i].yMin && B.y < cajaEscenario[i].yMax && B.z > cajaEscenario[i].zMin && B.z < cajaEscenario[i].zMax)
			|| (C.x > cajaEscenario[i].xMin && C.x < cajaEscenario[i].xMax && C.y > cajaEscenario[i].yMin && C.y < cajaEscenario[i].yMax && C.z > cajaEscenario[i].zMin && C.z < cajaEscenario[i].zMax)
			|| (D.x > cajaEscenario[i].xMin && D.x < cajaEscenario[i].xMax && D.y > cajaEscenario[i].yMin && D.y < cajaEscenario[i].yMax && D.z > cajaEscenario[i].zMin && D.z < cajaEscenario[i].zMax)
			|| (E.x > cajaEscenario[i].xMin && E.x < cajaEscenario[i].xMax && E.y > cajaEscenario[i].yMin && E.y < cajaEscenario[i].yMax && E.z > cajaEscenario[i].zMin && E.z < cajaEscenario[i].zMax)
			|| (F.x > cajaEscenario[i].xMin && F.x < cajaEscenario[i].xMax && F.y > cajaEscenario[i].yMin && F.y < cajaEscenario[i].yMax && F.z > cajaEscenario[i].zMin && F.z < cajaEscenario[i].zMax)
			|| (G.x > cajaEscenario[i].xMin && G.x < cajaEscenario[i].xMax && G.y > cajaEscenario[i].yMin && G.y < cajaEscenario[i].yMax && G.z > cajaEscenario[i].zMin && G.z < cajaEscenario[i].zMax)
			|| (H.x > cajaEscenario[i].xMin && H.x < cajaEscenario[i].xMax && H.y > cajaEscenario[i].yMin && H.y < cajaEscenario[i].yMax && H.z > cajaEscenario[i].zMin && H.z < cajaEscenario[i].zMax))
		{
			col=true;
			
			if(dir == 1) //x+
				player1.PosicionObj.x=cajaEscenario[i].xMin-cajaPersonaje.tama�o.x*0.5f;
			else if(dir == 2) //x-
				player1.PosicionObj.x=cajaEscenario[i].xMax+cajaPersonaje.tama�o.x*0.5f;
			else if(dir == 3) //z+
				player1.PosicionObj.z=cajaEscenario[i].zMin-cajaPersonaje.tama�o.z*0.5f;
			else if(dir == 4) //z-
				player1.PosicionObj.z=cajaEscenario[i].zMax+cajaPersonaje.tama�o.z*0.5f;
			else if(dir == 5) //y+
				player1.PosicionObj.y=cajaEscenario[i].yMin-cajaPersonaje.tama�o.y*0.5f-3.0f;
			else if(dir == 6) //y-
				player1.PosicionObj.y=cajaEscenario[i].yMax;
		
			break;
		}
	}

	return col;
}

void actualizaMovPersonaje()
{
	static int tiempoColPersonaje=0;
	static int tiempoColEnemigo=0;

	actualizaCajaColPersonaje();
	
	if(esferaPersonaje.estado == 1)
	{
		if(tiempoColPersonaje < 20)
			tiempoColPersonaje++;
		else
		{
			tiempoColPersonaje=0;
			esferaPersonaje.estado=0;
		}
	}

	if(esferaEnemigo.estado == 1)
	{
		if(tiempoColEnemigo < 20)
			tiempoColEnemigo++;
		else
		{
			tiempoColEnemigo=0;
			esferaEnemigo.estado=0;
		}
	}

	if(player1.saltando == false)
		obtieneAltPiso();

	if(player1.caminando == true)
	{
		if(player1.dirX == 1)
		{
			if(!colisionCajas(1))
				player1.PosicionObj.x+=player1.VelocidadObj;
			else
				player1.caminando=false;
		}
		else if(player1.dirX == 2)
		{
			if(!colisionCajas(2))
				player1.PosicionObj.x-=player1.VelocidadObj;
			else
				player1.caminando=false;
		}

		if(player1.dirZ == 1)
		{
			if(!colisionCajas(3))
				player1.PosicionObj.z+=player1.VelocidadObj;
			else
				player1.caminando=false;
		}
		else if(player1.dirZ == 2)
		{
			if(!colisionCajas(4))
				player1.PosicionObj.z-=player1.VelocidadObj;
			else
				player1.caminando=false;
		}
	}

	if(player1.saltando == true)
	{
		if(player1.PosicionObj.y < altPiso+10.0f)
		{
			if(!colisionCajas(5))
				player1.PosicionObj.y+=player1.VelocidadObj;
			else
			{
				player1.saltando=false;
				player1.cayendo=true;
			}
		}
		else
		{
			player1.saltando=false;
			player1.cayendo=true;
		}
	}
	
	//comentar para que el personaje no se caiga
	/*if(player1.cayendo == true)
	{
		if(player1.PosicionObj.y > altMin)
		{
			if(!colisionCajas(6))
				player1.PosicionObj.y-=player1.VelocidadObj;
		}
		else
		{
			player1.PosicionObj.y=altMin;
			altPiso=altMin;
		}
	}*/

	actualizaEsferasdeColision();
	colisionEsferas();
}

void controlEstados()
{
	if(infGame.estadoJuego == 1) //Logo estudio
	{
		if(timerEstados.TiempoActual() > 3000)
		{
			infGame.tiempoEstado=0;
			infGame.estadoJuego=2;
		}
	}
	else if(infGame.estadoJuego == 2) //Pantalla de carga
	{
		if(timerEstados.TiempoActual() > 6000)
			infGame.cargaArchivosCompleta=1;
			
		if(infGame.cargaArchivosCompleta == 1)
		{
			infGame.tiempoEstado=0;
			infGame.estadoJuego=3;
			sonidoF[2].volumenSonido(0.5f);
			sonidoF[2].pausaSonido(false);
		}
	}
	else if(infGame.estadoJuego == 3) //introduccion del juego
	{
		if(timerEstados.TiempoActual() > 28000)
		{
			infGame.tiempoEstado=0;
			infGame.estadoJuego=4;
		}
	}
	else if(infGame.estadoJuego == 4) //T�tulo del juego
	{
		if(timerEstados.TiempoActual() > 31000)
		{
			infGame.tiempoEstado=0;
			infGame.estadoJuego=5;
			timerEstados.detieneTimer();
		}
	}
}

void animacion(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play)
	{		
		if((abs(KeyFrame[playIndex+1].Angt1-Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angt2-Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc1-Angc1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc2-Angc2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi1-Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi2-Angbi2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbib-Angbib))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd1-Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd2-Angbd2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbdb-Angbdb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizq-Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpder-Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpi-Angpi))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpd-Angpd))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Xtor-Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ytor-Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ztor-Ztor))<0.1)
		{			
			playIndex++;			
			if(playIndex>maxKF-2)
			{
				playIndex=0;
				play=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex].incAngt1   = (KeyFrame[playIndex+1].Angt1-KeyFrame[playIndex].Angt1)/frames;
			KeyFrame[playIndex].incAngt2   = (KeyFrame[playIndex+1].Angt2-KeyFrame[playIndex].Angt2)/frames;
			KeyFrame[playIndex].incAngc1   = (KeyFrame[playIndex+1].Angc1-KeyFrame[playIndex].Angc1)/frames;
			KeyFrame[playIndex].incAngc2   = (KeyFrame[playIndex+1].Angc2-KeyFrame[playIndex].Angc2)/frames;
			KeyFrame[playIndex].incAngbi1  = (KeyFrame[playIndex+1].Angbi1-KeyFrame[playIndex].Angbi1)/frames;
			KeyFrame[playIndex].incAngbi2  = (KeyFrame[playIndex+1].Angbi2-KeyFrame[playIndex].Angbi2)/frames;
			KeyFrame[playIndex].incAngbib  = (KeyFrame[playIndex+1].Angbib-KeyFrame[playIndex].Angbib)/frames;
			KeyFrame[playIndex].incAngbd1  = (KeyFrame[playIndex+1].Angbd1-KeyFrame[playIndex].Angbd1)/frames;
			KeyFrame[playIndex].incAngbd2  = (KeyFrame[playIndex+1].Angbd2-KeyFrame[playIndex].Angbd2)/frames;
			KeyFrame[playIndex].incAngbdb  = (KeyFrame[playIndex+1].Angbdb-KeyFrame[playIndex].Angbdb)/frames;
			KeyFrame[playIndex].incAngpizq = (KeyFrame[playIndex+1].Angpizq-KeyFrame[playIndex].Angpizq)/frames;
			KeyFrame[playIndex].incAngpder = (KeyFrame[playIndex+1].Angpder-KeyFrame[playIndex].Angpder)/frames;
			KeyFrame[playIndex].incAngpi   = (KeyFrame[playIndex+1].Angpi-KeyFrame[playIndex].Angpi)/frames;
			KeyFrame[playIndex].incAngpd   = (KeyFrame[playIndex+1].Angpd-KeyFrame[playIndex].Angpd)/frames;
			KeyFrame[playIndex].incXtor    = (KeyFrame[playIndex+1].Xtor-KeyFrame[playIndex].Xtor)/frames;
			KeyFrame[playIndex].incYtor    = (KeyFrame[playIndex+1].Ytor-KeyFrame[playIndex].Ytor)/frames;
			KeyFrame[playIndex].incZtor    = (KeyFrame[playIndex+1].Ztor-KeyFrame[playIndex].Ztor)/frames;
			
			Angt1   += KeyFrame[playIndex].incAngt1;
			Angt2   += KeyFrame[playIndex].incAngt2;
			Angc1   += KeyFrame[playIndex].incAngc1;
			Angc2   += KeyFrame[playIndex].incAngc2;
			Angbi1  += KeyFrame[playIndex].incAngbi1;
			Angbi2  += KeyFrame[playIndex].incAngbi2;
			Angbib  += KeyFrame[playIndex].incAngbib;
			Angbd1  += KeyFrame[playIndex].incAngbd1;
			Angbd2  += KeyFrame[playIndex].incAngbd2;
			Angbdb  += KeyFrame[playIndex].incAngbdb;
			Angpizq += KeyFrame[playIndex].incAngpizq;
			Angpder += KeyFrame[playIndex].incAngpder;
			Angpi   += KeyFrame[playIndex].incAngpi;
			Angpd   += KeyFrame[playIndex].incAngpd;
			Xtor    += KeyFrame[playIndex].incXtor;
			Ytor    += KeyFrame[playIndex].incYtor;
			Ztor    += KeyFrame[playIndex].incZtor;
			
		}
	}
}

void ActualizaCamara()
{
	float objAlt=camara1.ObjetivoCam.y;
	float aux=(player1.PosicionObj.x-camara1.VariacionDespDist)*camara1.FactorCam+camara1.VariacionDespCam;

	//camara1.PosicionCam.x=150.0f*aux;
	camara1.PosicionCam.x=player1.PosicionObj.x-30.0f*aux;
	camara1.PosicionCam.y=12.0f;
	camara1.PosicionCam.z=40.0f;

	camara1.ObjetivoCam.x=player1.PosicionObj.x;
	camara1.ObjetivoCam.z=player1.PosicionObj.z;
	camara1.ObjetivoCam.y=objAlt;
}

void ControlPersonaje(int funcion)
{
	if(funcion == 1) //Giro a la derecha
	{
		player1.AngDir+=1.0f;
		if(player1.AngDir > 360.0f)
			player1.AngDir-=360.0f;

		player1.AngObj-=1.0f;
		if(player1.AngObj < 0.0f)
			player1.AngObj+=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);
	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngDir-=1.0f;
		if(player1.AngDir < 0.0f)
			player1.AngDir+=360.0f;

		player1.AngDirCam-=1.0f;
		if(player1.AngDirCam < 0.0f)
			player1.AngDirCam+=360.0f;
		
		player1.AngObj+=1.0f;
		if(player1.AngObj > 360.0f)
			player1.AngObj-=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);
	
	}
	else if(funcion == 3) //Avanza hacia adelante
	{
		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.DireccionCam*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosicionObjAnt=player1.PosicionObj;

		ActualizaCamara();

	}
	else if(funcion == 4) //Avanza hacia atr�s
	{
		player1.PosicionObj=player1.PosicionObj-player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.DireccionCam*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosicionObjAnt=player1.PosicionObj;

		ActualizaCamara();

	}
	else if(funcion == 5) //Sube objetivo de la c�mara
	{
		camara1.ObjetivoCam.y+=1.0f;
		player1.CamaraObjAlt+=1.0f;
	}
	else if(funcion == 6) //Baja objetivo de la c�mara
	{
		camara1.ObjetivoCam.y-=1.0f;
		player1.CamaraObjAlt-=1.0f;
	}
	
}

void actualizaCajaColPersonaje()
{
	cajaPersonaje.pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+3.0f, player1.PosicionObj.z);
	cajaPersonaje.xMin=cajaPersonaje.pos.x-cajaPersonaje.tama�o.x*0.5f;
	cajaPersonaje.xMax=cajaPersonaje.pos.x+cajaPersonaje.tama�o.x*0.5f;
	cajaPersonaje.yMin=cajaPersonaje.pos.y-cajaPersonaje.tama�o.y*0.5f;
	cajaPersonaje.yMax=cajaPersonaje.pos.y+cajaPersonaje.tama�o.y*0.5f;
	cajaPersonaje.zMin=cajaPersonaje.pos.z-cajaPersonaje.tama�o.z*0.5f;
	cajaPersonaje.zMax=cajaPersonaje.pos.z+cajaPersonaje.tama�o.z*0.5f;
}

void ControlPersonaje2(int funcion)
{
	if(funcion == 1) //Avanza a la derecha (x+)
	{
		player1.PosicionObj.x+=player1.VelocidadObj;
		player1.AngObj=90.0f;
	}
	else if(funcion == 2) //Avanza a la izquierda (x-)
	{
		player1.PosicionObj.x-=player1.VelocidadObj;
		player1.AngObj=-90.0f;
	}
	else if(funcion == 3) //Avanza hacia adentro (z-)
	{
		player1.PosicionObj.z-=player1.VelocidadObj;
		player1.AngObj=180.0f;
	}
	else if(funcion == 4) //Avanza hacia afuera (z+)
	{
		player1.PosicionObj.z+=player1.VelocidadObj;
		player1.AngObj=0.0f;
	}

	actualizaCajaColPersonaje();
}

void ControlPersonaje2D(int funcion)
{
	if(funcion == 1) //Avanza a la derecha
	{
		player1.AngObj=90.0f;

		player1.Direccion=CVector(1.0f,0.0f,0.0f);

		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.DireccionCam*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosicionObjAnt=player1.PosicionObj;

	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngObj=270.0f;

		player1.Direccion=CVector(-1.0f,0.0f,0.0f);

		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.DireccionCam*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosicionObjAnt=player1.PosicionObj;
	}
	
}

void DibujaEjes()
{
	glBegin(GL_LINES);
		//Eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glPointSize(10.0f);

	glBegin(GL_POINTS);
		//"Flecha" eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//"Flecha" eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//"Flecha" eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glPointSize(1.0f);

	glColor3f(1.0f,1.0f,1.0f);
}

void DibujaCaja(int tipo)
{
	glEnable(GL_TEXTURE_2D);

	//cara frontal
	if(tipo == 1)
		glBindTexture(GL_TEXTURE_2D, textura[17].texID);
	else if(tipo == 2)
		glBindTexture(GL_TEXTURE_2D, textura[18].texID);
	else if(tipo == 3)
		glBindTexture(GL_TEXTURE_2D, textura[19].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, -2.0f, 3.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f, -2.0f, 3.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f,  2.0f, 3.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,  2.0f, 3.5f);
	glEnd();

	//cara izq
	glBindTexture(GL_TEXTURE_2D, textura[20].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, -2.0f, -3.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, -2.0f,  3.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5f,  2.0f,  3.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,  2.0f, -3.5f);
	glEnd();

	//cara der
	glBindTexture(GL_TEXTURE_2D, textura[20].texID);

	glBegin(GL_QUADS);
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, -2.0f,  3.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, -2.0f, -3.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f,  2.0f, -3.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f,  2.0f,  3.5f);
	glEnd();

	//cara sup
	glBindTexture(GL_TEXTURE_2D, textura[21].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 2.0f, -3.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, 2.0f,  3.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.0f,  3.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 2.5f, 2.0f, -3.5f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void DibujaTransportadora()
{
	CVector V1, V2, N;
	static float animTs=0.0f;

	glEnable(GL_TEXTURE_2D);

	//Banda transportadora cara superior
	glBindTexture(GL_TEXTURE_2D, textura[24].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f-animTs, 0.0f); glVertex3f(-8.0f, 1.5f,  3.0f);
		glTexCoord2f(4.0f-animTs, 0.0f); glVertex3f( 8.0f, 1.5f,  3.0f);
		glTexCoord2f(4.0f-animTs, 1.0f); glVertex3f( 8.0f, 1.5f, -3.0f);
		glTexCoord2f(0.0f-animTs, 1.0f); glVertex3f(-8.0f, 1.5f, -3.0f);
	glEnd();

	//Banda transportadora cara izquierda inclinada
	
	V1=CVector(-8.0f, 1.5f,  3.0f)-CVector(-9.0f, 1.3f,  3.0f);
	V2=CVector(-9.0f, 1.3f, -3.0f)-CVector(-9.0f, 1.3f,  3.0f);
	N=Normaliza(Cruz(V1, V2));

	glBegin(GL_QUADS);
		glNormal3f(N.x,N.y,N.z);
		glTexCoord2f(0.6f-animTs, 0.0f); glVertex3f(-9.0f, 1.0f,  3.0f);
		glTexCoord2f(1.0f-animTs, 0.0f); glVertex3f(-8.0f, 1.5f,  3.0f);
		glTexCoord2f(1.0f-animTs, 1.0f); glVertex3f(-8.0f, 1.5f, -3.0f);
		glTexCoord2f(0.6f-animTs, 1.0f); glVertex3f(-9.0f, 1.0f, -3.0f);
	glEnd();
	
	//Banda transportadora cara izquierda vertical
	
	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f-animTs, 0.0f); glVertex3f(-9.0f, -0.5f,  3.0f);
		glTexCoord2f(0.6f-animTs, 0.0f); glVertex3f(-9.0f,  1.0f,  3.0f);
		glTexCoord2f(0.6f-animTs, 1.0f); glVertex3f(-9.0f,  1.0f, -3.0f);
		glTexCoord2f(0.0f-animTs, 1.0f); glVertex3f(-9.0f, -0.5f, -3.0f);
	glEnd();

	//Banda transportadora cara derecha inclinada
	
	V1=CVector(9.0f, 1.0f,  3.0f)-CVector(8.0f, 1.5f,  3.0f);
	V2=CVector(8.0f, 1.5f, -3.0f)-CVector(8.0f, 1.5f,  3.0f);
	N=Normaliza(Cruz(V1, V2));

	glBegin(GL_QUADS);
		glNormal3f(N.x,N.y,N.z);
		glTexCoord2f(0.0f-animTs, 0.0f); glVertex3f(8.0f, 1.5f,  3.0f);
		glTexCoord2f(0.4f-animTs, 0.0f); glVertex3f(9.0f, 1.0f,  3.0f);
		glTexCoord2f(0.4f-animTs, 1.0f); glVertex3f(9.0f, 1.0f, -3.0f);
		glTexCoord2f(0.0f-animTs, 1.0f); glVertex3f(8.0f, 1.5f, -3.0f);
	glEnd();

	//Banda transportadora cara frontal 1
	glBindTexture(GL_TEXTURE_2D, textura[23].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f, -1.5f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.0f, -1.5f, 3.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.0f,  1.5f, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0f,  1.5f, 3.0f);
	glEnd();

	//Banda transportadora cara frontal 2
	glBindTexture(GL_TEXTURE_2D, textura[22].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.0f, -1.5f, 3.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.0f, -1.5f, 3.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.0f,  1.5f, 3.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.0f,  1.5f, 3.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);

	glColor3f(0.0f,0.0f,0.0f);

	glBegin(GL_POLYGON);
		glVertex3f(-7.0f, -1.5f, 3.0f);
		glVertex3f(-7.0f,  1.5f, 3.0f);
		glVertex3f(-8.0f,  1.5f, 3.0f);
		glVertex3f(-9.0f,  1.0f, 3.0f);
		glVertex3f(-9.0f, -0.5f, 3.0f);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(7.0f, -1.5f, 3.0f);
		glVertex3f(9.0f, -0.5f, 3.0f);
		glVertex3f(9.0f,  1.0f, 3.0f);
		glVertex3f(8.0f,  1.5f, 3.0f);
		glVertex3f(7.0f,  1.5f, 3.0f);
	glEnd();
/*
	glBegin(GL_QUADS);
		glVertex3f(-7.0f, -1.5f, 3.0f);
		glVertex3f(-3.0f, -1.5f, 3.0f);
		glVertex3f(-3.0f,  1.5f, 3.0f);
		glVertex3f(-7.0f,  1.5f, 3.0f);
	glEnd();
*/
	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);

	if(animTs < 1.0f)
		animTs+=0.01f;
	else
		animTs=0.0f;
}

void dibujaCamara()
{
	glPushMatrix();
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		gluCylinder(e, 1.0f, 1.0f, 5.0f, 12, 1);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 5.0f);
			gluDisk(e, 0.0f, 1.0f, 12, 1);
		glPopMatrix();
	glPopMatrix();
}

void ejemploDeteccionCamara()
{
	glPushMatrix();
		glTranslatef(posCamV.x, posCamV.y, posCamV.z);
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
		dibujaCamara();
	glPopMatrix();

	vecCamVPl=Normaliza(player1.PosicionObj-posCamV);

	float p=Punto(vecCamVPl, dirCamV);

	if(p > 0.5f)
		detectado=1;
	else
		detectado=0;
	
}

void DibujaEscenario(){

	glEnable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, textura);

	//Base del escenario
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glNormal3f(0.0f, 0.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(0.0f,0.0f, 20.0f);
	//glTexCoord2f(8.0f, 0.0f); 
	glVertex3f(100.0f, 0.0f, 20.0f);
	//glTexCoord2f(8.0f, 5.0f); 
	glVertex3f(100.0f, 10.0f, 20.0f);
	//glTexCoord2f(0.0f, 5.0f); 
	glVertex3f(0.0f, 10.0f, 20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glNormal3f(0.0f, 0.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(0.0f, 10.0f, 0.0f);
	//glTexCoord2f(8.0f, 0.0f); 
	glVertex3f(0.0f, 10.0f, 20.0f);
	//glTexCoord2f(8.0f, 5.0f); 
	glVertex3f(100.0f, 10.0f, 20.0f);
	//glTexCoord2f(0.0f, 5.0f); 
	glVertex3f(100.0f, 10.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void DibujaEscenarioDelProfe()
{
	//-- Aqu� comienzan los planos con textura
	glEnable(GL_TEXTURE_2D);

	//Plano 1 (pared 1)
	glBindTexture(GL_TEXTURE_2D, textura[2].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -20.0f);
		glTexCoord2f(8.0f, 0.0f); glVertex3f( 80.0f, 20.0f, -20.0f);
		glTexCoord2f(8.0f, 5.0f); glVertex3f( 80.0f, 80.0f, -20.0f);
		glTexCoord2f(0.0f, 5.0f); glVertex3f(-90.0f, 80.0f, -20.0f);
	glEnd();

	//Plano 2 (piso 1)
	glBindTexture(GL_TEXTURE_2D, textura[4].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.1f, 0.0f); glVertex3f(-90.0f, 0.0f, 20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-60.0f, 0.0f, 20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f, 0.0f,  0.0f);
		glTexCoord2f(0.1f, 1.0f); glVertex3f(-90.0f, 0.0f,  0.0f);
	glEnd();

	//Plano 3 (pared 2)
	glBindTexture(GL_TEXTURE_2D, textura[3].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f,  0.0f, 0.0f);
		glTexCoord2f(3.0f, 0.0f); glVertex3f(-60.0f,  0.0f, 0.0f);
		glTexCoord2f(3.0f, 1.0f); glVertex3f(-60.0f, 15.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 15.0f, 0.0f);
	glEnd();

	//Plano 4 (pared 3)
	glBindTexture(GL_TEXTURE_2D, textura[0].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f,  5.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-45.0f,  5.0f, 0.0f);
		glTexCoord2f(1.0f, 0.7f); glVertex3f(-45.0f, 15.0f, 0.0f);
		glTexCoord2f(0.0f, 0.7f); glVertex3f(-60.0f, 15.0f, 0.0f);
	glEnd();

	//Plano 5 (pared 4)
	glBindTexture(GL_TEXTURE_2D, textura[0].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(-0.6f, 0.0f); glVertex3f(-90.0f, 15.0f, -10.0f);
		glTexCoord2f( 1.4f, 0.0f); glVertex3f(-45.0f, 15.0f, -10.0f);
		glTexCoord2f( 1.4f, 1.0f); glVertex3f(-45.0f, 30.0f, -10.0f);
		glTexCoord2f(-0.6f, 1.0f); glVertex3f(-90.0f, 30.0f, -10.0f);
	glEnd();

	//Plano 6 (piso 2)
	glBindTexture(GL_TEXTURE_2D, textura[1].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 15.0f,   0.0f);
		glTexCoord2f(2.5f, 0.0f); glVertex3f(-45.0f, 15.0f,   0.0f);
		glTexCoord2f(2.5f, 1.0f); glVertex3f(-45.0f, 15.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 15.0f, -10.0f);
	glEnd();

	//Plano 7 (pared 5)
	glBindTexture(GL_TEXTURE_2D, textura[16].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.5f); glVertex3f(-90.0f, -20.0f, 20.0f);
		glTexCoord2f(1.0f, 0.5f); glVertex3f(-60.0f, -20.0f, 20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f,   0.0f, 20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f,   0.0f, 20.0f);
	glEnd();

	//Plano 8 (bloque piso)
	glBindTexture(GL_TEXTURE_2D, textura[5].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, 5.0f, 10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-45.0f, 5.0f, 10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-45.0f, 5.0f,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 5.0f,  0.0f);
	glEnd();

	//Plano 9 (bloque pared front)
	glBindTexture(GL_TEXTURE_2D, textura[6].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-45.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-45.0f, 5.0f, 10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 5.0f, 10.0f);
	glEnd();

	//Plano 10 (bloque pared izq)
	glBindTexture(GL_TEXTURE_2D, textura[6].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, 0.0f,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-60.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-60.0f, 5.0f, 10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-60.0f, 5.0f,  0.0f);
	glEnd();

	//Plano 11 (pared 6)
	glBindTexture(GL_TEXTURE_2D, textura[12].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, -20.0f, 10.0f);
		glTexCoord2f(4.0f, 0.0f); glVertex3f(-45.0f, -20.0f, 10.0f);
		glTexCoord2f(4.0f, 8.0f); glVertex3f(-45.0f,   0.0f, 10.0f);
		glTexCoord2f(0.0f, 8.0f); glVertex3f(-60.0f,   0.0f, 10.0f);
	glEnd();

	//Plano 12 (pared 7)
	glBindTexture(GL_TEXTURE_2D, textura[12].texID);

	glBegin(GL_QUADS);
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-60.0f, -20.0f, 20.0f);
		glTexCoord2f(4.0f, 0.0f); glVertex3f(-60.0f, -20.0f, 10.0f);
		glTexCoord2f(4.0f, 8.0f); glVertex3f(-60.0f,   0.0f, 10.0f);
		glTexCoord2f(0.0f, 8.0f); glVertex3f(-60.0f,   0.0f, 20.0f);
	glEnd();

	//Plano 13 (pared 8)
	glBindTexture(GL_TEXTURE_2D, textura[7].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-45.0f, 15.0f, -10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-45.0f, 15.0f,   0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-45.0f, 30.0f,   0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-45.0f, 30.0f, -10.0f);
	glEnd();

	//Plano 14 (pared 9)
	glBindTexture(GL_TEXTURE_2D, textura[8].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-45.0f,  5.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f,  5.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.0f, 30.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-45.0f, 30.0f, 0.0f);
	glEnd();

	//Plano 15 (piso 3)
	glBindTexture(GL_TEXTURE_2D, textura[1].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-45.0f, 30.0f,   0.0f);
		glTexCoord2f(2.5f, 0.0f); glVertex3f(  0.0f, 30.0f,   0.0f);
		glTexCoord2f(2.5f, 1.0f); glVertex3f(  0.0f, 30.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-45.0f, 30.0f, -10.0f);
	glEnd();

	//Plano 16 (piso 4)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f); glVertex3f(-45.0f, 8.0f, 22.0f);
		glTexCoord2f(14.0f, 0.0f); glVertex3f(  0.0f, 8.0f, 22.0f);
		glTexCoord2f(14.0f, 5.0f); glVertex3f(  0.0f, 8.0f,  0.0f);
		glTexCoord2f( 0.0f, 5.0f); glVertex3f(-45.0f, 8.0f,  0.0f);
	glEnd();

	//Plano 17 (pared 10)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-45.0f, 4.0f,  0.0f);
		glTexCoord2f(5.0f, 0.0f); glVertex3f(-45.0f, 4.0f, 22.0f);
		glTexCoord2f(5.0f, 2.0f); glVertex3f(-45.0f, 8.0f, 22.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f(-45.0f, 8.0f,  0.0f);
	glEnd();

	//Plano 18 (pared 11)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f( 0.0f, 0.0f); glVertex3f(-45.0f, 4.0f, 22.0f);
		glTexCoord2f(14.0f, 0.0f); glVertex3f(  0.0f, 4.0f, 22.0f);
		glTexCoord2f(14.0f, 2.0f); glVertex3f(  0.0f, 8.0f, 22.0f);
		glTexCoord2f( 0.0f, 2.0f); glVertex3f(-45.0f, 8.0f, 22.0f);
	glEnd();

	//Plano 19 (piso 5)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f); glVertex3f(-45.0f, 4.0f, 25.0f);
		glTexCoord2f(14.0f, 0.0f); glVertex3f(  0.0f, 4.0f, 25.0f);
		glTexCoord2f(14.0f, 1.0f); glVertex3f(  0.0f, 4.0f, 22.0f);
		glTexCoord2f( 0.0f, 1.0f); glVertex3f(-45.0f, 4.0f, 22.0f);
	glEnd();

	//Plano 20 (pared 12)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-45.0f, -6.0f,  0.0f);
		glTexCoord2f(6.0f, 0.0f); glVertex3f(-45.0f, -6.0f, 25.0f);
		glTexCoord2f(6.0f, 5.0f); glVertex3f(-45.0f,  4.0f, 25.0f);
		glTexCoord2f(0.0f, 5.0f); glVertex3f(-45.0f,  4.0f,  0.0f);
	glEnd();

	//Plano 21 (pared 13)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f( 0.0f, 0.0f); glVertex3f(-45.0f, -6.0f, 25.0f);
		glTexCoord2f(14.0f, 0.0f); glVertex3f(  0.0f, -6.0f, 25.0f);
		glTexCoord2f(14.0f, 5.0f); glVertex3f(  0.0f,  4.0f, 25.0f);
		glTexCoord2f( 0.0f, 5.0f); glVertex3f(-45.0f,  4.0f, 25.0f);
	glEnd();

	//Plano 22 (piso 6)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-25.0f, 10.0f, 8.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(  0.0f, 10.0f, 8.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f(  0.0f, 10.0f, 0.0f);
		glTexCoord2f(2.0f, 3.0f); glVertex3f(-25.0f, 10.0f, 0.0f);
	glEnd();

	//Plano 23 (piso 7)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-15.0f, 10.0f, 12.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(  0.0f, 10.0f, 12.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.0f, 10.0f,  8.0f);
		glTexCoord2f(1.0f, 3.0f); glVertex3f(-15.0f, 10.0f,  8.0f);
	glEnd();

	//Plano 24 (piso 8)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-7.0f, 10.0f, 16.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 10.0f, 16.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f, 10.0f, 12.0f);
		glTexCoord2f(1.0f, 3.0f); glVertex3f(-7.0f, 10.0f, 12.0f);
	glEnd();

	//Plano 25 (piso 9)
	glBindTexture(GL_TEXTURE_2D, textura[11].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-8.0f, 12.0f, 8.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 12.0f, 8.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f( 0.0f, 12.0f, 0.0f);
		glTexCoord2f(2.0f, 3.0f); glVertex3f(-8.0f, 12.0f, 0.0f);
	glEnd();

	//Plano 26 (pared 14)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f,  8.0f, 0.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f(-25.0f,  8.0f, 8.0f);
		glTexCoord2f(2.0f, 1.0f); glVertex3f(-25.0f, 10.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 10.0f, 0.0f);
	glEnd();

	//Plano 27 (pared 15)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f,  8.0f,  8.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0f,  8.0f, 12.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0f, 10.0f, 12.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f, 10.0f,  8.0f);
	glEnd();

	//Plano 28 (pared 16)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f,  8.0f, 12.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.0f,  8.0f, 16.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.0f, 10.0f, 16.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0f, 10.0f, 12.0f);
	glEnd();

	//Plano 29 (pared 17)
	glBindTexture(GL_TEXTURE_2D, textura[9].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 10.0f, 0.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f(-8.0f, 10.0f, 8.0f);
		glTexCoord2f(2.0f, 1.0f); glVertex3f(-8.0f, 12.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 12.0f, 0.0f);
	glEnd();

	//Plano 30 (pared 18)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f,  8.0f, 16.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f( 0.0f,  8.0f, 16.0f);
		glTexCoord2f(2.0f, 1.0f); glVertex3f( 0.0f, 10.0f, 16.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0f, 10.0f, 16.0f);
	glEnd();

	//Plano 31 (pared 19)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0f,  8.0f, 12.0f);
		glTexCoord2f(3.0f, 0.0f); glVertex3f(- 7.0f,  8.0f, 12.0f);
		glTexCoord2f(3.0f, 1.0f); glVertex3f(- 7.0f, 10.0f, 12.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0f, 10.0f, 12.0f);
	glEnd();

	//Plano 32 (pared 20)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f,  8.0f, 8.0f);
		glTexCoord2f(4.0f, 0.0f); glVertex3f(-15.0f,  8.0f, 8.0f);
		glTexCoord2f(4.0f, 1.0f); glVertex3f(-15.0f, 10.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 10.0f, 8.0f);
	glEnd();

	//Plano 33 (pared 21)
	glBindTexture(GL_TEXTURE_2D, textura[10].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 10.0f, 8.0f);
		glTexCoord2f(2.0f, 0.0f); glVertex3f( 0.0f, 10.0f, 8.0f);
		glTexCoord2f(2.0f, 1.0f); glVertex3f( 0.0f, 12.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 12.0f, 8.0f);
	glEnd();

	//Plano 34 (pared 22)
	glBindTexture(GL_TEXTURE_2D, textura[14].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.6f, 0.0f); glVertex3f(0.0f, -20.0f,  0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -20.0f, 30.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,  17.0f, 30.0f);
		glTexCoord2f(0.6f, 1.0f); glVertex3f(0.0f,  17.0f,  0.0f);
	glEnd();

	//Plano 35 (pared 23)
	glBindTexture(GL_TEXTURE_2D, textura[14].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, -20.0f, 30.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, -20.0f, 30.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f,  17.0f, 30.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  17.0f, 30.0f);
	glEnd();

	//Plano 36 (piso 10)
	glBindTexture(GL_TEXTURE_2D, textura[13].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 17.0f, 30.0f);
		glTexCoord2f(6.0f, 0.0f); glVertex3f(80.0f, 17.0f, 30.0f);
		glTexCoord2f(6.0f, 2.0f); glVertex3f(80.0f, 17.0f,  0.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f( 0.0f, 17.0f,  0.0f);
	glEnd();

	//Plano 37 (pared 24)
	glBindTexture(GL_TEXTURE_2D, textura[15].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(22.0f, 17.0f, 8.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, 17.0f, 8.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f, 35.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(22.0f, 35.0f, 8.0f);
	glEnd();

	//Plano 38 (pared 25)
	glBindTexture(GL_TEXTURE_2D, textura[7].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(22.0f, 17.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(22.0f, 17.0f, 8.0f);
		glTexCoord2f(1.0f, 2.0f); glVertex3f(22.0f, 35.0f, 8.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f(22.0f, 35.0f, 0.0f);
	glEnd();

	//Plano 39 (piso 11)
	glBindTexture(GL_TEXTURE_2D, textura[1].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(22.0f, 35.0f, 8.0f);
		glTexCoord2f(2.5f, 0.0f); glVertex3f(80.0f, 35.0f, 8.0f);
		glTexCoord2f(2.5f, 1.0f); glVertex3f(80.0f, 35.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(22.0f, 35.0f, 0.0f);
	glEnd();

	//Plano 40 (pared 26)
	glBindTexture(GL_TEXTURE_2D, textura[7].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 35.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, 35.0f, 8.0f);
		glTexCoord2f(1.0f, 2.0f); glVertex3f(80.0f, 50.0f, 8.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f(80.0f, 50.0f, 0.0f);
	glEnd();

	//Plano 41 (pared 27)
	glBindTexture(GL_TEXTURE_2D, textura[8].texID);

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 17.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, 17.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(80.0f, 50.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 50.0f, 0.0f);
	glEnd();

	//Plano 42 (pared 28)
	glBindTexture(GL_TEXTURE_2D, textura[7].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 30.0f, -10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 30.0f,   0.0f);
		glTexCoord2f(1.0f, 2.0f); glVertex3f(0.0f, 50.0f,   0.0f);
		glTexCoord2f(0.0f, 2.0f); glVertex3f(0.0f, 50.0f, -10.0f);
	glEnd();

	//Plano 43 (pared 29)
	glBindTexture(GL_TEXTURE_2D, textura[16].texID);

	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 17.0f,  8.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(80.0f, 17.0f, 30.0f);
		glTexCoord2f(1.0f, 0.7f); glVertex3f(80.0f, 50.0f, 30.0f);
		glTexCoord2f(0.0f, 0.7f); glVertex3f(80.0f, 50.0f,  8.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//Cajas

	//Caja 1
	glPushMatrix();
		glTranslatef(8.9f, 19.0f, 3.5f);
		DibujaCaja(2);
	glPopMatrix();

	//Caja 2
	glPushMatrix();
		glTranslatef(14.1f, 19.0f, 3.0f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 3
	glPushMatrix();
		glTranslatef(19.3f, 19.0f, 3.5f);
		DibujaCaja(3);
	glPopMatrix();

	//Caja 4
	glPushMatrix();
		glTranslatef(9.0f, 23.0f, 3.5f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 5
	glPushMatrix();
		glTranslatef(17.0f, 23.0f, 3.5f);
		glRotatef(-70.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.8f, 1.0f, 1.2f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 6
	glPushMatrix();
		glTranslatef(14.3f, 27.0f, 3.0f);
		DibujaCaja(3);
	glPopMatrix();

	//Caja 7
	glPushMatrix();
		glTranslatef(19.4f, 27.0f, 3.0f);
		DibujaCaja(3);
	glPopMatrix();

	//Caja 8
	glPushMatrix();
		glTranslatef(19.4f, 31.0f, 3.0f);
		DibujaCaja(2);
	glPopMatrix();

	//Caja 9
	glPushMatrix();
		glTranslatef(67.4f, 19.0f, 11.5f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(2);
	glPopMatrix();

	//Caja 10
	glPushMatrix();
		glTranslatef(72.5f, 19.0f, 11.0f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 11
	glPushMatrix();
		glTranslatef(77.5f, 19.0f, 11.5f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(3);
	glPopMatrix();

	//Caja 12
	glPushMatrix();
		glTranslatef(72.5f, 23.0f, 11.0f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 13
	glPushMatrix();
		glTranslatef(77.5f, 23.0f, 11.5f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(1);
	glPopMatrix();

	//Caja 14
	glPushMatrix();
		glTranslatef(77.5f, 27.0f, 11.5f);
		glScalef(1.0f, 1.0f, 0.8f);
		DibujaCaja(3);
	glPopMatrix();

	//Banda transportadora
	glPushMatrix();
		glTranslatef(71.0f, 18.5f, 19.0f);
		DibujaTransportadora();
	glPopMatrix();
}

void DibujaPersonaje()
{
	glPushMatrix();

		glTranslatef(Xtor, Ytor+1.3f, Ztor);
		glRotatef(Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(Angt1, 1.0f, 0.0f, 0.0f);
		
		//Torso
		g_Load3ds.Render3DSFile(&g_3DModel1c, textureModel1c, 1);

		//Cabeza
		glPushMatrix();
			glTranslatef(0.0f, 1.9f ,0.0f);
			glRotatef(Angc2, 0.0f, 1.0f, 0.0f);
			glRotatef(Angc1, 1.0f, 0.0f, 0.0f);
			g_Load3ds.Render3DSFile(&g_3DModel2c, textureModel2c, 1);
		glPopMatrix();

		//Pierna derecha
		glPushMatrix();
			glTranslatef(-0.6f, 0.1f ,0.1f);
			glRotatef(Angpder, 1.0f, 0.0f, 0.0f);
			g_Load3ds.Render3DSFile(&g_3DModel3c, textureModel3c, 1);

			//Pie derecho
			glPushMatrix();
				glTranslatef(0.07f, -0.85f ,0.0f);
				glRotatef(Angpd, 1.0f, 0.0f, 0.0f);
				g_Load3ds.Render3DSFile(&g_3DModel9c, textureModel9c, 1);
			glPopMatrix();

		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef(0.6f, 0.1f ,0.1f);
			glRotatef(Angpizq, 1.0f, 0.0f, 0.0f);
			g_Load3ds.Render3DSFile(&g_3DModel4c, textureModel4c, 1);

			//Pie izquierdo
			glPushMatrix();
				glTranslatef(-0.07f, -0.85f ,0.0f);
				glRotatef(Angpi, 1.0f, 0.0f, 0.0f);
				g_Load3ds.Render3DSFile(&g_3DModel9c, textureModel9c, 1);
			glPopMatrix();

		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef(-0.9f, 1.45f, -0.1f);
			glRotatef(Angbd2, 0.0f, 0.0f, 1.0f);
			glRotatef(Angbd1, 1.0f, 0.0f, 0.0f);
			g_Load3ds.Render3DSFile(&g_3DModel5c, textureModel5c, 1);

		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(0.9f, 1.45f, -0.1f);
			glRotatef(Angbi2, 0.0f, 0.0f, 1.0f);
			glRotatef(Angbi1, 1.0f, 0.0f, 0.0f);
			g_Load3ds.Render3DSFile(&g_3DModel7c, textureModel7c, 1);

		glPopMatrix();

	glPopMatrix();
}

void ActualizaLuzParam()
{
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz0
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
}

void AlturaPiso()
{
	if(player1.PosicionObj.x >= -90.0f && player1.PosicionObj.x < -60.0f &&
	   player1.PosicionObj.z >=   0.0f && player1.PosicionObj.z <  20.0f)
		altPiso=0.0f;
	else if(player1.PosicionObj.x >= -60.0f && player1.PosicionObj.x < -45.0f &&
	   player1.PosicionObj.z >=   0.0f && player1.PosicionObj.z <  10.0f)
		altPiso=5.0f;
	else if(player1.PosicionObj.x >= -45.0f && player1.PosicionObj.x < 0.0f &&
	   player1.PosicionObj.z >=   0.0f && player1.PosicionObj.z <  22.0f)
		altPiso=8.0f;
	else if(player1.PosicionObj.x >= 0.0f && player1.PosicionObj.x < 80.0f &&
	   player1.PosicionObj.z >=   0.0f && player1.PosicionObj.z <  30.0f)
		altPiso=17.0f;
	else
		altPiso=-50.0f;
}

void AnimacionSalto()
{
	if(player1.contAuxAnim < 40)
	{
		if(play == false)
		{
			Angt1   = KeyFrame2[0].Angt1;
			Angt2   = KeyFrame2[0].Angt2;
			Angc1   = KeyFrame2[0].Angc1;
			Angc2   = KeyFrame2[0].Angc2;
			Angbi1  = KeyFrame2[0].Angbi1;
			Angbi2  = KeyFrame2[0].Angbi2;
			Angbib  = KeyFrame2[0].Angbib;
			Angbd1  = KeyFrame2[0].Angbd1;
			Angbd2  = KeyFrame2[0].Angbd2;
			Angbdb  = KeyFrame2[0].Angbdb;
			Angpizq = KeyFrame2[0].Angpizq;
			Angpder = KeyFrame2[0].Angpder;
			Angpi   = KeyFrame2[0].Angpi;
			Angpd   = KeyFrame2[0].Angpd;
			Xtor    = KeyFrame2[0].Xtor;
			Ytor    = KeyFrame2[0].Ytor;
			Ztor    = KeyFrame2[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=2;
		}
		player1.contAuxAnim++;
	}
	else
	{
		player1.kick=false;
		play=false;
		Angt1   = 0.0f;
		Angt2   = 0.0f;
		Angc1   = 0.0f;
		Angc2   = 0.0f;
		Angbi1  = 0.0f;
		Angbi2  = 0.0f;
		Angbib  = 0.0f;
		Angbd1  = 0.0f;
		Angbd2  = 0.0f;
		Angbdb  = 0.0f;
		Angpizq = 0.0f;
		Angpder = 0.0f;
		Angpi   = 0.0f;
		Angpd   = 0.0f;
		Xtor    = 0.0f;
		Ytor    = 0.0f;
		Ztor    = 0.0f;
	}
}

void IndicadorVidas()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);

	glBindTexture(GL_TEXTURE_2D, textura[29].texID);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(				 0.0f,					0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(infGame.glWidth*0.2f,					0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(infGame.glWidth*0.2f, infGame.glHeight*0.3f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(				 0.0f, infGame.glHeight*0.3f);
	glEnd();

	glDisable(GL_ALPHA_TEST);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

//Cambios para Fuentes
void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyecci�n
	glPushMatrix();										// Guarda la matriz de proyecci�n
	glLoadIdentity();									// Limpia la matriz de proyecci�n
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n ortogonal
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda la matriz de modelo de vista
	
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	//Font.glPrint((2.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.9f,"Ejemplo texto");
	Font.glPrint((0.8f/640.0f)*infGame.glWidth, infGame.glWidth*0.85f,infGame.glHeight*0.95f,".FPS %d",FPS);

	if(detectado == 1)
		Font.glPrint((8.0f/640.0f)*infGame.glWidth, infGame.glWidth*0.01f,infGame.glHeight*0.382f,"BUSTED!");
		
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyecci�n
	glPopMatrix();										// Recupera la anterior matriz de proyecci�n
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
}

void dibujaLogoStudio()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	
	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
		glVertex2f(infGame.glWidth*0.3f, infGame.glHeight*0.29f);
		glVertex2f(infGame.glWidth*0.7f, infGame.glHeight*0.29f);
		glVertex2f(infGame.glWidth*0.7f, infGame.glHeight*0.59f);
		glVertex2f(infGame.glWidth*0.3f, infGame.glHeight*0.59f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex2f(infGame.glWidth*0.25f, infGame.glHeight*0.59f);
		glVertex2f(infGame.glWidth*0.75f, infGame.glHeight*0.59f);
		glVertex2f(infGame.glWidth*0.50f, infGame.glHeight*0.79f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.3f, infGame.glHeight*0.22f,"GREEN HOUSE");
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.37f, infGame.glHeight*0.165f,"STUDIOS");
			
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void dibujaPantalladeCarga()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.06f, infGame.glHeight*0.05f,"Cargando...");
			
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void dibujaPantalladeIntroduccion()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	if(timerEstados.TiempoActual() < 9000)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.15f, infGame.glHeight*0.48f,"   Habia una vez");
	else if(timerEstados.TiempoActual() >= 9000 && timerEstados.TiempoActual() < 13000)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.11f, infGame.glHeight*0.48f,"en un lugar muy lejano");
	else if(timerEstados.TiempoActual() >= 13000 && timerEstados.TiempoActual() < 17000)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.11f, infGame.glHeight*0.48f,"  llamado Chiluca...");
	else if(timerEstados.TiempoActual() >= 17000 && timerEstados.TiempoActual() < 21000)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.11f, infGame.glHeight*0.48f,"    personas muy...  ");
	else if(timerEstados.TiempoActual() > 21000 && timerEstados.TiempoActual() < 25000)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.14f, infGame.glHeight*0.48f,"   ... chilucotas   ");
			
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void dibujaTituloJuego()
{
	static float color=0.0f;

	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glColor3f(color, color, color);

	glBindTexture(GL_TEXTURE_2D, textura[30].texID);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(infGame.glWidth*0.0f, infGame.glHeight*0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(infGame.glWidth*1.0f, infGame.glHeight*0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(infGame.glWidth*1.0f, infGame.glHeight*1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(infGame.glWidth*0.0f, infGame.glHeight*1.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	if(color < 1.0f)
		color+=0.001f;
	
	if(infGame.estadoJuego == 5 && color >= 0.9f)
	{
		glAlphaFunc(GL_GREATER, 0.6f);
		glEnable(GL_ALPHA_TEST);

		if(infGame.opcionMenuSelec == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.38f, infGame.glHeight*0.34f,"  Start");

		if(infGame.opcionMenuSelec == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.38f, infGame.glHeight*0.28f,"Load game");

		if(infGame.opcionMenuSelec == 2)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.38f, infGame.glHeight*0.22f," Options");
		
		glColor3f(1.0f, 1.0f, 1.0f);
				
		glDisable(GL_ALPHA_TEST);
	}

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void dibujaMenuOpciones()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textura[30].texID);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(infGame.glWidth*0.0f, infGame.glHeight*0.0f);
		glTexCoord2f(0.5f, 0.0f); glVertex2f(infGame.glWidth*1.0f, infGame.glHeight*0.0f);
		glTexCoord2f(0.5f, 0.3f); glVertex2f(infGame.glWidth*1.0f, infGame.glHeight*1.0f);
		glTexCoord2f(0.0f, 0.3f); glVertex2f(infGame.glWidth*0.0f, infGame.glHeight*1.0f);
	glEnd();
	
	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	if(infGame.opMenuPrinc.opcionSelec == 0)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.15f, infGame.glHeight*0.8f,"Dificultad:");

	if(infGame.opMenuPrinc.opcionSelec == 1)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.15f, infGame.glHeight*0.6f,"Num. vidas:");

	if(infGame.opMenuPrinc.opcionSelec == 2)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.15f, infGame.glHeight*0.4f,"Sonido:");

	if(infGame.opMenuPrinc.opcionSelec == 3)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.15f, infGame.glHeight*0.2f,"Regresar");
	
	glColor3f(1.0f, 1.0f, 1.0f);

	//Opciones de dificultad
	if(infGame.opMenuPrinc.dificultad == 0)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.8f,"Facil");
	else if(infGame.opMenuPrinc.dificultad == 1)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.8f,"Normal");
	else if(infGame.opMenuPrinc.dificultad == 2)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.8f,"Dificil");

	//N�mero de vidas
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.6f,"%d", infGame.opMenuPrinc.numVidas);

	//Sonido
	if(infGame.opMenuPrinc.sonido == 0)
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.4f,"Off");
	else
		Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.6f, infGame.glHeight*0.4f,"On");
	
				
	glDisable(GL_ALPHA_TEST);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void dibujaMenuPausa()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPushMatrix();										// Guarda la matriz de proyeccion
	glLoadIdentity();									// Limpia la matriz de proyeccion
	glOrtho(0,infGame.glWidth,0,infGame.glHeight,-1,1);					// Crea una proyecci�n paralela
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda matriz de modelo de vista
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	
	glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
		glVertex2f(infGame.glWidth*0.15f, infGame.glHeight*0.27f);
		glVertex2f(infGame.glWidth*0.85f, infGame.glHeight*0.27f);
		glVertex2f(infGame.glWidth*0.85f, infGame.glHeight*0.73f);
		glVertex2f(infGame.glWidth*0.15f, infGame.glHeight*0.73f);
	glEnd();
	
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);

	if(infGame.opcionMenuPausa == 0)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.26f, infGame.glHeight*0.61f,"Guargar juego");

	if(infGame.opcionMenuPausa == 1)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.25f, infGame.glHeight*0.46f,"Menu principal");

	if(infGame.opcionMenuPausa == 2)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 1.0f, 1.0f);
	Font.glPrint((2.2f/640.0f)*infGame.glWidth, infGame.glWidth*0.4f, infGame.glHeight*0.31f,"Salir");

	glColor3f(1.0f, 1.0f, 1.0f);
		
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyeccion
	glPopMatrix();										// Restaura la matriz de proyeccion anterior
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Restaura la matriz de modelo de vista anterior
	glEnable(GL_DEPTH_TEST);							// Activa prueba de profundidad
}

void ejemploBackFaceCulling()
{
	CVector vert[4];
	CVector N, vec1, vec2;
	CVector V;
	CVector pm;

	int visible=0;

	vert[0]=CVector(-70.0f,  0.0f, 0.0f);
	vert[1]=CVector(-50.0f,  0.0f, 0.0f);
	vert[2]=CVector(-50.0f, 20.0f, 0.0f);
	vert[3]=CVector(-70.0f, 20.0f, 0.0f);

	vec1=vert[1]-vert[0];
	vec2=vert[3]-vert[0];

	N=Normaliza(Cruz(vec1, vec2));

	//Punto medio
	pm=(vert[0]+vert[1]+vert[2]+vert[3])/4.0f;

	//Vector hacia la c�mara (V)
	V=Normaliza(CVector(player1.PosicionObj.x-10.0f, player1.PosicionObj.y+30.0f, player1.PosicionObj.z+40.0f)-pm);

	//Prueba con el producto punto
	if(Punto(N, V) > 0.0f)
		visible=1;

	if(visible == 1)
	{
		glBegin(GL_QUADS);
			glVertex3f(vert[0].x, vert[0].y, vert[0].z);
			glVertex3f(vert[1].x, vert[1].y, vert[1].z);
			glVertex3f(vert[2].x, vert[2].y, vert[2].z);
			glVertex3f(vert[3].x, vert[3].y, vert[3].z);
		glEnd();
	}
}

void dibujaVolumendeSombra()
{
	CVector posLuz=CVector(player1.PosicionObj.x-3.0f, player1.PosicionObj.y+35.0f, player1.PosicionObj.z-1.0f);
	CVector v1=CVector(player1.PosicionObj.x-1.5f, player1.PosicionObj.y+4.0f, player1.PosicionObj.z+1.5f);
	CVector v2=CVector(player1.PosicionObj.x+1.5f, player1.PosicionObj.y+4.0f, player1.PosicionObj.z+1.5f);
	CVector v3=CVector(player1.PosicionObj.x+1.5f, player1.PosicionObj.y+4.0f, player1.PosicionObj.z-1.5f);
	CVector v4=CVector(player1.PosicionObj.x-1.5f, player1.PosicionObj.y+4.0f, player1.PosicionObj.z-1.5f);

	CVector v1d=Normaliza(v1-posLuz);
	CVector v2d=Normaliza(v2-posLuz);
	CVector v3d=Normaliza(v3-posLuz);
	CVector v4d=Normaliza(v4-posLuz);

	CVector v1p=v1+v1d*200.0f;
	CVector v2p=v2+v2d*200.0f;
	CVector v3p=v3+v3d*200.0f;
	CVector v4p=v4+v4d*200.0f;

	//1
	glBegin(GL_QUADS);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);
	glEnd();

	//2
	glBegin(GL_QUADS);
		glVertex3f(v1p.x, v1p.y, v1p.z);
		glVertex3f(v2p.x, v2p.y, v2p.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v1.x, v1.y, v1.z);
	glEnd();

	//3
	glBegin(GL_QUADS);
		glVertex3f(v4p.x, v4p.y, v4p.z);
		glVertex3f(v1p.x, v1p.y, v1p.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v4.x, v4.y, v4.z);
	glEnd();

	//4
	glBegin(GL_QUADS);
		glVertex3f(v2p.x, v2p.y, v2p.z);
		glVertex3f(v3p.x, v3p.y, v3p.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v2.x, v2.y, v2.z);
	glEnd();

	//5
	glBegin(GL_QUADS);
		glVertex3f(v3p.x, v3p.y, v3p.z);
		glVertex3f(v4p.x, v4p.y, v4p.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v3.x, v3.y, v3.z);
	glEnd();

	//6
	glBegin(GL_QUADS);
		glVertex3f(v2p.x, v2p.y, v2.z);
		glVertex3f(v1p.x, v1p.y, v1p.z);
		glVertex3f(v4p.x, v4p.y, v4p.z);
		glVertex3f(v3p.x, v3p.y, v3p.z);
	glEnd();
}

void dibujaVolumendeSombra2()
{
	CVector vert[2];
	CVector vertP[2];
	CVector vertD[2];
	CVector centro, centroP, centroD;

	CVector posLuz=CVector(player1.PosicionObj.x-3.0f, player1.PosicionObj.y+35.0f, player1.PosicionObj.z-1.0f);

	float ang, deltaAng;

	deltaAng=360.0f/16;

	for(int i=0; i<16; i++)
	{
		ang=i*deltaAng;
		vert[0].x=player1.PosicionObj.x+1.5f*cos(ang*PI/180.0f);
		vert[0].y=player1.PosicionObj.y+2.5f;
		vert[0].z=player1.PosicionObj.z+1.5f*sin(ang*PI/180.0f);

		ang=(i+1)*deltaAng;
		vert[1].x=player1.PosicionObj.x+1.5f*cos(ang*PI/180.0f);
		vert[1].y=player1.PosicionObj.y+2.5f;
		vert[1].z=player1.PosicionObj.z+1.5f*sin(ang*PI/180.0f);

		centro.x=player1.PosicionObj.x;
		centro.y=player1.PosicionObj.y+2.5f;
		centro.z=player1.PosicionObj.z;

		vertD[0]=Normaliza(vert[0]-posLuz);
		vertD[1]=Normaliza(vert[1]-posLuz);
		centroD=Normaliza(centro-posLuz);

		vertP[0]=vert[0]+vertD[0]*200.0f;
		vertP[1]=vert[1]+vertD[1]*200.0f;
		centroP=centro+centroD*200.0f;

		glBegin(GL_TRIANGLES);
			glVertex3f(vert[1].x, vert[1].y, vert[1].z);
			glVertex3f(vert[0].x, vert[0].y, vert[0].z);
			glVertex3f(player1.PosicionObj.x, player1.PosicionObj.y+4.0f, player1.PosicionObj.z);
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f(vertP[1].x, vertP[1].y, vertP[1].z);
			glVertex3f(vertP[0].x, vertP[0].y, vertP[0].z);
			glVertex3f(vert[0].x, vert[0].y, vert[0].z);
			glVertex3f(vert[1].x, vert[1].y, vert[1].z);
		glEnd();

		glBegin(GL_TRIANGLES);
			glVertex3f(vertP[0].x, vertP[0].y, vertP[0].z);
			glVertex3f(vertP[1].x, vertP[1].y, vertP[1].z);
			glVertex3f(centroP.x, centroP.y, centroP.z);
		glEnd();

	}
}

void dibujaModelo()
{
	//Objeto 1

	//Plano 1
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-60.0f,  5.0f, 5.0f);
		glVertex3f(-40.0f,  5.0f, 5.0f);
		glVertex3f(-40.0f, 15.0f, 5.0f);
		glVertex3f(-60.0f, 15.0f, 5.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-40.0f,  5.0f,  5.0f);
		glVertex3f(-40.0f,  5.0f, 15.0f);
		glVertex3f(-40.0f, 15.0f, 15.0f);
		glVertex3f(-40.0f, 15.0f,  5.0f);
	glEnd();
	
}

void guardaModelo()
{
	FILE *F;
	float var3F[3];
	float var1F[1];
	int   var1I[1];

	F=fopen("modelo1.mod", "wb");

	//Num. de objetos
	var1I[0]=datosModelo.numObjetos;;
	fwrite(var1I, sizeof(int), 1, F);

	for(int i=0; i<datosModelo.numObjetos; i++)
	{
		var1I[0]=datosModelo.obj[i].numPlanos;
		fwrite(var1I, sizeof(int), 1, F);

		for(int j=0; j<datosModelo.obj[i].numPlanos; j++)
		{
			var3F[0]=datosModelo.obj[i].pl[j].normal.x;
			var3F[1]=datosModelo.obj[i].pl[j].normal.y;
			var3F[2]=datosModelo.obj[i].pl[j].normal.z;
			fwrite(var3F, sizeof(float), 3, F);

			var1I[0]=datosModelo.obj[i].pl[j].tieneTextura;
			fwrite(var1I, sizeof(int), 1, F);

			var1I[0]=datosModelo.obj[i].pl[j].tieneMaterial;
			fwrite(var1I, sizeof(int), 1, F);

			var1I[0]=datosModelo.obj[i].pl[j].idText;
			fwrite(var1I, sizeof(int), 1, F);

			var1I[0]=datosModelo.obj[i].pl[j].idMat;
			fwrite(var1I, sizeof(int), 1, F);

			for(int k=0; k<4; k++)
			{
				var3F[0]=datosModelo.obj[i].pl[j].vert[k].pos.x;
				var3F[1]=datosModelo.obj[i].pl[j].vert[k].pos.y;
				var3F[2]=datosModelo.obj[i].pl[j].vert[k].pos.z;
				fwrite(var3F, sizeof(float), 3, F);

				var1F[0]=datosModelo.obj[i].pl[j].vert[k].s;
				fwrite(var1F, sizeof(float), 1, F);

				var1F[0]=datosModelo.obj[i].pl[j].vert[k].t;
				fwrite(var1F, sizeof(float), 1, F);
			}
		}
	}
}

void cargaModelo()
{
	FILE *F;
	float var3F[3];
	float var1F[1];
	int   var1I[1];

	F=fopen("modelo1.mod", "rb");

	//Num. de objetos
	fread(var1I, sizeof(int), 1, F);
	datosModelo.numObjetos=var1I[0];

	datosModelo.obj=new objeto[datosModelo.numObjetos];

	for(int i=0; i<datosModelo.numObjetos; i++)
	{
		fread(var1I, sizeof(int), 1, F);
		datosModelo.obj[i].numPlanos=var1I[0];

		datosModelo.obj[i].pl=new plano[datosModelo.obj[i].numPlanos];

		for(int j=0; j<datosModelo.obj[i].numPlanos; j++)
		{
			fread(var3F, sizeof(float), 3, F);
			datosModelo.obj[i].pl[j].normal.x=var3F[0];
			datosModelo.obj[i].pl[j].normal.y=var3F[1];
			datosModelo.obj[i].pl[j].normal.z=var3F[2];

			fread(var1I, sizeof(int), 1, F);
			datosModelo.obj[i].pl[j].tieneTextura=var1I[0];

			fread(var1I, sizeof(int), 1, F);
			datosModelo.obj[i].pl[j].tieneMaterial=var1I[0];

			fread(var1I, sizeof(int), 1, F);
			datosModelo.obj[i].pl[j].idText=var1I[0];

			fread(var1I, sizeof(int), 1, F);
			datosModelo.obj[i].pl[j].idMat=var1I[0];

			for(int k=0; k<4; k++)
			{
				fread(var3F, sizeof(float), 3, F);
				datosModelo.obj[i].pl[j].vert[k].pos.x=var3F[0];
				datosModelo.obj[i].pl[j].vert[k].pos.y=var3F[1];
				datosModelo.obj[i].pl[j].vert[k].pos.z=var3F[2];

				fread(var1F, sizeof(float), 1, F);
				datosModelo.obj[i].pl[j].vert[k].s=var1F[0];

				fread(var1F, sizeof(float), 1, F);
				datosModelo.obj[i].pl[j].vert[k].t=var1F[0];
			}
		}
	}
}

void dibujaModeloArchivo()
{
	//Num. de objetos
	for(int i=0; i<datosModelo.numObjetos; i++)
	{
		for(int j=0; j<datosModelo.obj[i].numPlanos; j++)
		{
			glBegin(GL_QUADS);
				glNormal3f(datosModelo.obj[i].pl[j].normal.x,
						   datosModelo.obj[i].pl[j].normal.y,
						   datosModelo.obj[i].pl[j].normal.z);
			
				for(int k=0; k<4; k++)
				{
					glVertex3f(datosModelo.obj[i].pl[j].vert[k].pos.x,
							   datosModelo.obj[i].pl[j].vert[k].pos.y,
							   datosModelo.obj[i].pl[j].vert[k].pos.z);

				}

			glEnd();
		}
	}
}

void dibujaSpline()
{
	float t;
	CVector ter3,ter2,ter1,ter;
	CVector Punto;

	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_LINE_STRIP);

	float delta_t=1.0f/resolucion;

	for(int i=1; i<18; i++) //max-2
	{
		ter3=puntosdeControl[i-1]*-1.0f+puntosdeControl[i]*3.0f+puntosdeControl[i+1]*-3.0f+puntosdeControl[i+2];
		ter2=puntosdeControl[i-1]*3.0f+puntosdeControl[i]*-6.0f+puntosdeControl[i+1]*3.0f;
		ter1=puntosdeControl[i-1]*-3.0f+puntosdeControl[i+1]*3.0f;
		ter=puntosdeControl[i-1]+puntosdeControl[i]*4.0f+puntosdeControl[i+1];

		for(int j=0; j<resolucion; j++)
		{
			t=j*delta_t;
			Punto=(ter3*pow(t,3)+ter2*pow(t,2)+ter1*t+ter)/6.0f;

			glVertex3f(Punto.x, Punto.y, Punto.z);
		}
	}

	glEnd();

	glPointSize(10.0f);

	glBegin(GL_POINTS);

		for(int i=0; i<20; i++)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			
			glVertex3f(puntosdeControl[i].x, puntosdeControl[i].y, puntosdeControl[i].z);
		}

	glEnd();

	glPointSize(1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
}

void movimientoSpline()
{
	float t;
	float tAux;
	CVector ter3,ter2,ter1,ter;
	CVector Punto;

	float delta_t=1.0f/resolucion;
	static int pcAct=1;
	static int segAct=0;

	ter3=puntosdeControl[pcAct-1]*-1.0f+puntosdeControl[pcAct]*3.0f+puntosdeControl[pcAct+1]*-3.0f+puntosdeControl[pcAct+2];
	ter2=puntosdeControl[pcAct-1]*3.0f+puntosdeControl[pcAct]*-6.0f+puntosdeControl[pcAct+1]*3.0f;
	ter1=puntosdeControl[pcAct-1]*-3.0f+puntosdeControl[pcAct+1]*3.0f;
	ter=puntosdeControl[pcAct-1]+puntosdeControl[pcAct]*4.0f+puntosdeControl[pcAct+1];

	t=segAct*delta_t;
	Punto=(ter3*pow(t,3)+ter2*pow(t,2)+ter1*t+ter)/6.0f;
	
	pAct=Punto;
	if(pSpAct < totalP-1)
	{
		if(segAct < resolucion-1)
		{
			tAux=(segAct+1)*delta_t;
			pSig=(ter3*pow(tAux,3)+ter2*pow(tAux,2)+ter1*tAux+ter)/6.0f;
		}
		else
		{
			tAux=0.0f;
			if(pcAct < 18)
			{
				CVector ter_=puntosdeControl[(pcAct+1)-1]+puntosdeControl[(pcAct+1)]*4.0f+puntosdeControl[(pcAct+1)+1];
				pSig=ter_/6.0f;
			}
			else
			{
				CVector ter_=puntosdeControl[1-1]+puntosdeControl[1]*4.0f+puntosdeControl[1+1];
				pSig=ter_/6.0f;
			}
		}
		
		DirSp=Normaliza(pSig-pAct);
	}
	

	if(segAct < resolucion-1)
	{
		segAct++;
		pSpAct++;
	}
	else
	{
		if(pcAct < 18) //max-2
			pcAct++;
		else
		{
			pcAct=1;
			pSpAct=0;
			if(t > 0.9f)
				infGame.estadoJuego=6;
		}

		if(pcAct < 18) 
			segAct=0;
	}

	camSpline=Punto;
	objSpline=Punto+DirSp*5.0f;
	//objSpline=CVector(0.0f, 0.0f, 0.0f);
}

//PART�CULAS -- cambio 4

//Aqui se dibuja cada part�cula que consiste en un peque�o plano con alguna textura que representa algun
//efecto como humo, fuego, vapor, agua, etc. Debido a que se dibujan muchos planos y una forma de quitarle
//trabajo a la m�quina ser�a dibujar directamente los dos triangulos que forman cada plano en vez de usar
//GL_QUADS se usa GL_TRIANGLE_STRIP para dibujar 2 triangulos en un solo paso, solo indicando 4 vertices.

//A cada particula se le aplica transparencia y la combinaci�n de par�metros de glBlendFunc depende del efecto
//que se quiera representar. Antes de dibujar el plano se desactiva iluminaci�n y se aplica un color de 4 componentes
//usando la vida de la part�cula como valor de transparencia alpha

//Se actualiza la posici�n de cada part�cula usando los valores definidos para velocidad y gravedad y se disminuye su vida
//en una cantidad definida por la variable fade.
//Cuando una part�cula llega a cierto valor que se define segun el efecto a representar, se reinicia su posici�n y la vida
//de la part�cula tambien. En ese paso se pueden hacer calculos con alguna funci�n (por ejemplo senoidal) para definir un
//patr�n de movimiento de las part�culas. asignando el resultado a la nueva velocidad de la part�cula
void DibujaParticulas()
{
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);

	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_2D, textura[31].texID);

	for(int loop=0; loop<MAX_PARTICULAS; loop++)
	{
		float x=particle[loop].x;
		float y=particle[loop].y;
		float z=particle[loop].z;

		// Se dibuja la particula usando los valores RGB, El desvanecimiento depende de su vida
		glColor4f(0.3f,0.9f,0.2f,particle[loop].life);

		glBegin(GL_TRIANGLE_STRIP); // Se construye un Quad a partir de un Triangle Strip
			glTexCoord2f(1,1); glVertex3f(x+1.0f,y+1.0f,z); // Superior derecha
			glTexCoord2f(0,1); glVertex3f(x-1.0f,y+1.0f,z); // Inferior derecha
			glTexCoord2f(1,0); glVertex3f(x+1.0f,y-1.0f,z); // Superior izquierda
			glTexCoord2f(0,0); glVertex3f(x-1.0f,y-1.0f,z); // Inferior izquierda
		glEnd();

		particle[loop].x+=particle[loop].xi;	// Movimiento en el eje X a velocidad X
		particle[loop].y+=particle[loop].yi;	// Movimiento en el eje Y a velocidad Y
		particle[loop].z+=particle[loop].zi;	// Movimiento en el eje Z a velocidad Z

		//Efecto de gravedad
		particle[loop].xi+=particle[loop].xg;  
		particle[loop].yi+=particle[loop].yg;
		particle[loop].zi+=particle[loop].zg;

		particle[loop].life-=particle[loop].fade; // Se reduce la vida de las part�culas una cantidad "Fade"

		if (particle[loop].life < 0.7f) // Si la vida de la part�cula esta por agotarse
		{
			particle[loop].life=1.0f; // Se le da nueva vida
			particle[loop].fade=float(rand()%100)/1000.0f+0.005f; // Un valor aleatorio de desvanecimiento
			
			//Posici�n inicial de cada particula
			particle[loop].x= float((rand()%10)-5.0f)/10.0f;
			particle[loop].y= float((rand()%100)-50.0f)/10.0f;
			particle[loop].z= float((rand()%10)-5.0f)/10.0f;

			//Gravedad
			particle[loop].xg= 0.0f;
			particle[loop].yg= 0.01f;
			particle[loop].zg= 0.0f;
			
			//Velocidad
			particle[loop].xi = float((rand()%60)-3.0f)/100.0f;
			particle[loop].yi = float((rand()%6)-3.0f)/100.0f;
			particle[loop].zi = float((rand()%10)-5.0f)/100.0f;
		} 
	}      

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDepthMask(GL_TRUE);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);
}

int RenderizaEscena(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
		
	//gluLookAt(-100.0f, 30.0f, 90.0f, -30.0f, 25.0f, 0.0f, 0, 1, 0);
	/*if(infGame.estadoJuego == 9)
	{
		movimientoSpline();
		gluLookAt(camSpline.x, camSpline.y, camSpline.z, objSpline.x, objSpline.y, objSpline.z, 0.0f, 1.0f, 0.0f);
	}*/

	//else if(infGame.estadoJuego == 6)
		gluLookAt(player1.PosicionObj.x-10.0f, player1.PosicionObj.y+30.0f, player1.PosicionObj.z+40.0f, 
				  player1.PosicionObj.x, player1.PosicionObj.y+18.0f, player1.PosicionObj.z, 
				  0.0f, 1.0f, 0.0f);

	//AlturaPiso();
					
	ActualizaLuzParam();
	
	if(player1.kick == true)
		AnimacionSalto();

	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	DibujaEscenario();
	//dibujaSpline();
	ejemploDeteccionCamara();

	dibujaCajaColision(&cajaPersonaje);
	for(int i=0; i < 7; i++)
		dibujaCajaColision(&cajaEscenario[i]);
	dibujaEsferadeColision(&esferaPersonaje);
	dibujaEsferadeColision(&esferaEnemigo);

	if(player1.visible == true)
	{
		glPushMatrix();
			glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y, player1.PosicionObj.z);
			glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
			glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
			DibujaPersonaje();
		glPopMatrix();
	}

	glColorMask(0,0,0,0);
	glDepthMask(0);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	//1er paso de la prueba de pase de profundidad
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glCullFace(GL_BACK);
	dibujaVolumendeSombra2();

	//2do paso de la prueba de pase de profundidad
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	glCullFace(GL_FRONT);
	dibujaVolumendeSombra2();

	glCullFace(GL_BACK);

	glColorMask(1,1,1,1);
	glDepthMask(1);

	glStencilFunc(GL_NOTEQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);

	DibujaEscenario();
	ejemploDeteccionCamara();

	if(player1.visible == true)
	{
		glPushMatrix();
			glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y, player1.PosicionObj.z);
			glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
			glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
			DibujaPersonaje();
		glPopMatrix();
	}

	glDisable(GL_STENCIL_TEST);

	//dibujaModelo();
	dibujaModeloArchivo();

	//PART�CULAS -- cambio 6

	//Las part�culas deben estar orientada hacia la c�mara siempre, ya que de otro modo, por ejemplo si se ven desde un lado
	//se notar�a el "truco", que solo son un conjunto de planos texturizados moviendose con cierto patr�n y velocidad para
	//representar el efecto. Para evitar eso se usa una t�cnica llamada billboarding que consiste en calcular el �ngulo que
	//deben girarte las part�culas para que siempre "vean" a la c�mara

	//Dependiendo el efecto y el objeto al que se asocian las part�culas en la escena, el billboarding se puede calcular para
	//cada part�cula o para todo el grupo como en este ejemplo.

	//1. Se define el valor de la normal para cada part�cula o para el grupo teniendo en cuenta su posici�n y �ngulo inicial
	//2. Se calcula un vector que va desde cada part�cula o la posici�n del grupo hacia la c�mara
	//3. Se determina el angulo que hay entre esos dos vectores y se usa para rotar cada part�cula o el grupo, de modo que la normal
	//ahora apuntar�a hacia la c�mara.

	CVector N, ObjCamVec, up;
	float angcos;

	//Billboarding
	N=CVector(0.0f, 0.0f, 1.0f); //Orientaci�n original del objeto
	ObjCamVec=CVector(player1.PosicionObj.x-10.0f, player1.PosicionObj.y+30.0f, player1.PosicionObj.z+40.0f)-player1.PosicionObj;

	//N.y=0.0f;
	ObjCamVec.y=0.0f;
	
	ObjCamVec=Normaliza(ObjCamVec);

	up=Cruz(N, ObjCamVec);
	angcos=Punto(N, ObjCamVec);
	float AngObj=(float)acos(angcos)*180.0f/PI;
	
	//PART�CULAS -- cambio 5
	//Se incorpora el dibujo de las part�culas en el lugar deseado en la escena
	/*glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+2.5f, player1.PosicionObj.z);
		glRotatef(AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(3.0f, 3.0f, 3.0f);
		DibujaParticulas();
	glPopMatrix();*/

	IndicadorVidas();
	DibujaTextos();

	actualizaMovPersonaje();

	//Cambios para FPS
	CalculateFrameRate();

	return TRUE;
}

void dibujaSegunEstado()
{
	if(infGame.estadoJuego == 1)
	{
		/*glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		dibujaLogoStudio();*/
	}
	else if(infGame.estadoJuego == 2)
	{
		/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		dibujaPantalladeCarga();*/
	}
	else if(infGame.estadoJuego == 3)
	{
		/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		dibujaPantalladeIntroduccion();*/
	}
	else if(infGame.estadoJuego == 4 || infGame.estadoJuego == 5)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		dibujaTituloJuego();
	}
	else if(infGame.estadoJuego == 6 || infGame.estadoJuego == 9)
	{
		RenderizaEscena();
		if(infGame.pausa == 1)
			dibujaMenuPausa();

		if(infGame.pausa == 0)
		{
			if(play)
			{
				if(tipoAnim == 1) //caminar
					animacion(KeyFrame1, maxKF1 , 15);
				else if(tipoAnim == 2) //hurricane kick
					animacion(KeyFrame2, maxKF2 , 3);
				else if(tipoAnim == 3) //salto
					animacion(KeyFrame3, maxKF3 , 10);
			}
		}
	}
	else if(infGame.estadoJuego == 8)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		dibujaMenuOpciones();
	}
}

GLvoid DestruyeVentanaOGL(GLvoid)						// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalizaci�n",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalizaci�n",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalizaci�n",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalizaci�n",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalizaci�n",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este c�digo crea la ventana de OpenGL.  Par�metros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- N�mero de bits a usar para el color (8/16/24/32)	
  
BOOL CreaVentanaOGL(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rect�ngulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el �cono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay men� en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tama�o especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",							// Nombre de la clase
								title,								// T�tulo de la ventana
								dwStyle |							// Definici�n del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posici�n de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay men�
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		DestruyeVentanaOGL();										// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontr� un formato de pixel compatible
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReDimensionaEscenaGL(width, height);	// Inicia la perspectiva para la ventana OGL

	if (!IniGL())							// Si no se inicializa la ventana creada
	{
		DestruyeVentanaOGL();				// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicializaci�n.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Informaci�n adicional del mensaje
							LPARAM	lParam)	// Informaci�n adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activaci�n de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimizaci�n
			{
				active=TRUE;				// El programa est� activo
			}
			else
			{
				active=FALSE;				// El programa no est� activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// �Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// �Monitor tratando de entrar a modo de ahorro de energ�a?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se est� presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es as�, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// �Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es as�, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReDimensionaEscenaGL(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la funci�n principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	
	infGame.estadoJuego=0; //6 o 9 entra directo al juego
	infGame.glHeight=0;
	infGame.glWidth=0;

	static int estadoTeclaEsc=0;

	audioFunc.iniciaAudio();

	// Crea la ventana OpenGL
	if (!CreaVentanaOGL("Computaci�n Gr�fica Avanzada",640,480,16))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
			// Dibuja la escena. 
			if (active)								// Si est� activo el programa...
			{
				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					if(estadoTeclaEsc == 0)
					{
						if(infGame.pausa == 0)
						{
							infGame.opcionMenuPausa=0;
							infGame.pausa=1;
						}
						else
							infGame.pausa=0;

						estadoTeclaEsc=1;
					}
				}
				else								// De lo contrario, actualiza la pantalla
				{
					estadoTeclaEsc=0;

					audioFunc.actualizaSistemaAudio();

					//RenderizaEscena();				// Dibuja la escena
					dibujaSegunEstado();
					
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				//if(!ManejaTeclado()) return 0;
				controlFunc.actualizaControl();
				manejaEventosMain();
				controlEstados();
			}
			
		}
	}

	// Finalizaci�n del programa
	sonidoF[0].liberaSonido();
	sonidoF[1].liberaSonido();
	sonidoF[2].liberaSonido();
	audioFunc.finalizaAudio();
	DescargaTexturas();
	DescargaModelos();
	//Cambios para Fuentes
	Font.DestroyFont();
	DestruyeVentanaOGL();							// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

void guardaJuego()
{
	FILE *F;
	float varF[3];
	float varF1[1];
	int varI[1];

	F=fopen("save.cga", "wb");

	varI[0]=infGame.estadoJuego;
	fwrite(varI, sizeof(int), 1, F);

	//pos player
	varF[0]=player1.PosicionObj.x;
	varF[1]=player1.PosicionObj.y;
	varF[2]=player1.PosicionObj.z;
	fwrite(varF, sizeof(float), 3, F);

	//Ang obj player
	varF1[0]=player1.AngObj;
	fwrite(varF1, sizeof(float), 1, F);

	//pos player
	varF[0]=player1.escalaX;
	varF[1]=player1.escalaY;
	varF[2]=player1.escalaZ;
	fwrite(varF, sizeof(float), 3, F);

	fclose(F);
}

void cargaJuego()
{
	FILE *F;
	float varF[3];
	float varF1[1];
	int varI[1];

	F=fopen("save.cga", "rb");

	fread(varI, sizeof(int), 1, F);
	infGame.estadoJuego=varI[0];

	//pos player
	fread(varF, sizeof(float), 3, F);
	player1.PosicionObj.x=varF[0];
	player1.PosicionObj.y=varF[1];
	player1.PosicionObj.z=varF[2];

	//angObj player
	fread(varF1, sizeof(float), 1, F);
	player1.AngObj=varF1[0];

	//escala player
	fread(varF, sizeof(float), 3, F);
	player1.escalaX=varF[0];
	player1.escalaY=varF[1];
	player1.escalaZ=varF[2];

	fclose(F);

	if(infGame.opMenuPrinc.sonido == 1)
	{
		sonidoF[2].pausaSonido(true);
		sonidoF[0].volumenSonido(0.5f);
		sonidoF[0].pausaSonido(false);
	}
}


















void guardaConfiguracion()
{
	FILE *F;
	int valI[1];

	F=fopen("configuracion.jue", "wb");

	//Dificultad
	valI[0]=infGame.opMenuPrinc.dificultad;
	fwrite(valI, sizeof(int), 1, F);

	//Num. vidas
	valI[0]=infGame.opMenuPrinc.numVidas;
	fwrite(valI, sizeof(int), 1, F);

	//Sonido
	valI[0]=infGame.opMenuPrinc.sonido;
	fwrite(valI, sizeof(int), 1, F);

	fclose(F);
}

void cargaConfiguracion()
{
	FILE *F;
	int valI[1];

	F=fopen("configuracion.jue", "rb");

	//Dificultad
	fread(valI, sizeof(int), 1, F);
	infGame.opMenuPrinc.dificultad=valI[0];
	
	//Num. vidas
	fread(valI, sizeof(int), 1, F);
	infGame.opMenuPrinc.numVidas=valI[0];

	//Sonido
	fread(valI, sizeof(int), 1, F);
	infGame.opMenuPrinc.sonido=valI[0];

	fclose(F);
}

void manejaEventosMain()
{
	static int estadoTeclaUp=0;
	static int estadoTeclaDown=0;
	static int estadoTeclaLeft=0;
	static int estadoTeclaRight=0;
	static int estadoTeclaEnter=0;

	static int menuPrincipalActivo=1;
	static int menuOpcionesActivo=1;

	if(infGame.estadoJuego == 5) //Men� principal
	{
		if(controlFunc.obtieneEstadoTecla(0) == 1) //Up
		{
			if(estadoTeclaUp == 0)
			{
				if(infGame.opcionMenuSelec > 0)
					infGame.opcionMenuSelec--;

				estadoTeclaUp=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(0) == 0) //Up
			estadoTeclaUp=0;

		if(controlFunc.obtieneEstadoTecla(1) == 1) //Down
		{
			if(estadoTeclaDown == 0)
			{
				if(infGame.opcionMenuSelec < 2)
					infGame.opcionMenuSelec++;

				estadoTeclaDown=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(1) == 0) //Down
			estadoTeclaDown=0;

		if(controlFunc.obtieneEstadoTecla(6) == 1) //Enter
		{
			if(menuPrincipalActivo == 1)
			{
				if(infGame.opcionMenuSelec == 0)
				{
					infGame.estadoJuego=9;
					if(infGame.opMenuPrinc.sonido == 1)
					{
						sonidoF[2].pausaSonido(true);
						sonidoF[0].volumenSonido(0.5f);
						sonidoF[0].pausaSonido(false);
					}
				}
				else if(infGame.opcionMenuSelec == 1)
				{
					cargaJuego();
				}
				else if(infGame.opcionMenuSelec == 2)
				{
					infGame.estadoJuego=8;
					menuOpcionesActivo=0;
					infGame.opMenuPrinc.opcionSelec=0;
				}
			}
		}
		else if(controlFunc.obtieneEstadoTecla(6) == 0) //Enter
		{
			if(menuPrincipalActivo == 0)
				menuPrincipalActivo=1;
		}
	}
	if(infGame.estadoJuego == 8) //Men� Opciones principales
	{
		if(controlFunc.obtieneEstadoTecla(0) == 1) //Up
		{
			if(estadoTeclaUp == 0)
			{
				if(infGame.opMenuPrinc.opcionSelec > 0)
					infGame.opMenuPrinc.opcionSelec--;

				estadoTeclaUp=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(0) == 0) //Up
			estadoTeclaUp=0;

		if(controlFunc.obtieneEstadoTecla(1) == 1) //Down
		{
			if(estadoTeclaDown == 0)
			{
				if(infGame.opMenuPrinc.opcionSelec < 3)
					infGame.opMenuPrinc.opcionSelec++;

				estadoTeclaDown=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(1) == 0) //Down
			estadoTeclaDown=0;

		if(controlFunc.obtieneEstadoTecla(2) == 1) //Left
		{
			if(estadoTeclaLeft == 0)
			{
				if(infGame.opMenuPrinc.opcionSelec == 0) //dificultad
				{
					if(infGame.opMenuPrinc.dificultad > 0)
						infGame.opMenuPrinc.dificultad--;
				}
				else if(infGame.opMenuPrinc.opcionSelec == 1) //vidas
				{
					if(infGame.opMenuPrinc.numVidas == 7)
						infGame.opMenuPrinc.numVidas=5;
					else if(infGame.opMenuPrinc.numVidas == 5)
						infGame.opMenuPrinc.numVidas=3;
				}
				else if(infGame.opMenuPrinc.opcionSelec == 2) //sonido
					infGame.opMenuPrinc.sonido=0;
					
				estadoTeclaLeft=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(2) == 0) //Up
			estadoTeclaLeft=0;

		if(controlFunc.obtieneEstadoTecla(3) == 1) //Right
		{
			if(estadoTeclaRight == 0)
			{
				if(infGame.opMenuPrinc.opcionSelec == 0) //dificultad
				{
					if(infGame.opMenuPrinc.dificultad < 2)
						infGame.opMenuPrinc.dificultad++;
				}
				else if(infGame.opMenuPrinc.opcionSelec == 1) //vidas
				{
					if(infGame.opMenuPrinc.numVidas == 3)
						infGame.opMenuPrinc.numVidas=5;
					else if(infGame.opMenuPrinc.numVidas == 5)
						infGame.opMenuPrinc.numVidas=7;
				}
				else if(infGame.opMenuPrinc.opcionSelec == 2) //sonido
					infGame.opMenuPrinc.sonido=1;

				estadoTeclaRight=1;
			}
		}
		else if(controlFunc.obtieneEstadoTecla(3) == 0) //!Right
			estadoTeclaRight=0;

		if(controlFunc.obtieneEstadoTecla(6) == 1) //Enter
		{
			if(menuOpcionesActivo == 1)
			{
				if(infGame.opMenuPrinc.opcionSelec == 3)
				{
					infGame.estadoJuego=5;
					menuPrincipalActivo=0;
					guardaConfiguracion();

					if(infGame.opMenuPrinc.sonido == 0)
					{
						sonidoF[0].pausaSonido(true);
						sonidoF[2].pausaSonido(true);
					}
				}
			}
		}
		else if(controlFunc.obtieneEstadoTecla(6) == 0) //Enter
		{
			if(menuOpcionesActivo == 0)
				menuOpcionesActivo=1;
		}
	}
	else if(infGame.estadoJuego == 6)
	{
		if(infGame.pausa == 0)
		{
			if(controlFunc.obtieneEstadoTecla(0) == 1) //Up
			{
				if(player1.kick==false && player1.saltando == false)
				{
					//ControlPersonaje2(3);
					player1.caminando=true;
					player1.dirZ=2;
					player1.AngObj=180.0f;
					
					if(fabs(player1.PosicionObj.y - altPiso) < 0.01f)
					{
						if(play==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
						{
							//Se le asignan a las variables del personaje los 
							//valores almacenados en el primer keyframe para que
							//inicie desde ah� la animaci�n.
							Angt1   = KeyFrame1[0].Angt1;
							Angt2   = KeyFrame1[0].Angt2;
							Angc1   = KeyFrame1[0].Angc1;
							Angc2   = KeyFrame1[0].Angc2;
							Angbi1  = KeyFrame1[0].Angbi1;
							Angbi2  = KeyFrame1[0].Angbi2;
							Angbib  = KeyFrame1[0].Angbib;
							Angbd1  = KeyFrame1[0].Angbd1;
							Angbd2  = KeyFrame1[0].Angbd2;
							Angbdb  = KeyFrame1[0].Angbdb;
							Angpizq = KeyFrame1[0].Angpizq;
							Angpder = KeyFrame1[0].Angpder;
							Angpi   = KeyFrame1[0].Angpi;
							Angpd   = KeyFrame1[0].Angpd;
							Xtor    = KeyFrame1[0].Xtor;
							Ytor    = KeyFrame1[0].Ytor;
							Ztor    = KeyFrame1[0].Ztor;

							play=true;
							playIndex=0;
							tipoAnim=1;
						}
					}
				}
			}

			if(controlFunc.obtieneEstadoTecla(1) == 1) //Down
			{
				if(player1.kick==false && player1.saltando == false)
				{
					//ControlPersonaje2(4);
					player1.caminando=true;
					player1.dirZ=1;
					player1.AngObj=0.0f;
					
					if(fabs(player1.PosicionObj.y - altPiso) < 0.01f)
					{
						if(play==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
						{
							//Se le asignan a las variables del personaje los 
							//valores almacenados en el primer keyframe para que
							//inicie desde ah� la animaci�n.
							Angt1   = KeyFrame1[0].Angt1;
							Angt2   = KeyFrame1[0].Angt2;
							Angc1   = KeyFrame1[0].Angc1;
							Angc2   = KeyFrame1[0].Angc2;
							Angbi1  = KeyFrame1[0].Angbi1;
							Angbi2  = KeyFrame1[0].Angbi2;
							Angbib  = KeyFrame1[0].Angbib;
							Angbd1  = KeyFrame1[0].Angbd1;
							Angbd2  = KeyFrame1[0].Angbd2;
							Angbdb  = KeyFrame1[0].Angbdb;
							Angpizq = KeyFrame1[0].Angpizq;
							Angpder = KeyFrame1[0].Angpder;
							Angpi   = KeyFrame1[0].Angpi;
							Angpd   = KeyFrame1[0].Angpd;
							Xtor    = KeyFrame1[0].Xtor;
							Ytor    = KeyFrame1[0].Ytor;
							Ztor    = KeyFrame1[0].Ztor;

							play=true;
							playIndex=0;
							tipoAnim=1;
						}
					}
				}
			}

			if(controlFunc.obtieneEstadoTecla(2) == 1) //Left
			{
				if(player1.kick==false && player1.saltando == false)
				{
					//ControlPersonaje2(2);
					player1.caminando=true;
					player1.dirX=2;
					player1.AngObj=-90.0f;
				
					if(fabs(player1.PosicionObj.y - altPiso) < 0.01f)
					{
						if(play==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
						{
							//Se le asignan a las variables del personaje los 
							//valores almacenados en el primer keyframe para que
							//inicie desde ah� la animaci�n.
							Angt1   = KeyFrame1[0].Angt1;
							Angt2   = KeyFrame1[0].Angt2;
							Angc1   = KeyFrame1[0].Angc1;
							Angc2   = KeyFrame1[0].Angc2;
							Angbi1  = KeyFrame1[0].Angbi1;
							Angbi2  = KeyFrame1[0].Angbi2;
							Angbib  = KeyFrame1[0].Angbib;
							Angbd1  = KeyFrame1[0].Angbd1;
							Angbd2  = KeyFrame1[0].Angbd2;
							Angbdb  = KeyFrame1[0].Angbdb;
							Angpizq = KeyFrame1[0].Angpizq;
							Angpder = KeyFrame1[0].Angpder;
							Angpi   = KeyFrame1[0].Angpi;
							Angpd   = KeyFrame1[0].Angpd;
							Xtor    = KeyFrame1[0].Xtor;
							Ytor    = KeyFrame1[0].Ytor;
							Ztor    = KeyFrame1[0].Ztor;

							play=true;
							playIndex=0;
							tipoAnim=1;
						}
					}
				}
			}

			if(controlFunc.obtieneEstadoTecla(3) == 1) //Right
			{
				if(player1.kick==false && player1.saltando == false)
				{
					//ControlPersonaje2(1);
					player1.caminando=true;
					player1.dirX=1;
					player1.AngObj=90.0f;
					
					if(fabs(player1.PosicionObj.y - altPiso) < 0.01f)
					{
						if(play==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
						{
							//Se le asignan a las variables del personaje los 
							//valores almacenados en el primer keyframe para que
							//inicie desde ah� la animaci�n.
							Angt1   = KeyFrame1[0].Angt1;
							Angt2   = KeyFrame1[0].Angt2;
							Angc1   = KeyFrame1[0].Angc1;
							Angc2   = KeyFrame1[0].Angc2;
							Angbi1  = KeyFrame1[0].Angbi1;
							Angbi2  = KeyFrame1[0].Angbi2;
							Angbib  = KeyFrame1[0].Angbib;
							Angbd1  = KeyFrame1[0].Angbd1;
							Angbd2  = KeyFrame1[0].Angbd2;
							Angbdb  = KeyFrame1[0].Angbdb;
							Angpizq = KeyFrame1[0].Angpizq;
							Angpder = KeyFrame1[0].Angpder;
							Angpi   = KeyFrame1[0].Angpi;
							Angpd   = KeyFrame1[0].Angpd;
							Xtor    = KeyFrame1[0].Xtor;
							Ytor    = KeyFrame1[0].Ytor;
							Ztor    = KeyFrame1[0].Ztor;

							play=true;
							playIndex=0;
							tipoAnim=1;
						}
					}
				}
			}

			if(!(controlFunc.obtieneEstadoTecla(0) == 1 || controlFunc.obtieneEstadoTecla(1) == 1))
				player1.dirZ=0;

			if(!(controlFunc.obtieneEstadoTecla(2) == 1 || controlFunc.obtieneEstadoTecla(3) == 1))
				player1.dirX=0;

			//Si no se presiona ninguna tecla de direccion
			if(!(controlFunc.obtieneEstadoTecla(0) == 1 || controlFunc.obtieneEstadoTecla(1) == 1
				|| controlFunc.obtieneEstadoTecla(2) == 1 || controlFunc.obtieneEstadoTecla(3) == 1))
			{
				//if(tipoAnim == 1)
				if(!player1.kick && !player1.saltando && fabs(player1.PosicionObj.y - altPiso) < 0.01f)
				{
					play=false;
					Angt1   = 0.0f;
					Angt2   = 0.0f;
					Angc1   = 0.0f;
					Angc2   = 0.0f;
					Angbi1  = 0.0f;
					Angbi2  = 0.0f;
					Angbib  = 0.0f;
					Angbd1  = 0.0f;
					Angbd2  = 0.0f;
					Angbdb  = 0.0f;
					Angpizq = 0.0f;
					Angpder = 0.0f;
					Angpi   = 0.0f;
					Angpd   = 0.0f;
					Xtor    = 0.0f;
					Ytor    = 0.0f;
					Ztor    = 0.0f;
				}
			}

			if(controlFunc.obtieneEstadoTecla(4) == 1) //Z
			{
				if(player1.kick == false && player1.saltando == false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
				{
					//Se le asignan a las variables del personaje los 
					//valores almacenados en el primer keyframe para que
					//inicie desde ah� la animaci�n.
					Angt1   = KeyFrame2[0].Angt1;
					Angt2   = KeyFrame2[0].Angt2;
					Angc1   = KeyFrame2[0].Angc1;
					Angc2   = KeyFrame2[0].Angc2;
					Angbi1  = KeyFrame2[0].Angbi1;
					Angbi2  = KeyFrame2[0].Angbi2;
					Angbib  = KeyFrame2[0].Angbib;
					Angbd1  = KeyFrame2[0].Angbd1;
					Angbd2  = KeyFrame2[0].Angbd2;
					Angbdb  = KeyFrame2[0].Angbdb;
					Angpizq = KeyFrame2[0].Angpizq;
					Angpder = KeyFrame2[0].Angpder;
					Angpi   = KeyFrame2[0].Angpi;
					Angpd   = KeyFrame2[0].Angpd;
					Xtor    = KeyFrame2[0].Xtor;
					Ytor    = KeyFrame2[0].Ytor;
					Ztor    = KeyFrame2[0].Ztor;

					play=true;
					playIndex=0;
					tipoAnim=2;

					player1.kick=true;
					player1.contAuxAnim=0;

					if(infGame.opMenuPrinc.sonido == 1)
						sonidoF[1].reproduceSonido();
				}
			}

			if(controlFunc.obtieneEstadoTecla(5) == 1) //X
			{
				if(player1.saltando == false && player1.kick == false
					&& fabs(player1.PosicionObj.y - altPiso) < 0.01f)
				{
					//Se le asignan a las variables del personaje los 
					//valores almacenados en el primer keyframe para que
					//inicie desde ah� la animaci�n.
					Angt1   = KeyFrame3[0].Angt1;
					Angt2   = KeyFrame3[0].Angt2;
					Angc1   = KeyFrame3[0].Angc1;
					Angc2   = KeyFrame3[0].Angc2;
					Angbi1  = KeyFrame3[0].Angbi1;
					Angbi2  = KeyFrame3[0].Angbi2;
					Angbib  = KeyFrame3[0].Angbib;
					Angbd1  = KeyFrame3[0].Angbd1;
					Angbd2  = KeyFrame3[0].Angbd2;
					Angbdb  = KeyFrame3[0].Angbdb;
					Angpizq = KeyFrame3[0].Angpizq;
					Angpder = KeyFrame3[0].Angpder;
					Angpi   = KeyFrame3[0].Angpi;
					Angpd   = KeyFrame3[0].Angpd;
					Xtor    = KeyFrame3[0].Xtor;
					Ytor    = KeyFrame3[0].Ytor;
					Ztor    = KeyFrame3[0].Ztor;

					play=true;
					playIndex=0;
					tipoAnim=3;

					player1.saltando=true;
					player1.cayendo=false;
				}
			}
		}
		else if(infGame.pausa == 1)
		{
			if(controlFunc.obtieneEstadoTecla(0) == 1) //Up
			{
				if(estadoTeclaUp == 0)
				{
					if(infGame.opcionMenuPausa > 0)
						infGame.opcionMenuPausa--;

					estadoTeclaUp=1;
				}
			}
			else if(controlFunc.obtieneEstadoTecla(0) == 0) //Up
				estadoTeclaUp=0;

			if(controlFunc.obtieneEstadoTecla(1) == 1) //Down
			{
				if(estadoTeclaDown == 0)
				{
					if(infGame.opcionMenuPausa < 2)
						infGame.opcionMenuPausa++;

					estadoTeclaDown=1;
				}
			}
			else if(controlFunc.obtieneEstadoTecla(1) == 0) //Down
				estadoTeclaDown=0;

			if(controlFunc.obtieneEstadoTecla(6) == 1) //Enter
			{
				if(estadoTeclaEnter == 0)
				{
					if(infGame.opcionMenuPausa == 0)
						guardaJuego();
					else if(infGame.opcionMenuPausa == 1)
					{
						infGame.estadoJuego=5;
						menuPrincipalActivo=0;
					}
					else if(infGame.opcionMenuPausa == 2)
						done=TRUE;						// ESC indica el termino del programa

					estadoTeclaEnter=1;
				}
			}
			else if(controlFunc.obtieneEstadoTecla(6) == 0) //Enter
				estadoTeclaEnter=0;
		}
	}
}

//int ManejaTeclado()
//{
//	//Al presionar cualquiera de las teclas de direcci�n se iniciar� la animaci�n de caminar
//	//Al soltar la tecla presionada se detiene la animaci�n.
//	if(GetKeyState(VK_UP) & 0x80) //Si est� presionada la tecla UP
//	{
//		if(player1.kick==false)
//		{
//			ControlPersonaje2(3);
//			
//			if(play==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
//			{
//				//Se le asignan a las variables del personaje los 
//				//valores almacenados en el primer keyframe para que
//				//inicie desde ah� la animaci�n.
//				Angt1   = KeyFrame1[0].Angt1;
//				Angt2   = KeyFrame1[0].Angt2;
//				Angc1   = KeyFrame1[0].Angc1;
//				Angc2   = KeyFrame1[0].Angc2;
//				Angbi1  = KeyFrame1[0].Angbi1;
//				Angbi2  = KeyFrame1[0].Angbi2;
//				Angbib  = KeyFrame1[0].Angbib;
//				Angbd1  = KeyFrame1[0].Angbd1;
//				Angbd2  = KeyFrame1[0].Angbd2;
//				Angbdb  = KeyFrame1[0].Angbdb;
//				Angpizq = KeyFrame1[0].Angpizq;
//				Angpder = KeyFrame1[0].Angpder;
//				Angpi   = KeyFrame1[0].Angpi;
//				Angpd   = KeyFrame1[0].Angpd;
//				Xtor    = KeyFrame1[0].Xtor;
//				Ytor    = KeyFrame1[0].Ytor;
//				Ztor    = KeyFrame1[0].Ztor;
//
//				play=true;
//				playIndex=0;
//				tipoAnim=1;
//			}
//		}
//	}
//
//	if(GetKeyState(VK_DOWN) & 0x80) //Si est� presionada la tecla DOWN
//	{
//		if(player1.kick==false)
//		{
//			ControlPersonaje2(4);
//			
//			if(play==false && player1.kick==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
//			{
//				//Se le asignan a las variables del personaje los 
//				//valores almacenados en el primer keyframe para que
//				//inicie desde ah� la animaci�n.
//				Angt1   = KeyFrame1[0].Angt1;
//				Angt2   = KeyFrame1[0].Angt2;
//				Angc1   = KeyFrame1[0].Angc1;
//				Angc2   = KeyFrame1[0].Angc2;
//				Angbi1  = KeyFrame1[0].Angbi1;
//				Angbi2  = KeyFrame1[0].Angbi2;
//				Angbib  = KeyFrame1[0].Angbib;
//				Angbd1  = KeyFrame1[0].Angbd1;
//				Angbd2  = KeyFrame1[0].Angbd2;
//				Angbdb  = KeyFrame1[0].Angbdb;
//				Angpizq = KeyFrame1[0].Angpizq;
//				Angpder = KeyFrame1[0].Angpder;
//				Angpi   = KeyFrame1[0].Angpi;
//				Angpd   = KeyFrame1[0].Angpd;
//				Xtor    = KeyFrame1[0].Xtor;
//				Ytor    = KeyFrame1[0].Ytor;
//				Ztor    = KeyFrame1[0].Ztor;
//
//				play=true;
//				playIndex=0;
//				tipoAnim=1;
//			}
//		}
//	}
//
//	if(GetKeyState(VK_LEFT) & 0x80) //Si est� presionada la tecla LEFT
//	{
//		if(player1.kick==false)
//		{
//			ControlPersonaje2(2);
//		
//			if(play==false && player1.kick==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
//			{
//				//Se le asignan a las variables del personaje los 
//				//valores almacenados en el primer keyframe para que
//				//inicie desde ah� la animaci�n.
//				Angt1   = KeyFrame1[0].Angt1;
//				Angt2   = KeyFrame1[0].Angt2;
//				Angc1   = KeyFrame1[0].Angc1;
//				Angc2   = KeyFrame1[0].Angc2;
//				Angbi1  = KeyFrame1[0].Angbi1;
//				Angbi2  = KeyFrame1[0].Angbi2;
//				Angbib  = KeyFrame1[0].Angbib;
//				Angbd1  = KeyFrame1[0].Angbd1;
//				Angbd2  = KeyFrame1[0].Angbd2;
//				Angbdb  = KeyFrame1[0].Angbdb;
//				Angpizq = KeyFrame1[0].Angpizq;
//				Angpder = KeyFrame1[0].Angpder;
//				Angpi   = KeyFrame1[0].Angpi;
//				Angpd   = KeyFrame1[0].Angpd;
//				Xtor    = KeyFrame1[0].Xtor;
//				Ytor    = KeyFrame1[0].Ytor;
//				Ztor    = KeyFrame1[0].Ztor;
//
//				play=true;
//				playIndex=0;
//				tipoAnim=1;
//			}
//		}
//	}
//
//	if(GetKeyState(VK_RIGHT) & 0x80) //Si est� presionada la tecla RIGHT
//	{
//		if(player1.kick==false)
//		{
//			ControlPersonaje2(1);
//			
//			if(play==false && player1.kick==false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
//			{
//				//Se le asignan a las variables del personaje los 
//				//valores almacenados en el primer keyframe para que
//				//inicie desde ah� la animaci�n.
//				Angt1   = KeyFrame1[0].Angt1;
//				Angt2   = KeyFrame1[0].Angt2;
//				Angc1   = KeyFrame1[0].Angc1;
//				Angc2   = KeyFrame1[0].Angc2;
//				Angbi1  = KeyFrame1[0].Angbi1;
//				Angbi2  = KeyFrame1[0].Angbi2;
//				Angbib  = KeyFrame1[0].Angbib;
//				Angbd1  = KeyFrame1[0].Angbd1;
//				Angbd2  = KeyFrame1[0].Angbd2;
//				Angbdb  = KeyFrame1[0].Angbdb;
//				Angpizq = KeyFrame1[0].Angpizq;
//				Angpder = KeyFrame1[0].Angpder;
//				Angpi   = KeyFrame1[0].Angpi;
//				Angpd   = KeyFrame1[0].Angpd;
//				Xtor    = KeyFrame1[0].Xtor;
//				Ytor    = KeyFrame1[0].Ytor;
//				Ztor    = KeyFrame1[0].Ztor;
//
//				play=true;
//				playIndex=0;
//				tipoAnim=1;
//			}
//		}
//	}
//
//	if((GetAsyncKeyState('Z')&1) == 1)
//	{
//		if(player1.kick == false) //Para que la asignaci�n de valores siguiente solo se haga una vez y empiece la animaci�n
//		{
//			//Se le asignan a las variables del personaje los 
//			//valores almacenados en el primer keyframe para que
//			//inicie desde ah� la animaci�n.
//			Angt1   = KeyFrame2[0].Angt1;
//			Angt2   = KeyFrame2[0].Angt2;
//			Angc1   = KeyFrame2[0].Angc1;
//			Angc2   = KeyFrame2[0].Angc2;
//			Angbi1  = KeyFrame2[0].Angbi1;
//			Angbi2  = KeyFrame2[0].Angbi2;
//			Angbib  = KeyFrame2[0].Angbib;
//			Angbd1  = KeyFrame2[0].Angbd1;
//			Angbd2  = KeyFrame2[0].Angbd2;
//			Angbdb  = KeyFrame2[0].Angbdb;
//			Angpizq = KeyFrame2[0].Angpizq;
//			Angpder = KeyFrame2[0].Angpder;
//			Angpi   = KeyFrame2[0].Angpi;
//			Angpd   = KeyFrame2[0].Angpd;
//			Xtor    = KeyFrame2[0].Xtor;
//			Ytor    = KeyFrame2[0].Ytor;
//			Ztor    = KeyFrame2[0].Ztor;
//
//			play=true;
//			playIndex=0;
//			tipoAnim=2;
//
//			player1.kick=true;
//			player1.contAuxAnim=0;
//		}
//	}
//
//	//Para que al soltar la tecla presionada el personaje no quede en una posici�n
//	//intermedia de la animaci�n se asignan los valores originales a las variables
//	if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
//	|| GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80)) //Si no est� presionada alguna de esas teclas
//	{
//		if(tipoAnim == 1)
//		{
//			play=false;
//			Angt1   = 0.0f;
//			Angt2   = 0.0f;
//			Angc1   = 0.0f;
//			Angc2   = 0.0f;
//			Angbi1  = 0.0f;
//			Angbi2  = 0.0f;
//			Angbib  = 0.0f;
//			Angbd1  = 0.0f;
//			Angbd2  = 0.0f;
//			Angbdb  = 0.0f;
//			Angpizq = 0.0f;
//			Angpder = 0.0f;
//			Angpi   = 0.0f;
//			Angpd   = 0.0f;
//			Xtor    = 0.0f;
//			Ytor    = 0.0f;
//			Ztor    = 0.0f;
//		}
//	}
//
//	if(keys[VK_HOME])
//	{
//		ControlPersonaje(5);
//	}
//
//	if(keys[VK_END])
//	{
//		ControlPersonaje(6);
//	}
//
//	if(keys['A'])
//	{
//		
//	}
//	if(keys['S'])
//	{
//		
//	}
//	
//
//	return TRUE;
//}