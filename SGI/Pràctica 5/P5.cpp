#define PROYECTO "Reloj analogico"
#define _CRT_SECURE_NO_WARNINGS // açò ho pose pq no em deixa gastar la funció localtime sense esta definició
#include <iostream> // Biblioteca de entrada salida
#include <freeglut.h> // Biblioteca grafica
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime> // per a saber l'hora actual


static GLuint estrela, centre, seg, hores, cercleHores, min, manillahores, marcaminuts, cercleMinuts;
static float asec, tacumulat = 0, radi = 0.08, tacmin = 0, amin, ahores, tachores;
static const float velocidad = 0.05 * 360; // velocitat en la que es mou continuament
static float ang; // Angle en el que anem a rotar l'esfera
static float eyex = 0, eyey = 0, eyez = 7; // posició de la càmera

// HEM DE CANVIAR LA POSICIO PER ALGO GLOBAL ACI!!!!!

void init()
{
	ang = 0;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST); // Buffer actiu

	centre = glGenLists(1);
	glNewList(centre, GL_COMPILE);
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
	min = glGenLists(1);
	glNewList(min, GL_COMPILE);
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
	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glEnd();

	// segona part de l'estrela
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0);
	glEnd();
	glPopMatrix();
	glEndList();

	// agulla de les hores
	manillahores = glGenLists(1);
	glNewList(manillahores, GL_COMPILE);
	glPushMatrix();
	glColor3f(0.854, 0.160, 0.109);
	//ix cap a afora wtf sens el rotate
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.02, 0.4, 100, 100);
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
		} else {
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
}


void display()
// Funcion de atencion al dibujo
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla y el Z-buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1, 0); // Situa la camara
	glEnable(GL_DEPTH_TEST);


	// cridem a totes les llistes per a dibuixar les figures
	glPushMatrix();
	glCallList(centre);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1);
	glRotatef(-asec, 0, 0, 1);
	glCallList(seg);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-amin, 0, 0, 1);
	glCallList(min);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-ahores, 0, 0, 1);
	glCallList(manillahores);
	glPopMatrix();

	glPushMatrix();
	glRotatef(ang, 1, 0, 0);
	glCallList(cercleHores);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-ang, 0, 1, 0);
	glCallList(cercleMinuts);
	glPopMatrix();

	/* per a comprovar que cap en l'esfera unitat
	glColor3f(0.8, 0.0, 0.0);
	glutWireSphere(1.0, 20, 20); */

	glFlush(); // Finaliza el dibujo
	glutSwapBuffers();

}
void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h); // todo el area de dibujo
	float razon = (float)w / h;

	// matriz de proyeccion, definim la camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float distancia, fovy;
	distancia = sqrt(pow(eyex, 2) + pow(eyey, 2) + pow(eyez, 2)); // d de la posició de la camara a lo que mira
	fovy = (asin(1 / distancia)) * 2 * (180 / M_PI);
	gluPerspective(fovy, razon, 1, 10);
}
void onTimer(int valor)
// Funcion de atencion al timer periodico
{
	// hem de saber l'hora actual!!!!, ho fem en la biblioteca ctime
	time_t ara = time(0);
	tm* horaActual = localtime(&ara);

	//Calculamos el tiempo transcurrido desde la última vez
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME); //Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; //Tiempo transcurrido desde antes msg


	/*tacumulat += tiempo_transcurrido; // acumulem per a vore si ha passat 1 segon o 1 minut
	tacmin += tiempo_transcurrido;
	tachores += tiempo_transcurrido;*/

	ang += velocidad * tiempo_transcurrido / 1000.0;
	antes = ahora;

	/*
	if (tacumulat > 1000) {
		tacumulat = 0;
		//std::cout << PROYECTO << " 1s" << std::endl; // Mensaje por consola
		asec = asec + 6;
		if (asec >= 360) asec = (int)asec % 360;
	}
	if (tacmin > (60 * 1000)) {
		//std::cout << PROYECTO << " 1min" << std::endl; // Mensaje por consola
		tacmin = 0;
		amin += 6;
		if (amin >= 360) amin = (int)amin % 360;
	}
	if (tachores > (3600 * 1000)) {
		//std::cout << PROYECTO << " 1hora" << std::endl; // Mensaje por consola
		tachores = 0;
		ahores += 30;
		if (ahores >= 360) ahores = (int)ahores % 360;
	} */
	// CODI MÉS SENZILL, gastant la llibreria ctime
	amin = 360 / 60 * (horaActual->tm_min);
	asec = 360 / 60 * (horaActual->tm_sec);
	// hem de posar mòdul 12
	ahores = 360 / 12 * (horaActual->tm_hour % 12);

	glutTimerFunc(1000 / 60, onTimer, 60); // Se encola un nuevo timer
	glutPostRedisplay(); // Se manda el dibujo
}


void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo
	std::cout << PROYECTO << " running" << std::endl; // Mensaje por consola
	// tasaFPS = 60
	glutTimerFunc(1000 / 60, onTimer, 60);
	init();
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutMainLoop(); // Puesta en marcha del programa
}