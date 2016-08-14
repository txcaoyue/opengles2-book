//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Simple_VertexShader.c
//
//    This is a simple example that draws a rotating cube in perspective
//    using a vertex shader to transform the object
//
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Attribute locations
   GLint  positionLoc;
   GLint  colorLoc;

   // Uniform locations
   GLint  mvpLoc;
   
   // Vertex daata
   GLfloat  *vertices;
   GLfloat  *colors;
   GLuint   *indices;
   int       numIndices;

   // Rotation angle
   GLfloat   angle;
   GLint	increase;

   // MVP matrix
   ESMatrix  mvpMatrix;
} UserData;

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =  
      "uniform mat4 u_mvpMatrix;                   \n"
      "attribute vec4 a_position;                  \n"
	  "attribute vec4 a_color;\n"
	  "varying vec4 v_color;\n"
      "void main()                                 \n"
      "{                                           \n"
      "   gl_Position = u_mvpMatrix * a_position;  \n"
	  "   v_color = a_color;\n"
      "}                                           \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
	  "varying vec4 v_color;\n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = v_color;        \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObject, "a_position" );
   userData->colorLoc = glGetAttribLocation ( userData->programObject, "a_color" );

   // Get the uniform locations
   userData->mvpLoc = glGetUniformLocation( userData->programObject, "u_mvpMatrix" );
   
   // Generate the vertex data
    //userData->numIndices = esGenCube( 1.0, &userData->vertices, NULL, NULL, &userData->indices );
  userData->numIndices = esGenArrow( 1.0, &userData->vertices, &userData->colors, NULL, NULL, &userData->indices );
   //userData->numIndices = esGenSphere(16, 1.0, &userData->vertices, NULL, NULL, &userData->indices );
   
   // Starting rotation angle for the cube
   userData->angle = -45.0f;
   userData->increase = 1;
   glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
   return TRUE;
}

#define ANGLE_MAX (75.0f)
#define ANGLE_MIN (-75.0f)
///
// Update MVP matrix based on time
//
void Update ( ESContext *esContext, float deltaTime )
{
   UserData *userData = (UserData*) esContext->userData;
   ESMatrix perspective;
   ESMatrix modelview;
   float    aspect;
   
   // Compute a rotation angle based on time to rotate the cube
   if (userData->increase) {
	userData->angle += ( deltaTime * 100.0f );   
	if( userData->angle >= ANGLE_MAX ) {
		userData->angle = ANGLE_MAX;
		userData->increase = 0;
	}
   }
   else {
		userData->angle -= ( deltaTime * 60.0 );
			if( userData->angle <= ANGLE_MIN ) {
		userData->angle = ANGLE_MIN;
		userData->increase = 1;
	}
   }


   // Compute the window aspect ratio
   aspect = (GLfloat) esContext->width / (GLfloat) esContext->height;
   
   // Generate a perspective matrix with a 60 degree FOV
   esMatrixLoadIdentity( &perspective );
   esPerspective( &perspective, 60.0f, aspect, 1.0f, 20.0f );

   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity( &modelview );

   // Translate away from the viewer
   esTranslate( &modelview, 0.0, 0.0, -2.0 );

   // Rotate the cube
   esRotate( &modelview, 60.0f, 1.0, 0.0, 0.0 );
   esRotate( &modelview, userData->angle, 0.0, 0.0, 1.0 );
   
   // Compute the final MVP by multiplying the 
   // modevleiw and perspective matrices together
   esMatrixMultiply( &userData->mvpMatrix, &modelview, &perspective );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   
   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable (GL_DEPTH_TEST);

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), userData->vertices );
   glVertexAttribPointer ( userData->colorLoc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), userData->colors );
   
   glEnableVertexAttribArray ( userData->positionLoc );
   glEnableVertexAttribArray ( userData->colorLoc );
   
   
   // Load the MVP matrix
   glUniformMatrix4fv( userData->mvpLoc, 1, GL_FALSE, (GLfloat*) &userData->mvpMatrix.m[0][0] );
   
   // Draw the cube
   glDrawElements ( GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT, userData->indices );
   //glDrawElements ( GL_TRIANGLES, 12, GL_UNSIGNED_INT, userData->indices );
   //glDrawArrays(GL_TRIANGLES,0,3);

   eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   if ( userData->vertices != NULL )
   {
      free ( userData->vertices );
   }

   if ( userData->indices != NULL )
   {
      free ( userData->indices );
   }

   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int main ( int argc, char *argv[] )
{
   ESContext esContext;
   UserData  userData;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Simple Texture 2D", 320, 240, ES_WINDOW_RGB );
   
   if ( !Init ( &esContext ) )
      return 0;

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );
   
   esMainLoop ( &esContext );

   ShutDown ( &esContext );
}
