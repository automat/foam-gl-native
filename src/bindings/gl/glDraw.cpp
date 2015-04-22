#include "glDraw.h"
#include <GL/glew.h>
#include "../utils/utils.h"

NAN_METHOD(drawLinef){
    NanScope();
    CHECK_ARGS_LEN(6);
    GLfloat vertices[6];
    vertices[0] = static_cast<GLfloat>(args[0]->NumberValue());
    vertices[1] = static_cast<GLfloat>(args[1]->NumberValue());
    vertices[2] = static_cast<GLfloat>(args[2]->NumberValue());
    vertices[3] = static_cast<GLfloat>(args[3]->NumberValue());
    vertices[4] = static_cast<GLfloat>(args[4]->NumberValue());
    vertices[5] = static_cast<GLfloat>(args[5]->NumberValue());
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    NanReturnUndefined();
}

//drawVectorf
//drawPointf
//drawPointsf
//drawLinesf
//drawRect
//drawRectPoints
//drawRectStroked
//drawCircle
//drawCircleStroked
//drawCircles
//setCircleSegments
//getCircleSegments
//drawCube
//drawCubePoints
//drawCubeStroked
//drawCubeColored
//drawGrid
//drawGridPoints
//drawPivot
//drawOnB
//drawMesh
//drawVboMesh
//drawVboMeshes
//colorf
//alpha
//getColor
//getAlpha
//drawScreenRectf
//drawWindowRect

void glDraw::init(Handle<Object> exports){
    EXPORT_SET_METHOD(drawLinef);
}
