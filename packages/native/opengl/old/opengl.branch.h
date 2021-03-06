
#ifndef OPENGL_WRAP_H_INCLUDED
#define OPENGL_WRAP_H_INCLUDED

// OpenGL Constants
#define boomslang_GL_QUADS                            boomslang_UnsignedInteger(GL_QUADS)
#define boomslang_GL_MODELVIEW                        boomslang_UnsignedInteger(GL_MODELVIEW)
#define boomslang_GL_PROJECTION                       boomslang_UnsignedInteger(GL_PROJECTION)
#define boomslang_GL_TEXTURE                          boomslang_UnsignedInteger(GL_TEXTURE)
#define boomslang_GL_COLOR                            boomslang_UnsignedInteger(GL_COLOR)

#define boomslang_GL_ALPHA_TEST                       boomslang_UnsignedInteger(GL_ALPHA_TEST)
#define boomslang_GL_AUTO_NORMAL                      boomslang_UnsignedInteger(GL_AUTO_NORMAL)
#define boomslang_GL_BLEND                            boomslang_UnsignedInteger(GL_BLEND)
#define boomslang_GL_COLOR_LOGIC_OP                   boomslang_UnsignedInteger(GL_COLOR_LOGIC_OP)
#define boomslang_GL_COLOR_MATERIAL                   boomslang_UnsignedInteger(GL_COLOR_MATERIAL)
#define boomslang_GL_COLOR_SUM                        boomslang_UnsignedInteger(GL_COLOR_SUM)
#define boomslang_GL_COLOR_TABLE                      boomslang_UnsignedInteger(GL_COLOR_TABLE)
#define boomslang_GL_CONVOLUTION_1D                   boomslang_UnsignedInteger(GL_CONVOLUTION_1D)
#define boomslang_GL_CONVOLUTION_2D                   boomslang_UnsignedInteger(GL_CONVOLUTION_2D)
#define boomslang_GL_CULL_FACE                        boomslang_UnsignedInteger(GL_CULL_FACE)
#define boomslang_GL_DEPTH_TEST                       boomslang_UnsignedInteger(GL_DEPTH_TEST)
#define boomslang_GL_DITHER                           boomslang_UnsignedInteger(GL_DITHER)
#define boomslang_GL_FOG                              boomslang_UnsignedInteger(GL_FOG)
#define boomslang_GL_HISTOGRAM                        boomslang_UnsignedInteger(GL_HISTOGRAM)
#define boomslang_GL_INDEX_LOGIC_OP                   boomslang_UnsignedInteger(GL_INDEX_LOGIC_OP)
#define boomslang_GL_LIGHTING                         boomslang_UnsignedInteger(GL_LIGHTING)
#define boomslang_GL_LINE_SMOOTH                      boomslang_UnsignedInteger(GL_LINE_SMOOTH)
#define boomslang_GL_LINE_STIPPLE                     boomslang_UnsignedInteger(GL_LINE_STIPPLE)
#define boomslang_GL_MINMAX                           boomslang_UnsignedInteger(GL_MINMAX)
#define boomslang_GL_MULTISAMPLE                      boomslang_UnsignedInteger(GL_MULTISAMPLE)
#define boomslang_GL_NORMALIZE                        boomslang_UnsignedInteger(GL_NORMALIZE)
#define boomslang_GL_POINT_SMOOTH                     boomslang_UnsignedInteger(GL_POINT_SMOOTH)
#define boomslang_GL_POINT_SPRITE                     boomslang_UnsignedInteger(GL_POINT_SPRITE)
#define boomslang_GL_POLYGON_OFFSET_FILL              boomslang_UnsignedInteger(GL_POLYGON_OFFSET_FILL)
#define boomslang_GL_POLYGON_OFFSET_LINE              boomslang_UnsignedInteger(GL_POLYGON_OFFSET_LINE)
#define boomslang_GL_POLYGON_OFFSET_POINT             boomslang_UnsignedInteger(GL_POLYGON_OFFSET_POINT)
#define boomslang_GL_POLYGON_SMOOTH                   boomslang_UnsignedInteger(GL_POLYGON_SMOOTH)
#define boomslang_GL_POLYGON_STIPPLE                  boomslang_UnsignedInteger(GL_POLYGON_STIPPLE)
#define boomslang_GL_POST_COLOR_MATRIX_COLOR_TABLE    boomslang_UnsignedInteger(GL_POST_COLOR_MATRIX_COLOR_TABLE)
#define boomslang_GL_POST_CONVOLUTION_COLOR_TABLE     boomslang_UnsignedInteger(GL_POST_CONVOLUTION_COLOR_TABLE)
#define boomslang_GL_RESCALE_NORMAL                   boomslang_UnsignedInteger(GL_RESCALE_NORMAL)
#define boomslang_GL_SAMPLE_ALPHA_TO_COVERAGE         boomslang_UnsignedInteger(GL_SAMPLE_ALPHA_TO_COVERAGE)
#define boomslang_GL_SAMPLE_ALPHA_TO_ONE              boomslang_UnsignedInteger(GL_SAMPLE_ALPHA_TO_ONE)
#define boomslang_GL_SAMPLE_COVERAGE                  boomslang_UnsignedInteger(GL_SAMPLE_COVERAGE)
#define boomslang_GL_SEPARABLE_2D                     boomslang_UnsignedInteger(GL_SEPARABLE_2D)
#define boomslang_GL_SCISSOR_TEST                     boomslang_UnsignedInteger(GL_SCISSOR_TEST)
#define boomslang_GL_STENCIL_TEST                     boomslang_UnsignedInteger(GL_STENCIL_TEST)
#define boomslang_GL_TEXTURE_1D                       boomslang_UnsignedInteger(GL_TEXTURE_1D)
#define boomslang_GL_TEXTURE_2D                       boomslang_UnsignedInteger(GL_TEXTURE_2D)
#define boomslang_GL_TEXTURE_3D                       boomslang_UnsignedInteger(GL_TEXTURE_3D)
#define boomslang_GL_TEXTURE_CUBE_MAP                 boomslang_UnsignedInteger(GL_TEXTURE_CUBE_MAP)
#define boomslang_GL_TEXTURE_GEN_Q                    boomslang_UnsignedInteger(GL_TEXTURE_GEN_Q)
#define boomslang_GL_TEXTURE_GEN_R                    boomslang_UnsignedInteger(GL_TEXTURE_GEN_R)
#define boomslang_GL_TEXTURE_GEN_S                    boomslang_UnsignedInteger(GL_TEXTURE_GEN_S)
#define boomslang_GL_TEXTURE_GEN_T                    boomslang_UnsignedInteger(GL_TEXTURE_GEN_T)
#define boomslang_GL_VERTEX_PROGRAM_POINT_SIZE        boomslang_UnsignedInteger(GL_VERTEX_PROGRAM_POINT_SIZE)
#define boomslang_GL_VERTEX_PROGRAM_TWO_SIDE          boomslang_UnsignedInteger(GL_VERTEX_PROGRAM_TWO_SIDE)

