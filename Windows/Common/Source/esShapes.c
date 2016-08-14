//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESShapes.c
//
//    Utility functions for generating shapes
//

///
//  Includes
//
#include "esUtil.h"
#include <stdlib.h>
#include <math.h>

///
// Defines
//
#define ES_PI  (3.14159265f)

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//



//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals, 
                             GLfloat **texCoords, GLuint **indices )
{
   int i;
   int j;
   int numParallels = numSlices / 2;
   int numVertices = ( numParallels + 1 ) * ( numSlices + 1 );
   int numIndices = numParallels * numSlices * 6;
   float angleStep = (2.0f * ES_PI) / ((float) numSlices);

   // Allocate memory for buffers
   if ( vertices != NULL )
      *vertices = malloc ( sizeof(GLfloat) * 3 * numVertices );
   
   if ( normals != NULL )
      *normals = malloc ( sizeof(GLfloat) * 3 * numVertices );

   if ( texCoords != NULL )
      *texCoords = malloc ( sizeof(GLfloat) * 2 * numVertices );

   if ( indices != NULL )
      *indices = malloc ( sizeof(GLuint) * numIndices );

   for ( i = 0; i < numParallels + 1; i++ )
   {
      for ( j = 0; j < numSlices + 1; j++ )
      {
         int vertex = ( i * (numSlices + 1) + j ) * 3; 

         if ( vertices )
         {
            (*vertices)[vertex + 0] = radius * sinf ( angleStep * (float)i ) *
                                               sinf ( angleStep * (float)j );
            (*vertices)[vertex + 1] = radius * cosf ( angleStep * (float)i );
            (*vertices)[vertex + 2] = radius * sinf ( angleStep * (float)i ) *
                                               cosf ( angleStep * (float)j );
         }

         if ( normals )
         {
            (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
            (*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
            (*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
         }

         if ( texCoords )
         {
            int texIndex = ( i * (numSlices + 1) + j ) * 2;
            (*texCoords)[texIndex + 0] = (float) j / (float) numSlices;
            (*texCoords)[texIndex + 1] = ( 1.0f - (float) i ) / (float) (numParallels - 1 );
         }
      }
   }

   // Generate the indices
   if ( indices != NULL )
   {
      GLuint *indexBuf = (*indices);
      for ( i = 0; i < numParallels ; i++ ) 
      {
         for ( j = 0; j < numSlices; j++ )
         {
            *indexBuf++  = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );

            *indexBuf++ = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );
            *indexBuf++ = i * ( numSlices + 1 ) + ( j + 1 );
         }
      }
   }

   return numIndices;
}

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals, 
                           GLfloat **texCoords, GLuint **indices )
{
   int i;
   int numVertices = 24;
   int numIndices = 36;
   
   GLfloat cubeVerts[] =
   {
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,
      -0.5f,  0.5f, 0.5f,
      0.5f,  0.5f, 0.5f, 
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
   };

   GLfloat cubeNormals[] =
   {
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
   };

   GLfloat cubeTex[] =
   {
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
   };
   
   // Allocate memory for buffers
   if ( vertices != NULL )
   {
      *vertices = malloc ( sizeof(GLfloat) * 3 * numVertices );
      memcpy( *vertices, cubeVerts, sizeof( cubeVerts ) );
      for ( i = 0; i < numVertices * 3; i++ )
      {
         (*vertices)[i] *= scale;
      }
   }

   if ( normals != NULL )
   {
      *normals = malloc ( sizeof(GLfloat) * 3 * numVertices );
      memcpy( *normals, cubeNormals, sizeof( cubeNormals ) );
   }

   if ( texCoords != NULL )
   {
      *texCoords = malloc ( sizeof(GLfloat) * 2 * numVertices );
      memcpy( *texCoords, cubeTex, sizeof( cubeTex ) ) ;
   }


   // Generate the indices
   if ( indices != NULL )
   {
      GLuint cubeIndices[] =
      {
         0, 2, 1,
         0, 3, 2, 
         4, 5, 6,
         4, 6, 7,
         8, 9, 10,
         8, 10, 11, 
         12, 15, 14,
         12, 14, 13, 
         16, 17, 18,
         16, 18, 19, 
         20, 23, 22,
         20, 22, 21
      };

      *indices = malloc ( sizeof(GLuint) * numIndices );
      memcpy( *indices, cubeIndices, sizeof( cubeIndices ) );
   }

   return numIndices;
}

#if 0
     v1
     /\
	/  \
   / v4 \
   v2   v3
#endif
#define ARROW_V1	0.0f, 1.0f, 0.0f
#define ARROW_V2	-0.3f, -0.3f, 0.0f
#define ARROW_V3	0.3f, -0.3f, 0.0f
#define ARROW_V4	0.0f, 0.0f, 0.3f

#define ARROW_C1	1.0f, 0.0f, 0.0f, 0.0f
#define ARROW_C2	0.0f, 1.0f, 0.0f, 0.0f
#define ARROW_C3	0.0f, 0.0f, 1.0f, 0.0f

int ESUTIL_API esGenArrow ( float scale, GLfloat **vertices, GLfloat **colors, GLfloat **normals, 
                           GLfloat **texCoords, GLuint **indices )
{
   int i;
   int numVertices = 12;
   int numIndices = 12;
   
   GLfloat arrowVerts[] =
   {  
	  //ARROW_V1,
      //ARROW_V2,
      //ARROW_V3,
	  
      ARROW_V1,
	  ARROW_V4,
      ARROW_V2,
      
      ARROW_V1,
      ARROW_V4,
      ARROW_V3,

      //ARROW_V2,
      //ARROW_V3,
      //ARROW_V4,     
   };

   GLfloat arrowColors[] =
   {
      ARROW_C1,
      ARROW_C1,
      ARROW_C1,

      ARROW_C2,
      ARROW_C2,
      ARROW_C2,

      ARROW_C3,
      ARROW_C3,
      ARROW_C3,

      ARROW_C1,
      ARROW_C1,
      ARROW_C1,
   };

   
   // Allocate memory for buffers
   if ( vertices != NULL )
   {
      *vertices = (GLfloat*)malloc ( sizeof(GLfloat) * 3 * numVertices );
      memcpy( *vertices, arrowVerts, sizeof( arrowVerts ) );
      for ( i = 0; i < numVertices*3; i++ )
      {
         (*vertices)[i] *= scale;
      }
   }

   if ( colors != NULL )
   {
      *colors = (GLfloat*)malloc ( sizeof(GLfloat) * 4 * numVertices );
      memcpy( *colors, arrowColors, sizeof( arrowColors ) );
   }

   // Generate the indices
   if ( indices != NULL )
   {
      GLuint arrowIndices[] =
      {
         0, 1, 2,
         3,4,5,
		 6,7,8,
		 9,10,11
      };

      *indices = malloc ( sizeof(GLuint) * numIndices );
      memcpy( *indices, arrowIndices, sizeof( arrowIndices ) );
   }

   return numIndices;
}
