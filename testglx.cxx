/*
	A demo of how to allocate a pbuffer, draw into it and dump
	the framebuffer into a file. 

	Draw a red cube to a file called framebuffer (original name).
        The companion program to this one is readframe. It reads the framebuffer
	file and loads it into a window on the screen. 

	Dave Semeraro
	NCSA
	January 2003

*/
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glut.h>
#include <GL/glx.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
#define MAX_PFORMATS 256
#define MAX_ATTRIBS 32

void draw_cube() {
        glBegin(GL_QUADS);
//      bottom
//      the normal for the bottom is specified here. All subsequent calls
//      to glVertex will have this normal set. IE calling glNormal3f sets
//      the current value for a normal.
        glNormal3f(0.,0.,-1.);
        glVertex3f(0.,0.,0.);
        glVertex3f(0.,1.,0.);
        glVertex3f(1.,1.,0.);
        glVertex3f(1.,0.,0.);
//      top
        glNormal3f(0.,0.,1.);
        glVertex3f(0.,0.,1.);
        glVertex3f(1.,0.,1.);
        glVertex3f(1.,1.,1.);
        glVertex3f(0.,1.,1.);
//      front
        glNormal3f(0.,-1.,0.);
        glVertex3f(0.,0.,0.);
        glVertex3f(1.,0.,0.);
        glVertex3f(1.,0.,1.);
        glVertex3f(0.,0.,1.);
//      back
        glNormal3f(0.,1.,0.);
        glVertex3f(0.,1.,0.);
        glVertex3f(0.,1.,1.);
        glVertex3f(1.,1.,1.);
        glVertex3f(1.,1.,0.);
//      left
        glNormal3f(-1.,0.,0.);
        glVertex3f(0.,0.,0.);
        glVertex3f(0.,0.,1.);
        glVertex3f(0.,1.,1.);
        glVertex3f(0.,1.,0.);
//      right
        glNormal3f(1.,0.,0.);
        glVertex3f(1.,0.,0.);
        glVertex3f(1.,1.,0.);
        glVertex3f(1.,1.,1.);
        glVertex3f(1.,0.,1.);
        glEnd();
        return;
}
// initialize the pbuffer object 
// this routine handles all initialization of the pbuffer.
// the rest of the application should be ready to render
// if this routine returns 1. 
int INIT_PBUFFER(int width, int height) {
	char *glxversion;
	int iConfigCount;
	int configid;
	int curAttrib = 0;
	int iScreen;
	int iAttributes[2*MAX_ATTRIBS];
	Display *mydisplay;
	GLXPbuffer pBuffer = None;
	GLXContext m_glxContext;
	GLXFBConfig *fbConfigs;
	GLXFBConfig thechosenFBconfig;

#if defined(GLX_VERSION_1_3) || defined(GLX_VERSION_1_4)
	// grab the display and screen. The display and screen are the 
	// default. 
	mydisplay = XOpenDisplay(NULL);
        if(NULL == mydisplay) {
                cerr << "couldnt get display" << endl;
                return 0;
        }
	iScreen = DefaultScreen(mydisplay);
	// get the extensions this display supports
	glxversion = (char *) glXGetClientString(mydisplay,GLX_VERSION);
	if(!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4"))){
		cerr << " glx 1.3 or 1.4 needed at runtime" << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	glxversion = (char *) glXQueryServerString(mydisplay,iScreen,GLX_VERSION);
	if(!(strstr(glxversion, "1.3") || strstr(glxversion, "1.4"))) {
		cerr << " glx 1.3 or 1.4 needed at runtime" << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	// frame buffer configuration 
	// set up some attributes to get use in obtaining a frame buffer 
	// configuration.
	memset(iAttributes, 0, 2*MAX_ATTRIBS*sizeof(int));
	iAttributes[2*curAttrib  ] = GLX_RENDER_TYPE;
        iAttributes[2*curAttrib+1] = GLX_RGBA_BIT;
        curAttrib++;
	iAttributes[2*curAttrib  ] = GLX_DRAWABLE_TYPE;
        iAttributes[2*curAttrib+1] = GLX_PBUFFER_BIT;
        curAttrib++;
	iAttributes[2*curAttrib  ] = GLX_DEPTH_SIZE;
        iAttributes[2*curAttrib+1] = 1;
        curAttrib++;
// add the color sizes
	iAttributes[2*curAttrib   ] = GLX_RED_SIZE;
	iAttributes[2*curAttrib + 1] = 1;
	curAttrib++;
	iAttributes[2*curAttrib   ] = GLX_GREEN_SIZE;
	iAttributes[2*curAttrib + 1] = 1;
	curAttrib++;
	iAttributes[2*curAttrib   ] = GLX_BLUE_SIZE;
	iAttributes[2*curAttrib + 1] = 1;
	curAttrib++;
	iAttributes[2*curAttrib  ] = None;
	// get a list of configurations
	fbConfigs = glXChooseFBConfig(mydisplay,iScreen,iAttributes,&iConfigCount);
	if(iConfigCount == 0 || !fbConfigs) {
		cerr << "glXChooseFBConfig failed to find a suitable config" << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	// All the configs should satisfy our iAttributes criteria so just use
	// the first one. If that fails chose another or alter the attributes.
	thechosenFBconfig = fbConfigs[0];
	glXGetFBConfigAttrib(mydisplay, thechosenFBconfig,GLX_FBCONFIG_ID, &configid);
	cout << " Chose 0x" << std::hex << configid << "  as config "  << endl;
	printf ("Chose 0x%x as configid\n", configid);
	// Now creat the pbuffer 
	curAttrib = 0;
	memset(iAttributes, 0, 2*MAX_ATTRIBS*sizeof(int));
	iAttributes[2*curAttrib  ] = GLX_PBUFFER_WIDTH;
        iAttributes[2*curAttrib+1] = width;
	curAttrib++;
	iAttributes[2*curAttrib  ] = GLX_PBUFFER_HEIGHT;
        iAttributes[2*curAttrib+1] = height;
	curAttrib++;
	iAttributes[2*curAttrib  ] = GLX_LARGEST_PBUFFER;
        iAttributes[2*curAttrib+1] = False;
	curAttrib++;
	iAttributes[2*curAttrib  ] = GLX_PRESERVED_CONTENTS;
        iAttributes[2*curAttrib+1] = False;
	pBuffer = glXCreatePbuffer(mydisplay,thechosenFBconfig, iAttributes);
	if(pBuffer == None) {
		cerr << "pbuffer creation failed " << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	// create glx context
	m_glxContext = glXCreateNewContext(mydisplay,thechosenFBconfig,GLX_RGBA_TYPE, NULL, True);
	if(!m_glxContext) {
		cerr << " glx create context failed " << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	// bind the context
	if(!glXMakeCurrent(mydisplay,pBuffer,m_glxContext)) {
		cerr << " glxMakeCurrent failed " << endl;
		XCloseDisplay(mydisplay);
		return 0;
	}
	return 1;
#else
	cerr << " Need GLX version 1.3 or 1.4 to compile" << endl; 
	return 0;
#endif
}
int main( int argc, char **argv )
{
	int fd;
	FILE *f;
	int width,height;
	GLfloat position[] = {5.0,5.0,5.0,1.0};
	GLubyte frame[500*500*4];
        GLfloat red_material[] = {1.,0.,0.,1.};
        GLfloat spec_material[] = {1.,1.,1.,1.};
        GLfloat shine[] = { 50. };
	width = 500;
	height = 500;
	if(!INIT_PBUFFER(width,height)){
		cerr << " pbuffer init failed " << endl;
		exit(0);
	}
	glClearColor(0.,0.,0.,0.);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.577, 0.577, -0.577, 0.577, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
// 
//      draw something into the buffer...
//
	glClear (GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.,0.,-3.666);
	glRotatef(45.0,1.,0.,0.);
	glRotatef(45.0,0.,0.,1.);
        glMaterialfv(GL_FRONT, GL_AMBIENT, red_material);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red_material);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec_material);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	draw_cube();
	glPopMatrix();
	glFlush();
	cout << " I think I drew something.. " << endl;
//
//	read some pixels
//
	glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, frame );
//
//	open a file and write the frame buffer contents to it. 
//
	/*fd = creat("framebuffer",0644);
	if(fd == -1) {
		cout << " could not creat frame buffer file " << endl;
		exit(1);
	}
	write(fd,frame,width*height*4); */
	f = fopen("framebuffer.ppm", "w");
	fprintf(f,"P6\n");
	fprintf(f,"# ppm-file created by %s\n", "testglx");
	fprintf(f,"%i %i\n",width,height);
	fprintf(f,"255\n");
	fclose(f);
	f = fopen("framebuffer.ppm","ab");
	for(int i=0;i<height;i++) {
		GLubyte *row;
		row = frame + (height-1-i) *width*3;
		fwrite(row,1,width*3,f);
	} 
	fclose(f);
}

