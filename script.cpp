#include <GL\glut.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

float lado = 20;
GLuint texID1;
double movx = 20;
double movy = 100;
double movz = 200;
double velocidadeCamera = 5.0f;


void quadrado(float SIZE, GLuint texid1)
{
	glColor3f(1.0, 1.0, 1.0);
	
	
	//Estrutura da casa
	glBindTexture(GL_TEXTURE_2D, texid1);
	glBegin(GL_QUADS);
		// Face frontral 
		glTexCoord2f(0,0);		glVertex3f(-SIZE, -SIZE, 0.0);
		glTexCoord2f(1,0);		glVertex3f(SIZE, -SIZE, 0.0);
		glTexCoord2f(1,1);		glVertex3f(SIZE, SIZE, 0.0);
		glTexCoord2f(0,1);		glVertex3f(-SIZE, SIZE, 0.0);
		
		// Face direita
		glTexCoord3f(0,0,0);	glVertex3f(SIZE, SIZE, 0);
		glTexCoord3f(1,0,1);	glVertex3f(SIZE, SIZE, -SIZE*2);
		glTexCoord3f(1,1,1);	glVertex3f(SIZE, -SIZE, -SIZE*2);
		glTexCoord3f(0,1,0);	glVertex3f(SIZE, -SIZE, 0);
		
		// Face esquerda
		glTexCoord3f(0,0,0);	glVertex3f(-SIZE, -SIZE, 0);
		glTexCoord3f(1,0,1);	glVertex3f(-SIZE, -SIZE, -SIZE*2);
		glTexCoord3f(1,1,1);	glVertex3f(-SIZE, SIZE, -SIZE*2);
		glTexCoord3f(0,1,0);	glVertex3f(-SIZE, SIZE, 0);
		
		// Face posterior
		glTexCoord2f(0,0);		glVertex3f(-SIZE, -SIZE, -SIZE*2);
		glTexCoord2f(1,0);		glVertex3f(SIZE, -SIZE, -SIZE*2);
		glTexCoord2f(1,1);		glVertex3f(SIZE, SIZE, -SIZE*2);
		glTexCoord2f(0,1);		glVertex3f(-SIZE, SIZE, -SIZE*2);
		
		// Face superior
		glTexCoord2f(0,0);		glVertex3f(-SIZE, SIZE, 0);
		glTexCoord2f(1,0);		glVertex3f(-SIZE, SIZE, -SIZE*2);
		glTexCoord2f(1,1);		glVertex3f(SIZE, SIZE, -SIZE*2);
		glTexCoord2f(0,1);		glVertex3f(SIZE, SIZE, 0);
		
		// Face inferior
		glTexCoord2f(0,0);		glVertex3f(-SIZE, -SIZE, 0);
		glTexCoord2f(1,0);		glVertex3f(-SIZE, -SIZE, -SIZE*2);
		glTexCoord2f(1,1);		glVertex3f(SIZE, -SIZE, -SIZE*2);
		glTexCoord2f(0,1);		glVertex3f(SIZE, -SIZE, 0);
	glEnd();
	
	//Telhado da casa
	glBegin(GL_TRIANGLES);
		// Face 1
		glVertex3f(-SIZE, SIZE, 0);
		glVertex3f(SIZE, SIZE, 0);
		glVertex3f(0, SIZE*2, -SIZE);
		
		// Face 2
		glVertex3f(SIZE, SIZE, 0);
		glVertex3f(SIZE, SIZE, -SIZE*2);
		glVertex3f(0, SIZE*2, -SIZE);
		
		// Face 3
		glVertex3f(-SIZE, SIZE, 0);
		glVertex3f(-SIZE, SIZE, -SIZE*2);
		glVertex3f(0, SIZE*2, -SIZE);
		
		// Face 4
		glVertex3f(-SIZE, SIZE, -SIZE*2);
		glVertex3f(SIZE, SIZE, -SIZE*2);
		glVertex3f(0, SIZE*2, -SIZE);
	glEnd();
	
	// Porta da casa
	/*glBegin(GL_QUADS);
		glVertex3f(SIZE-5, -SIZE, 0);
		glVertex3f(SIZE-5, -SIZE+8, 0);
		glVertex3f(SIZE-10, -SIZE+8, 0);
		glVertex3f(SIZE-10, -SIZE, 0);
	glEnd();*/
	
}

void desenho(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	quadrado(lado, texID1);
	
	glFlush();
}

void ajuste(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45,w/h, 0.4, 500);
	
}

void carregarTextura(GLuint tex_id1, string filepath){
	unsigned char* imgData;
	int largura, altura, canais;
	
	imgData = stbi_load(filepath.c_str(), &largura, &altura,&canais, 4);
	
	if(imgData){
		glBindTexture(GL_TEXTURE_2D, tex_id1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, 
		altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		stbi_image_free(imgData);
	}else{
		cout<<"ERRO!Não foi possível carregar a imagem "<<filepath.c_str()<<endl;
	}
	
}

void initializeTexture(){
	glGenTextures(1, &texID1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	carregarTextura(texID1, "images/tijolo.jpg");
	
	glEnable(GL_DEPTH_TEST);
}

void initializeLight(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	float luzAmbiente[4] = {0.2, 0.2, 0.2, 1};
	float luzDifusa[4] = {0.7, 0.7, 0.7, 1};
	float luzEspecular[4] = {1, 1, 1, 1};
	float posicaoLuz0[4] = {0, 20, 0, 1};
	float especularidade[4] = {1, 1, 1, 1};
	int especMaterial = 128;
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	glMaterialf(GL_FRONT, GL_SHININESS, especMaterial);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0);
		
	
}

void teclado(unsigned char key, int x, int y){
	glutPostRedisplay();
	
	if(key == 'w')
		movz -= velocidadeCamera;
	if(key == 's')
		movz += velocidadeCamera;
		
	if(key == 'a')
		movx -= velocidadeCamera;
	if(key == 'd')
		movx += velocidadeCamera;
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(70,movy,movz,
			movx,0,0,
			0,1,0);
}

int main(){
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("3D");
	glutDisplayFunc(desenho);
	glutReshapeFunc(ajuste);
	glutKeyboardFunc(teclado);
	initializeTexture();
	initializeLight();
	glutMainLoop();	
}
