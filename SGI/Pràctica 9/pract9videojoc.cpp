#define PROYECTO "Videojoc"
#define _CRT_SECURE_NO_WARNINGS // açò ho pose pq no em deixa gastar la funció localtime sense esta definició
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matemática de C
#include <freeglut.h> // Biblioteca grafica
#include <..\Utilidades.h>
#include <ctime> // per a saber l'hora actual
#include <sstream>
#include <..\IrrKlang\irrKlang.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

static float velocitat = 0, angle = PI / 2, auxAngle = 0;
static float distx, disty, distz, mirax = 0, miraz = 0; // per a la càmera
static float A = 10, T = 100;
GLfloat v0[3], v1[3], v2[3], v3[3]; // necessaris per a dibuixar la carretera
GLint ampleCarr = 4; // amplaria de la carretera

static GLuint centre, distFanals = 20, distMedici = 50; // dist = 25
static float anterior = 4, abansC = 60, primerMedici, seguentMedici = 10; // seguent medici = 5
GLfloat ini, iniciC; // punt on comencen a apareixers els fanals
GLfloat carretDif[] = { 0.8,0.8,0.8,1.0 };
GLfloat carretEspec[] = { 0.3,0.3,0.3,1.0 };
GLfloat fanals[] = { GL_LIGHT2, GL_LIGHT3,GL_LIGHT4,GL_LIGHT5 };
//static enum { SOLIDO, ALAMBRICO, LLUM, BOIRA } mode;

bool ALAMBRICO = false, NIT = false, BOIRA = false, DIFICIL = false, SOLIDARIO = false, VELOCITAT = false, ALTURA = false, CANVIFIGURA = false;
static GLuint asfalt, fonsCircuit, cartell, cel, celFosc, medici, moneda, trenRapid, nauEspacial, cotxe, contenidor, esquerra, dreta; // objectes per a les textures
static GLuint contenidorEsquerra, contenidorDreta;
bool visibleCart;
GLfloat posemision[] = { 0.0,-1.0,0.0 }; //posicion de emision
bool monedaArreplegada = false;
int monedes = 0; // contem les monedes que hem arreplegat
bool esquerraPolsat = false, dretaPolsat = false;

// RELLOTGE NOMES AMB MINUTER I SECUNDER
static GLuint estrela, centreLlista, seg, hores, cercleHores, minLlista, marcaminuts, cercleMinuts, accelerador;
static float asec, tacumulat = 0, radi = 0.08, tacmin = 0, amin, ahores, tachores;
static const float velocidad = 0.05 * 360; // velocitat en la que es mou continuament
static float ang; // Angle en el que anem a rotar l'esfera
static float eyex = 0, eyey = 0, eyez = 7; // posició de la càmera

time_t in;

using namespace irrklang; // per a la musica
ISoundEngine* engine;


// Funcions auxiliars ja desplegades que ens ajuden a dibuixar la carretera
float carrFuncio(float x) {
	return A * sin(x * ((2 * PI) / T));
}

float dcarrFuncio(float x) {
	return ((2 * PI * A) / T) * cos(x * (2 * PI / T));
}

GLfloat xnormal(float derivada, float tzx) {
	return (1 / sqrt(1 + derivada * derivada)) * tzx;
}

GLfloat znormal(float derivada, float tzz) {
	return (1 / sqrt(1 + derivada * derivada)) * tzz;
}

// actualitzarà les monedes que anem agafant i que apareguem pel circuit
void actualitzaMonedes(int segons) {
	// agafem el codi del mètode onTimer (parcialment)
	static int antes = 0;
	int ahora, tiempo;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo = ahora - antes;

	if (tiempo > 1000) {
		monedaArreplegada = false;
		antes = ahora;
	}

	glutTimerFunc(2000, actualitzaMonedes, 1500);
	glutPostRedisplay();
}

void displayRellotge() {
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0); // Situa la camara
	glEnable(GL_DEPTH_TEST);

	// per al cilindre de la velocitat
	glPushMatrix();
	glTranslatef(-3, -1, 0);

	if (velocitat < 3) {
		glScalef(1, 1.35768, 1);
	}
	else glScalef(1, (1.2 * (log(velocitat))), 1);

	//std::cout << (1.2 * (log(velocitat))) << std::endl; // Mensaje por consola
	// canviem de color si va molt ràpid
	// roig fosquet
	if (velocitat > 5) glColor3f(0.831, 0.141, 0.109);
	// verd fosquet
	else {
		//glColor3f(0.196, 0.588, 0.172);
		glColor3f(0, 0, 0);
	}
	
	//glTranslatef(-3, -1, 0);
	glCallList(accelerador);
	glPopMatrix();

	if (!NIT) {
		glScalef(0.6, 0.6, 0.6);
		glTranslatef(6.8, 2.8, 0);

		// cridem a totes les llistes per a dibuixar les figures
		glPushMatrix();
		glCallList(centreLlista);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0, 0, 1);
		glRotatef(-asec, 0, 0, 1);
		glCallList(seg);
		glPopMatrix();

		glPushMatrix();
		glRotatef(-amin, 0, 0, 1);
		glCallList(minLlista);
		glPopMatrix();

		glPushMatrix();
		//glRotatef(ang, 1, 0, 0);
		glCallList(cercleHores);
		glPopMatrix();

		glPushMatrix();
		//glRotatef(-ang, 0, 1, 0);
		glCallList(cercleMinuts);
		glPopMatrix();
	}
}