#define boomslang_GL_COLOR_BUFFER_BIT                 boomslang_UnsignedInteger(GL_COLOR_BUFFER_BIT)

#define boomslang_GL_INVALID_ENUM                     boomslang_UnsignedInteger(GL_INVALID_ENUM)
#define boomslang_GL_INVALID_OPERATION                boomslang_UnsignedInteger(GL_INVALID_OPERATION)

// OpenGl Functions
#define boomslang_glClearColor(a,b,c,d)               glClearColor(a,b,c,d)
#define boomslang_glClear(a)                          glClear(a)
#define boomslang_glBegin(a)                          glBegin(a)
#define boomslang_glEnd()                             glEnd()
#define boomslang_glVertex2f(a,b)                     glVertex2f(a,b)
#define boomslang_glColor3f(a,b,c)                    glColor3f(a,b,c)
#define boomslang_glViewport(a,b,c,d)                 glViewport(a,b,c,d)
#define boomslang_glMatrixMode(a)                     glMatrixMode(a)
#define boomslang_glLoadIdentity()                    glLoadIdentity()
#define boomslang_glOrtho(a,b,c,d,e,f)                glOrtho(a,b,c,d,e,f)
#define boomslang_glPushMatrix()                      glPushMatrix()
#define boomslang_glPopMatrix()                       glPopMatrix()
#define boomslang_glScale(a,b,c)                      glScaled(a,b,c)
#define boomslang_glScaled(a,b,c)                     glScaled(a,b,c)
#define boomslang_glScalef(a,b,c)                     glScalef(a,b,c)
#define boomslang_glRotate(a,b,c,d)                   glRotated(a,b,c,d)
#define boomslang_glRotated(a,b,c,d)                  glRotated(a,b,c,d)
#define boomslang_glRotatef(a,b,c,d)                  glRotatef(a,b,c,d)
#define boomslang_glEnable(a)                         glEnable(a)
#define boomslang_glDisable(a)                        glDisable(a)

#endif // OPENGL_WRAP_H_INCLUDED