void solidario() {
	//glLoadIdentity();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	displayRellotge();
}

void cotxeDibuix() {
	// Habilitamos blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Z-Buffer Readonly
	glDepthMask(GL_FALSE);
	//glDisable(GL_BLEND);
	glPushMatrix();
	// posem una imatge o altra depenent si estem en velocitat alta o no. 
	// A mes, tenim en campte si estem girant per a canviar la imatge a la inclinada
	if (CANVIFIGURA) {
		if (esquerraPolsat) glBindTexture(GL_TEXTURE_2D, contenidorEsquerra);
		else if (dretaPolsat) glBindTexture(GL_TEXTURE_2D, contenidorDreta);
		else glBindTexture(GL_TEXTURE_2D, contenidor);
	}
	else {
		if (esquerraPolsat) glBindTexture(GL_TEXTURE_2D, esquerra);
		else if (dretaPolsat) glBindTexture(GL_TEXTURE_2D, dreta);
		else glBindTexture(GL_TEXTURE_2D, cotxe);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor3f(0, 0, 0);
	// coordenades per als quads del vehicle
	GLfloat m0[3] = { 1 ,1,-2 };
	GLfloat m1[3] = { 1, 1,+2 };
	GLfloat m2[3] = { 1, 3,+2 };
	GLfloat m3[3] = { 1, 3,-2 };

	//glTranslatef(distx+2, 0.2, distz);
	glTranslatef(distx + sin(angle) * 2, 0.2, distz + cos(-angle) * 2);
	glScalef(1, 0.3, 0.15);
	//std::cout << auxAngle << std::endl; // Mensaje por consola
	//glRotatef(-(angle*(180/PI)),0,1,0);
	//glRotatef(-auxAngle, 0, 1, 0);
	quadtex(m0, m1, m2, m3, 0, 1, 0, 1, 50, 50);
	glPopMatrix();
	// Z-Buffer a estado normal
	glDepthMask(GL_TRUE);

}
// funcio auxiliar que carrega les textures
void textures()
{
	glGenTextures(1, &asfalt);
	glBindTexture(GL_TEXTURE_2D, asfalt);
	loadImageFile((char*)"asfalt2.jpg"); // posava asfalt

	glGenTextures(1, &fonsCircuit);
	glBindTexture(GL_TEXTURE_2D, fonsCircuit);
	loadImageFile((char*)"grass.jpg");

	glGenTextures(1, &cartell);
	glBindTexture(GL_TEXTURE_2D, cartell);
	loadImageFile((char*)"blackFriday.png");

	glGenTextures(1, &cel);
	glBindTexture(GL_TEXTURE_2D, cel);
	loadImageFile((char*)"cel.jpg");

	glGenTextures(1, &celFosc);
	glBindTexture(GL_TEXTURE_2D, celFosc);
	loadImageFile((char*)"celFosc.jpg");

	glGenTextures(1, &medici);
	glBindTexture(GL_TEXTURE_2D, medici);
	loadImageFile((char*)"medici.jpg");

	glGenTextures(1, &nauEspacial);
	glBindTexture(GL_TEXTURE_2D, nauEspacial);
	loadImageFile((char*)"nasa.jpg");

	glGenTextures(1, &trenRapid);
	glBindTexture(GL_TEXTURE_2D, trenRapid);
	loadImageFile((char*)"trenRapid.jpg");

	glGenTextures(1, &moneda);
	glBindTexture(GL_TEXTURE_2D, moneda);
	loadImageFile((char*)"moneda.jpg");

	glGenTextures(1, &cotxe);
	glBindTexture(GL_TEXTURE_2D, cotxe);
	loadImageFile((char*)"cotxeT.png");

	glGenTextures(1, &esquerra);
	glBindTexture(GL_TEXTURE_2D, esquerra);
	loadImageFile((char*)"girantEsquerraT.png");

	glGenTextures(1, &dreta);
	glBindTexture(GL_TEXTURE_2D, dreta);
	loadImageFile((char*)"girantDretaT.png");

	glGenTextures(1, &contenidor);
	glBindTexture(GL_TEXTURE_2D, contenidor);
	loadImageFile((char*)"contenidorT.png");

	glGenTextures(1, &contenidorEsquerra);
	glBindTexture(GL_TEXTURE_2D, contenidorEsquerra);
	loadImageFile((char*)"contenidorEsquerraT.png");

	glGenTextures(1, &contenidorDreta);
	glBindTexture(GL_TEXTURE_2D, contenidorDreta);
	loadImageFile((char*)"contenidorDretaT.png");

}

void initRellotge() {
	ang = 0;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST); // Buffer actiu

	centreLlista = glGenLists(1);
	glNewList(centreLlista, GL_COMPILE);
	glPushMatrix();
	//std::cout << PROYECTO << radi << std::endl; // Mensaje por consola
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(radi, 100, 100); // esta esfera es la que vull moure continuament
	glPopMatrix();
	glEndList();

	// busqueta dels segons
	seg = glGenLists(1);
	glNewList(seg, GL_COMPILE);
	glPushMatrix();
	glColor3f(0.372, 0.294, 0.545);
	//ix cap a afora wtf sens el rotate
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.01, 0.78, 100, 100);
	glPopMatrix();
	glEndList();

	// agulla dels minuts
	minLlista = glGenLists(1);
	glNewList(minLlista, GL_COMPILE);
	glPushMatrix();
	glColor3f(0.337, 0.658, 0.796);
	//ix cap a afora wtf sens el rotate
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.02, 0.6, 100, 100);
	glPopMatrix();
	glEndList();

	// copiem el codi tal qual de la pràctica 2
	estrela = glGenLists(1);
	glNewList(estrela, GL_COMPILE);
	glColor3f(0, 0, 0);
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(0 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(0 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(0 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(0 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(2 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(2 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(2 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(2 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(4 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(4 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(4 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(4 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(0 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(0 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(0 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(0 * (2 * PI / 6) + (PI / 2)), 0);
	glEnd();

	// segona part de l'estrela
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(1 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(1 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(1 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(1 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(3 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(3 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(3 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(3 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(5 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(5 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(5 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(5 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(1.0 * cos(1 * (2 * PI / 6) + (PI / 2)), 1.0 * sin(1 * (2 * PI / 6) + (PI / 2)), 0);
	glVertex3f(0.7 * cos(1 * (2 * PI / 6) + (PI / 2)), 0.7 * sin(1 * (2 * PI / 6) + (PI / 2)), 0);
	glEnd();
	glPopMatrix();
	glEndList();

	// anem a marcar les hores
	hores = glGenLists(1);
	glNewList(hores, GL_COMPILE);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.8, 0.0);
	//queda raro sense el rotate
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.035, 0.2, 100, 100);
	glPopMatrix();
	glEndList();

	//ara posem ixa marca de l'hora en el rellotge en si
	cercleHores = glGenLists(1);
	glNewList(cercleHores, GL_COMPILE);
	int a;
	for (int i = 0; i < 12; i++) {
		a = i * (360 / 12);
		// en les hores 12, 3, 6 i 9 posem una estrela de David
		if ((a % 90) == 0) {
			glPushMatrix();
			glRotatef(a, 0, 0, 1);
			glTranslatef(0.0, 0.895, 0.0); // posava 0.895
			glScalef(0.12, 0.12, 0.12);
			glCallList(estrela);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glRotatef(a, 0, 0, 1);
			glCallList(hores);
			glPopMatrix();
		}
	}
	glEndList();

	// Posem les marques per als minuts
	marcaminuts = glGenLists(1);
	glNewList(marcaminuts, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0.0, 1, 0.0); // estava a 1.05
	glRotatef(90, 1, 0, 0);
	glutSolidCone(0.02, 0.1, 100, 100);
	glPopMatrix();
	glEndList();

	// tornem a posar les marques dels minuts igual que haviem fet abans amb les hores
	cercleMinuts = glGenLists(1);
	glNewList(cercleMinuts, GL_COMPILE);
	for (int i = 0; i < 60; i++) {
		a = i * (360 / 60);
		// no vull que aparega la marca dels minuts si ja està la de les hores!
		if ((i % 5) != 0) {
			glPushMatrix();
			glColor3f(0, 0, 0);
			glRotatef(a, 0, 0, 1);
			glCallList(marcaminuts);
			glPopMatrix();
		}
	}
	glEndList();

	accelerador = glGenLists(1);
	glNewList(accelerador, GL_COMPILE);
	glPushMatrix();

	// el logaritme ens dona problemssss si la v es <1
	glRotatef(270, 1, 0, 0); // posava 270

	if (ALAMBRICO) glutWireCylinder(0.2, 0.5, 10, 10);
	else glutSolidCylinder(0.2, 0.5, 10, 10);
	glPopMatrix();
	glEndList();

	// ens guardem l'hora d'inici de joc
	in = time(0);
}

void init()
// Funcion de inicializacion propia
{
	// Reproduim la musica

	engine = createIrrKlangDevice();
	engine->play2D("./Confusion.mp3", true); // true per a que reproduisca en bucle

	// per a la camera
	distx = 0; // posava 0
	disty = 1; // posava 1
	distz = 0; // posava 0

	glClearColor(0.0, 0.0, 0.0, 1.0);
	//visibleCart = true; No util, ho teniem per als cartells abans

	glEnable(GL_DEPTH_TEST); //Buffer actiu
	glShadeModel(GL_SMOOTH); // Per a les ombres
	//activem iluminacio
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);

	//Llum direccional de la lluna
	GLfloat LunaAmbienteDifusa[] = { 0.05,0.05,0.05,1.0 };
	GLfloat LunaEspecular[] = { 0.0,0.0,0.0,1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, LunaAmbienteDifusa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LunaAmbienteDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LunaEspecular);

	// Llum del focus del vehicle
	GLfloat farAmbiente[] = { 0.2,0.2,0.2,1.0 };
	GLfloat farDifusa[] = { 1.0,1.0,1.0,1.0 };
	GLfloat farEspecular[] = { 0.3,0.3,0.3,1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, farAmbiente);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, farDifusa);
	glLightfv(GL_LIGHT1, GL_SPECULAR, farEspecular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0); // posava 25
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);

	// FANALS
	GLfloat fanalAmbient[] = { 0.0,0.0,0.0,1.0 };
	GLfloat fanalDifusa[] = { 0.5,0.5,0.2,1.0 };
	GLfloat fanalEspecular[] = { 0.0,0.0,0.0,1.0 };

	// Com que tenim 4 fanals, hem d'anar activant un a un les seues llums

	glLightfv(GL_LIGHT2, GL_AMBIENT, fanalAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, fanalDifusa);
	glLightfv(GL_LIGHT2, GL_SPECULAR, fanalEspecular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT3, GL_AMBIENT, fanalAmbient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, fanalDifusa);
	glLightfv(GL_LIGHT3, GL_SPECULAR, fanalEspecular);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT4, GL_AMBIENT, fanalAmbient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, fanalDifusa);
	glLightfv(GL_LIGHT4, GL_SPECULAR, fanalEspecular);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);

	glLightfv(GL_LIGHT5, GL_AMBIENT, fanalAmbient);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, fanalDifusa);
	glLightfv(GL_LIGHT5, GL_SPECULAR, fanalEspecular);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);

	// carreguem les textures
	textures();

	// activem les textures
	glEnable(GL_TEXTURE_2D);

	initRellotge();
}


void display()
// Funcion de atencion al dibujo
{
	//std::cout << " display " << std::endl; // Mensaje por consola
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla y el Z-buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// si el mode altura està activat, augmentem disty
	if (ALTURA) disty = 9;
	else disty = 1;

	gluLookAt(distx, disty, distz, mirax, 1, miraz, 0, 1, 0); // Situa la camara
	glEnable(GL_DEPTH_TEST);

	if (ALAMBRICO) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}
	else {
		// no estem en modo alambrico
		if (NIT == false) {
			glDisable(GL_LIGHTING);
			glClearColor(1.0, 1.0, 1.0, 1.0);
		}
		else {
			glShadeModel(GL_SMOOTH);
			glEnable(GL_LIGHTING);
			glClearColor(0.0, 0.0, 0.0, 0.0);
		}
		glEnable(GL_TEXTURE_2D);
	}

	// si hi ha boira
	if (BOIRA) {
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, BLANCO);
		glFogf(GL_FOG_DENSITY, 0.05);
		//std::cout << "  boira" << std::endl; // Mensaje por consola
	}
	else {
		glDisable(GL_FOG);
		//std::cout << " NO boira" << std::endl; // Mensaje por consola
	}

	// si el mode es el dificil fem una tormenta de arena
	if (DIFICIL) {
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, ORO);
		glFogf(GL_FOG_DENSITY, 0.3); // prou densa per a que siga mes dificil
	}

	//LLum direccional de la lluna!!!!
	GLfloat pLluna[] = { 0.0, 10.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pLluna);

	// Llum del far del vehicle, hem d'anar actualitzant. Ultim component ojo per a diferenciar tipus de llum
	GLfloat posiciollumV[] = { distx, disty - 0.3, distz,1 };
	//std::cout << mirax << "  mirax" << std::endl; // Mensaje por consola
	//std::cout << miraz << "  miraz" << std::endl; // Mensaje por consola
	GLfloat direccioLlumV[] = { mirax, -1.5, miraz };
	glLightfv(GL_LIGHT1, GL_POSITION, posiciollumV);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direccioLlumV);

	if (true == ALAMBRICO) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// per a situar els fanals
	float xfarola;
	float zfarola;

	// Dibuixem els fanals 
	ini = anterior;
	// bucle per a dibuixar els 4 fanals
	for (int i = 0; i < 4; i++) {
		ini += distFanals; // distancia entre cada un
		xfarola = ini;
		zfarola = carrFuncio(xfarola);
		// comprovem si ja l'hem passat o encara no
		if (distx > ini + 2) {
			anterior = ini;
		}

		GLfloat posFanal[] = { xfarola,4,zfarola,1.0 };
		glLightfv(fanals[i], GL_POSITION, posFanal);
		glLightfv(fanals[i], GL_SPOT_DIRECTION, posemision);

		// posem les monedes també, aixi apareixen segur sobre la carretera. Son cilindres amb textura de l'or
		glPushMatrix();
		GLUquadricObj* mon = gluNewQuadric();
		gluQuadricNormals(mon, GLU_SMOOTH);
		gluQuadricTexture(mon, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, moneda);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glTranslatef(xfarola, 0.5, zfarola);
		if (abs(xfarola - distx) < 0.8 && abs(zfarola - distz) < 0.8 && !monedaArreplegada) {
			// Quan arrepleguem una moneda emetem l'efecte de so ixe
			engine->play2D("./Coin Sound Effect.mp3");
			monedes++;
			monedaArreplegada = true;
			cout << "Has agafat una moneda!" << endl;
		}

		glColor3f(0, 0, 0);
		if (ALAMBRICO)gluQuadricDrawStyle(mon, GLU_LINE);
		else gluQuadricDrawStyle(mon, GLU_FILL);

		// dibuixem el cilindre
		gluCylinder(mon, 0.3, 0.3, 0.1, 10, 10);

		glPopMatrix();
	}

	// COL.LOQUEM 2 ANUNCIS

	// mediciTV
	float xmedici; // per al cartell medici
	float zmedici;
	primerMedici = seguentMedici;
	primerMedici += (2 * distMedici);
	// les coordenades les calculem de manera pareguda a com ho feiem amb els fanals
	xmedici = primerMedici;
	zmedici = carrFuncio(xmedici);

	glPushMatrix();
	// posem un cartell o altre depenent si estem en velocitat alta o no
	if (!VELOCITAT) glBindTexture(GL_TEXTURE_2D, medici);
	else glBindTexture(GL_TEXTURE_2D, trenRapid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTranslatef(xmedici, 1, zmedici);
	// coordenades per als quads
	GLfloat m0[3] = { 1 ,2,-6 };
	GLfloat m1[3] = { 1, 2,+6 }; // posava 2 en la z
	GLfloat m2[3] = { 1, 5,+6 };
	GLfloat m3[3] = { 1, 5,-6 };
	quadtex(m0, m1, m2, m3, 0, 1, 0, 1, 50, 50);
	glPopMatrix();
	// COMPROVEM SI ES VISIBLE O QUE EL CARTELL I SI NO, DIBUIXEM UN ALTRE
	if (distx > primerMedici) { // comprovem si ja l'hem passat o encara no
		//std::cout << "no es fuck visible" << std::endl;
		seguentMedici = primerMedici;
	}

	// dibuixem les potes als cartells, que son cilindres
	glPushMatrix();
	glColor3f(0, 0, 0);
	// dimensio del cartell la restes i que quede aliniat i au
	// posem +2 per a que estiguen les potes per darrere del cartell i no el tapen
	glTranslatef(xmedici + 1.5, 3.2, zmedici - 5); // posava 3.75
	glRotatef(90, 1, 0, 0);
	if (ALAMBRICO) {
		glutWireCylinder(0.2, 3, 10, 10);
		//std::cout << "alambrico true" << std::endl;
	}
	else {
		glutSolidCylinder(0.2, 3, 10, 10);
		//std::cout << "solid" << std::endl;
	}
	glPopMatrix();

	// segon cilindre del cartell
	glPushMatrix();
	glColor3f(0, 0, 0);
	// dimensio del cartell la sumes
	glTranslatef(xmedici + 1.5, 3.2, zmedici + 5.2); // posava 3.65
	glRotatef(90, 1, 0, 0);
	if (ALAMBRICO) glutWireCylinder(0.2, 3.2, 10, 10);
	else glutSolidCylinder(0.2, 3.2, 10, 10);
	glPopMatrix();

	// DIBUIXEM EL 2n CARTELLL
	float xcartell; // per a la seua posicio
	float zcartell;
	iniciC = abansC;
	iniciC += (2 * distMedici);
	xcartell = iniciC;
	zcartell = carrFuncio(xcartell);

	glPushMatrix();
	// posem un cartell o altre depenent si estem en velocitat alta o no
	if (!VELOCITAT) glBindTexture(GL_TEXTURE_2D, cartell);
	else glBindTexture(GL_TEXTURE_2D, nauEspacial);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor3f(0, 0, 1);

	glTranslatef(xcartell, 1, zcartell);
	// coordenades per als quads que son les mateixes que per al cartell de medici
	quadtex(m0, m1, m2, m3, 0, 1, 0, 1, 50, 50);

	glPopMatrix();
	// COMPROVEM SI ES VISIBLE O QUE EL CARTELL I SI NO, DIBUIXEM UN ALTRE
	if (distx > iniciC) { // comprovem si ja l'hem passat o encara no
		//std::cout << "no es fuck visible" << std::endl;
		abansC = iniciC;
	}

	// POSEM POTES DEL 2n CARTELL
	// primer cilindre del cartell
	glPushMatrix();
	glColor3f(0, 0, 0);
	// dimensio del cartell la restes i que quede aliniat i au
	glTranslatef(xcartell + 1.5, 3.2, zcartell - 5); // 3.75
	glRotatef(90, 1, 0, 0);
	if (ALAMBRICO) {
		glutWireCylinder(0.2, 3, 10, 10);
		//std::cout << "alambrico true" << std::endl;
	}
	else {
		glutSolidCylinder(0.2, 3, 10, 10);
		//std::cout << "solid" << std::endl;
	}
	glPopMatrix();

	// segon cilindre del cartell
	glPushMatrix();
	glColor3f(0, 0, 0);
	// dimensio del cartell la sumes
	glTranslatef(xcartell + 1.5, 3.2, zcartell + 5); // 3.65
	glRotatef(90, 1, 0, 0);
	if (ALAMBRICO) glutWireCylinder(0.2, 3, 10, 10);
	else glutSolidCylinder(0.2, 3, 10, 10);
	glPopMatrix();


	//Carretera  DINÀMICA dibuix. Fem us de les funcions auxiliars que hem definit al principi
	float ini_carretera = distx, vcarrFuncio = carrFuncio(ini_carretera);
	float derivada = dcarrFuncio(ini_carretera);
	GLfloat precalcul[3] = { ini_carretera,0,vcarrFuncio };
	GLfloat tz[3] = { -derivada,0,1 };

	GLfloat nx = xnormal(derivada, tz[0]);
	GLfloat nz = znormal(derivada, tz[2]);
	GLfloat vnormals[3] = { nx ,0, nz };

	for (int i = 0; i < 3; i++) {
		v0[i] = precalcul[i] - (vnormals[i] * ampleCarr);
		v3[i] = precalcul[i] + (vnormals[i] * ampleCarr);
	}

	for (int i = 1; i < 100; i++) {
		float aux = ini_carretera + i;
		vcarrFuncio = carrFuncio(aux);
		float derivada = dcarrFuncio(aux);
		GLfloat precalcul2[3] = { aux,0,vcarrFuncio };
		GLfloat tz[3] = { -derivada,0,1 };

		GLfloat nx = xnormal(derivada, tz[0]);
		GLfloat nz = znormal(derivada, tz[2]);
		GLfloat vnormals2[3] = { nx ,0, nz };

		for (int i = 0; i < 3; i++) {
			v1[i] = precalcul2[i] - (vnormals2[i] * ampleCarr);
			v2[i] = precalcul2[i] + (vnormals2[i] * ampleCarr);
		}

		// abans de dibuixar el quad apliquem les caracteristiques del material
		glNormal3f(0, 1, 0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, carretDif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, carretEspec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3); // posava 3
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glPushMatrix();

		//textura per a la carretera
		glBindTexture(GL_TEXTURE_2D, asfalt);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glColor3f(1, 0, 0);
		//quad(v3, v0, v1, v2, 10, 5); ara cridem quadtex
		quadtex(v2, v1, v0, v3, 0, 1, 0, 1, 50, 50);
		glPopMatrix();

		// actualitzem!
		for (int i = 0; i < 3; i++) {
			v0[i] = v1[i];
			v3[i] = v2[i];
		}
	}

	// apliquem les textures per als costats del circuit
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, fonsCircuit);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (NIT) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLfloat p0[3] = { 100 + distx, -0.1, -100 - distz };
	GLfloat p1[3] = { 100 + distx, -0.1, 100 - distz };
	GLfloat p2[3] = { -100 - distx, -0.1, 100 + distz };
	GLfloat p4[3] = { -100 - distx, -0.1, -100 - distz };
	quadtex(p0, p1, p2, p4, 0, 10, 0, 10, 10 * 10, 5 * 10);
	glPopMatrix();

	// TEXTURA PER AL CEL DEL FONS: ANEM A FER UN CILINDRE
	glPushMatrix();
	// posem un fons o un altre depenent del mode
	if (NIT) glBindTexture(GL_TEXTURE_2D, celFosc);
	else glBindTexture(GL_TEXTURE_2D, cel);

	float alfa = 2 * PI / 50; //angle
	GLfloat cil0[3] = { 190 * cos(0) + distx, 100, 190 * -sin(0) + distz };
	GLfloat cil1[3] = { 190 * cos(0) + distx,-170, 190 * -sin(0) + distz };
	GLfloat cil2[3];
	GLfloat cil3[3];
	for (int i = 1; i <= 50; i++) {
		// definim vertexs per al cilinder
		cil2[0] = 190 * cos(i * alfa) + distx;
		cil2[1] = 100;
		cil2[2] = 190 * -sin(i * alfa) + distz;
		cil3[0] = 190 * cos(i * alfa) + distx;
		cil3[1] = -170;
		cil3[2] = 190 * -sin(i * alfa) + distz;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		quadtex(cil3, cil1, cil0, cil2, (i) / 50.0 + 0.5, (i - 1) / 50.0 + 0.5, 0, 1);
		// actualitzem valors
		for (int j = 0; j < 3; j++) {
			cil0[j] = cil2[j];
			cil1[j] = cil3[j];
		}
	}
	glPopMatrix();

	// dibuixem el cotxe
	cotxeDibuix();

	/* dibuixem els eixos
	glPushMatrix();
	glScalef(20, 20, 20);
	ejes();
	glPopMatrix(); */

	// dibuixem els objectes solidaris si el mode esta activat
	if (SOLIDARIO) solidario();

	glFlush(); // Finaliza el dibujo
	glutSwapBuffers();

}

void onArrow(int tecla, int x, int y)
{
	switch (tecla) {
		// Si estem en mode velocitat, la velocitat varia mes i girem mes en polsar una tecla
	case GLUT_KEY_UP:
		//Flecha arriba: Aumenta el módulo de la velocidad en 0.1 m/sg
		if (VELOCITAT) velocitat += 0.5;
		else velocitat += 0.1;
		break;
	case GLUT_KEY_DOWN:
		//Disminuye el módulo de la velocidad en 0.1 m/sg
		if (velocitat > 0.1) {
			if (VELOCITAT) velocitat -= 0.5;
			else velocitat -= 0.1;
		}
		else velocitat = 0.0;
		break;
	case GLUT_KEY_LEFT:
		//Flecha izquierda: El vector director de la velocidad gira ¼ de grado respecto al eje Y
		if (angle < 2) {
			angle += 0.25 * PI / 180;
			if (VELOCITAT) angle += 0.25 * PI / 180;
		}
		//std::cout << angle << std::endl; // Mensaje por consola
		esquerraPolsat = true;
		break;
	case GLUT_KEY_RIGHT:
		//Flecha derecha: El vector director de la velocidad gira ¼ de grado respecto al eje Y
		if (angle > 0.218172) {
			angle -= 0.25 * PI / 180;
			if (VELOCITAT) angle -= 0.25 * PI / 180;
		}

		dretaPolsat = true;
		break;
	}
	stringstream title;
	title << "Interfaz de conducción " << velocitat << "m/s. Has recogido " << monedes << " monedas";
	glutSetWindowTitle(title.str().c_str());
	glutPostRedisplay();
}

void onKeyRelease(int tecla, int x, int y)
{
	switch (tecla) {
	case GLUT_KEY_LEFT:
		esquerraPolsat = false;
		//std::cout << "he soltat esq" << std::endl;
		break;
	case GLUT_KEY_RIGHT:
		dretaPolsat = false;
		//std::cout << "he soltat dreta" << std::endl;
		break;
	}
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h);
	float razon = (float)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, razon, 1, 250); // càmera perspectiva
}

void onTimer(int valor) {
	time_t ara = time(0);
	// hem de saber l'hora actual!!!!, ho fem en la biblioteca ctime
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora, tiempo_transcurrido;
	//Tiempo transcurrido desde el ini
	ahora = glutGet(GLUT_ELAPSED_TIME);
	//Tiempo transcurrido desde antes msg
	tiempo_transcurrido = ahora - antes;

	// per al rellotge
	tacumulat += tiempo_transcurrido; // acumulem per a vore si ha passat 1 segon o 1 minut
	tacmin += tiempo_transcurrido;
	tachores += tiempo_transcurrido;

	ang += velocidad * tiempo_transcurrido / 1000.0;

	antes = ahora;

	float segonsPassats = difftime(ara, in);

	float sego = (int)segonsPassats % 60;
	float min = (int)(segonsPassats / 60) % 60;
	//std::cout << sego << std::endl; // Mensaje por consola
	amin = 6 * min;
	asec = 6 * sego;

	float metros = velocitat * (tiempo_transcurrido / 1000.0);
	distx += metros * sin(angle);
	distz += metros * cos(angle);

	mirax = 10 * sin(angle) + distx;
	miraz = 10 * cos(angle) + distz;

	// per al cotxe: son els graus que has girat (no en radians)
	//auxAngle = (180 / PI) * (angle - (PI / 2));
	auxAngle = (180 / PI) * (angle);
	glutTimerFunc(1000 / 60, onTimer, 60); // Se encola un nuevo timer
	//glutTimerFunc(2000, onTimer, 60); // Se encola un nuevo timer
	glutPostRedisplay(); // Se manda el dibujo
}

void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case 's':
	case 'S':
		// canviem de mode solid a alambric
		if (ALAMBRICO) ALAMBRICO = false;
		else ALAMBRICO = true;
		break;
	case 'n':
	case 'N':
		// activem o desactivem la boira
		if (BOIRA) BOIRA = false;
		else BOIRA = true;
		break;
	case 'l':
	case 'L':
		// activem o desactivem el mode nit amb la iluminacio
		if (NIT) {
			NIT = false;
			engine->drop(); // delete engine
			// reproduim la cançó de l'inici
			engine = createIrrKlangDevice();
			if (!VELOCITAT) engine->play2D("./Confusion.mp3", true); // true per a que reproduisca en bucle
			else engine->play2D("./Highway to Hell.mp3", true);
		}
		else {
			NIT = true;
			engine->drop(); // delete engine
			// reproduim la cançó  del terror
			engine = createIrrKlangDevice();
			engine->play2D("./terror.mp3", true); // true per a que reproduisca en bucle
		}
		break;
	case 'c':
	case 'C':
		// Elements solidaris a la camera
		if (SOLIDARIO) SOLIDARIO = false;
		else SOLIDARIO = true;
		break;
	case 'd':
	case 'D':
		// Activem el mode dificil
		if (DIFICIL) {
			A = 8;
			T = 100;
			DIFICIL = false;
		}
		else {
			// canviem els valors de A i T per a fer-ho més dificil
			A = 12;
			T = 70;
			DIFICIL = true;
		}
		break;
	case 'v':
	case 'V':
		if (VELOCITAT) {
			VELOCITAT = false;
			engine->drop(); // delete engine
			// reproduim la cançó de l'inici
			engine = createIrrKlangDevice();
			engine->play2D("./Confusion.mp3", true); // true per a que reproduisca en bucle
		}
		else {
			VELOCITAT = true;
			if (!NIT) {
				engine->drop(); // delete engine
				// reproduim una altra musica
				engine = createIrrKlangDevice();
				engine->play2D("./Highway to Hell.mp3", true); // true per a que reproduisca en bucle
			}
		}
		break;
	case 'h':
	case 'H':
		// Activem o desactivem el mode altura, camera alta
		if (ALTURA) ALTURA = false;
		else ALTURA = true;
		break;
	case 'r':
	case 'R':
		// Resetegem tot com ho teniem a l'inici
		ALAMBRICO = false;
		NIT = false;
		BOIRA = false;
		DIFICIL = false;
		SOLIDARIO = false;
		VELOCITAT = false;
		ALTURA = false;
		CANVIFIGURA = false;
		engine->drop(); // delete engine
		// reproduim la cançó inicial
		engine = createIrrKlangDevice();
		engine->play2D("./Confusion.mp3", true); // true per a que reproduisca en bucle
		break;
	case 'w':
	case 'W':
		// Canviem de vehicle
		if (CANVIFIGURA) CANVIFIGURA = false;
		else CANVIFIGURA = true;
		break;
	case 'p':
	case 'P':
		// Guardem una captura del que hi ha a la pantalla a la carpeta del projecte
		saveScreenshot((char*)"captura.jpg", 1280, 720);
		break;
	case 27:
		// Ixim del programa
		exit(0);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
	FreeImage_Initialise(); // per a freeimage
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(1280, 720); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	glutTimerFunc(1000 / 60, onTimer, 60);
	//glutTimerFunc(2000, onTimer, 60);
	glutTimerFunc(1000, actualitzaMonedes, 2000);
	init(); // Inicializacion propia. IMPORTANTE SITUAR AQUI
	std::cout << PROYECTO << " running\n" << std::endl; // Mensaje por consola

	// missatge per pantalla per a indicar els controls a l'usuari SENSE ACCENTS
	std::cout << "Flecha izquierda/derecha: giro del vehiculo " << std::endl;
	std::cout << "Flecha arriba/abajo: aumento/disminucion de la velocidad " << std::endl;
	std::cout << "S/s: activa/desactiva un modelo simple en alambrico sin luces ni texturas " << std::endl;
	std::cout << "L/l: cambia entre modo diurno/nocturno " << std::endl;
	std::cout << "N/n: cambia el estado de la niebla (on/off) " << std::endl;
	std::cout << "C/c: cambia la visibilidad de elementos solidarios a la camara -HUD- (on/off) " << std::endl;
	std::cout << "D/d: activa / desactiva modo dificil " << std::endl;
	std::cout << "V/v: activa / desactiva modo alta velocidad. Aceleraras y giraras mucho mas!" << std::endl;
	std::cout << "H/h: activa / desactiva la vista de aguila" << std::endl;
	std::cout << "W/w: cambia el vehiculo" << std::endl;
	std::cout << "R/r: resetea y vuelve al modo inicial" << std::endl;
	std::cout << "P/p: guarda una captura de pantalla" << std::endl;

	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutSpecialFunc(onArrow);
	glutKeyboardFunc(onKey);
	glutSpecialUpFunc(onKeyRelease);
	glutMainLoop(); // Puesta en marcha del programa

	FreeImage_DeInitialise();
}