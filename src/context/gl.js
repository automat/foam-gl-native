var ContextGLNative = require('bindings')('foam_gl_native');
var gl_ = ContextGLNative.gl;
var gl  = {};

/*--------------------------------------------------------------------------------------------*/
// OPENGL ERROR
/*--------------------------------------------------------------------------------------------*/

gl.getError = function(){};

/*--------------------------------------------------------------------------------------------*/
// VERTEX ARRAYS
/*--------------------------------------------------------------------------------------------*/

gl.vertexPointer = function(size,type,stride,pointer){};
gl.normalPointer = function(type,stride,pointer){};
gl.colorPointer = function(size,type,stride,pointer){};
gl.secondaryColorPointer = function(size,type,stride,pointer){};
gl.indexPointer = function(type,stride,pointer){};
gl.edgeFlagPointer = function(stride,pointer){};
gl.fogCoordPointer = function(type,stride,pointer){};
gl.texCoordPointer = function(size,type,stride,pointer){};
gl.vertexAttribPointer = function(index,size,type,normalized,stride,pointer){};
gl.vertexAttribIPointer = function(index,size,type,stride,pointer){};
gl.vertexAttribLPointer = function(index,size,type,stride,pointer){};
gl.enableClientState = function(array){};
gl.disableClientState = function(array){};
gl.vertexAttribDivisor = function(index,divisor){};
gl.clientActiveTexture = function(texture){};
gl.arrayElement = function(i){};

//region DRAWING COMMANDS
gl.drawArrays = function(mode,first,count){};
gl.drawArraysInstanced = function(mode,first,count,primcount){};
gl.drawElements = function(mode,count,type,indices){};
gl.drawRangeElements = function(mode,start,end,count,type,indices){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// VERTEX SPECIFICATIONS
/*--------------------------------------------------------------------------------------------*/

//

gl.disable =
gl.enable =
gl.finish =
gl.flush =
gl.getParameter =
gl.hint =
gl.isEnabled =
gl.pixelStorei = null;


/*--------------------------------------------------------------------------------------------*/
// PROGRAMS AND SHADERS
/*--------------------------------------------------------------------------------------------*/

//region SHADER OBJECTS
gl.createShader = function(type){};
gl.shaderSource = function(shader,string){};
gl.compileShader = function(shader){};
gl.deleteShader = function(shader){};
//endregion

//region PROGRAM OBJECTS
gl.createProgram = function(){};
gl.attachShader = function(program,shader){};
gl.detachShader = function(program,shader){};
gl.linkProgram = function(program){};
gl.useProgram = function(program){};
gl.getProgramParameter = function(program,pname){};
gl.deleteProgram = function(program){};
//endregion

//region PROGRAM PIPELINE OBJECTS
gl.createProgramPipeline = function(){};
gl.deleteProgramPipeline = function(pipeline){};
gl.bindProgramPipeline = function(pipeline){};
gl.useProgramStages = function(pipeline,stages,program){};
gl.activeShaderProgram = function(pipeline,program){};
//endregion

//region VERTEX ATTRIBUTES
gl.getAttribLocation = function(program,name){};
gl.bindAttribLocation = function(program,name){};
//endregion

//region UNIFORM VARIABLES
gl.getUniformLocation = function(program,name){};
gl.uniform1f = function(location,x){};
gl.uniform2f = function(location,x,y){};
gl.uniform3f = function(location,x,y,z){};
gl.uniform4f = function(location,x,y,z,w){};
gl.uniform1i = function(location,x){};
gl.uniform2i = function(location,x,y){};
gl.uniform3i = function(location,x,y,z){};
gl.uniform4i = function(location,x,y,z,w){};
gl.uniformMatrix2fv = function(location,transpose,value){};
gl.uniformMatrix3fv = function(location,transpose,value){};
gl.uniformMatrix4fv = function(location,transpose,value){};
//endregion

//region UNIFORM BUFFER OBJECTS BINDING
//uniformBlockBinding
//endregion

//region SUBROUTINE UNIFORM VARIABLES
//getSubroutineUniformLocation
//getSubroutineIndex
//getActiveSubroutineUniformiv
//getActiveSubroutineUniformName
//getActiveSubroutineName
//uniformSubroutinesuiv
//endregion

//region VARYING VARIABLES
//transformFeedbackVaryings
//getTransformFeedbackVarying
//endregion

//region SHADER EXECUTION
gl.validateProgram = function(program){};
gl.validateProgramPipeline = function(pipeline){};
//endregion

//region FRAGMENT SHADERS
gl.bindFragDataLocation = function(program,colorNumber,name){};
gl.bindFragDataLocationIndex = function(program,colorNumber,index,name){};
gl.getFragDataLocation = function(program,name){};
gl.getFragDataIndex = function(program,name){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// SHADER AND PROGRAM QUERIES
/*--------------------------------------------------------------------------------------------*/

//region SHADER QUERIES
gl.isShader = function(shader){};
gl.getShaderInfoLog = function(shader){};
gl.getShaderSource = function(shader){};
//endregion

//region PROGRAM QUERIES
gl.isProgramPipeline = function(pipeline){};
gl.isProgram = function(program){};
gl.getProgramInfoLog = function(program){};
gl.getProgramPipelineInfoLog = function(program){};
gl.getShaderParameter = function(shader,pname){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// RASTERIZATION
/*--------------------------------------------------------------------------------------------*/

//region MULTISAMPLING
gl.getMultisamplefv = function(pname,index){};
gl.minSampleShading = function(value){};
//endregion

//region POINTS
gl.pointSize = function(size){};
gl.pointParameteri = function(pname,param){};
gl.pointParameterf = function(pname,param){};
//pointParameteriv
//pointParameterfv
//endregion

//region LINE SEGMENTS
gl.lineWidth = function(size){};
gl.lineStipple = function(factor,pattern){};
//endregion

//region POLYGONS
gl.frontFace = function(dir){};
gl.cullFace = function(mode){};
//endregion

//region STIPPLING
//endregion

//region POLYGON RASTERIZATION & DEPTH OFFSET
gl.polygonOffset = function(factor,units){};
//endregion

//region PIXEL STORAGE MODES
gl.pixelStorei = function(pname,param){};
gl.pixelStoref = function(pname,param){};
//endregion

//region COLOR TABLE SPECIFIATION
//endregion

//region RASTERIZATION OF PIXEL RECTANGLES
//endregion

//region PIXEL TRANSFER OPERATIONS
//endregion

//region BITMAPS
//endregion

/*--------------------------------------------------------------------------------------------*/
// WHOLE FRAMEBUFFER
/*--------------------------------------------------------------------------------------------*/

//region SELECTING A BUFFER FOR WRITING
gl.drawBuffer = function(buf){};
//endregion

//region FINE CONTROL OF BUFFER UPDATES
gl.indexMask = function(mask){};
gl.colorMask = function(r,g,b,a){};
gl.colorMaski = function(buf,r,g,b,a){};
gl.depthMask = function(mask){};
gl.stencilMask = function(mask){};
gl.stencilMaskSeparate = function(face,mask){};
//endregion

//region CLEARING THE BUFFERS
gl.clear = function(buf){};
gl.clearColor = function(r,g,b,a){};
gl.clearIndex = function(index){};
gl.clearDepth = function(d){};
gl.clearStencil = function(d){};
gl.clearAccum = function(r,g,b,a){};
gl.clearBufferfi = function(buffer,drawbuffer,depth,stencil){};
//endregion

//region ACCUMULATION BUFFER
gl.accum = function(op,value){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// COLOR SUM, FOG AND HINTS
/*--------------------------------------------------------------------------------------------*/

gl.fogi = function(pname,param){};
gl.fogf = function(pname,param){};

/*--------------------------------------------------------------------------------------------*/
// TEXTURING
/*--------------------------------------------------------------------------------------------*/

gl.activeTexture = function(texture){};

//region TEXTURE_OBJECTS
gl.bindTexture = function(target,texture){};
gl.createTexture = function(){};
gl.deleteTexture = function(texture){};
//endregion

//region SAMPLER OBJECTS
gl.createSampler = function(){};
gl.bindSampler = function(unit,sampler){};
gl.deleteSampler = function(sampler){};
//endregion

//region TEXTURE IMAGE SPEC
//endregion

//region ALTERNATE TEXTURE IMAGE SPEC
gl.copyTexImage2D = function(target,level,internalformat,x,y,width,height,border){};
gl.copyTexImage1D = function(target,level,internalformat,x,y,width,border){};
gl.copyTexSubImage3D = function(target,level,xoffset,yoffset,zoffset,x,y,width,height){};
gl.copyTexSubImage2D = function(target,level,xoffset,yoffset,x,y,width,height){};
gl.copyTexSubImage1D = function(target,level,xoffset,x,y,width){};
//endregio

//region COMPRESSED TEXTURE IMAGES
//endregion

//region MULTISAMPLE TEXTURES
//endregion

//region BUFFER TEXTURES
//endregion

//region TEXTURE PARAMETERS
gl.texParameteri = function(target,pname,params){};
gl.texParameterf = function(target,pname,params){};
gl.texParameterfv = function(target,pname,params){};
gl.texParameteriv = function(target,pname,params){};
gl.texParameterIiv = function(target,pname,params){};
gl.texParameterIfv = function(target,pname,params){};
//endregion

//region TEXTURE MINIFICATION
gl.generateMipMap = function(target){};
//endregion

//region TEXTURE ENVIRONMENTS & FUNCTIONS
gl.texEnvi = function(target,pname,param){};
gl.texEnvf = function(target,pname,param){};
gl.texEnviv = function(target,pname,params){};
gl.texEnvfv = function(target,pname,params){};
//endregion

//region TEXTURE QUERIES
gl.isTexture = function(texture){};
//endregion

//region SAMPLER QUERIES
//endregion

/*--------------------------------------------------------------------------------------------*/
// PER-FRAGMENT OPERATIONS
/*--------------------------------------------------------------------------------------------*/

//region SCISSOR TEST
gl.scissor = function(left,bottom,width,height){};
//endregion

//region MULTISAMPLE FRAGMENT OPERATIONS
gl.sampleCoverage = function(value,invert){};
gl.sampleMaski = function(maskNumber,mask){};
//endregion

//region ALPHA TEST
gl.alphaFunc = function(func,ref){};
//endregion

//region STENCIL TEST
gl.stencilFunc = function(func,ref,mask){};
gl.stencilFuncSeparate = function(face,func,ref,mask){};
gl.stencilOp = function(sfail,dpfail,dppass){};
gl.stencilOpSeparate = function(face,sfail,dpfail,dppass){};
//endregion

//region DEPTH BUFFER TESTING
gl.depthFunc = function(func){};
//endregion

//region OCCLUSSION QUERIES
gl.beginQuery = function(target,id){};
gl.endQuery = function(target){};
//endregion

//region BLENDING
gl.blendEquation = function(mode){};
gl.blendEquationi = function(buf,mode){};
gl.blendEquationSeparate = function(modeRGB,modeAlpha){};
gl.blendEquationSeparatei = function(buf,modeRGB,modeAlpha){};
gl.blendFunc = function(src,dst){};
gl.blendFunci = function(buf,src,dst){};
gl.blendFuncSeparate = function(srcRGB,dstRGB,srcAlpha,dstAlpha){};
gl.blendFuncSeparatei = function(buf,srcRGB,dstRGB,srcAlpha,dstAlpha){};
gl.blendColor = function(red,green,blue,alpha){};
//endregion

//region LOGICAL OPERATION
gl.logicalOp = function(op){};
//endregion

gl.viewport =
gl.depthRange = null;

//fragment shaders

//UNIFORMS AND ATTRIBUTES

gl.disableVertexAttribArray =
gl.enableVertexAttribArray =
//gl.uniform1fv =
//gl.uniform2fv =
//gl.uniform3fv =
//gl.uniform4fv =
gl.vertexAttribPointer = null;

//BUFFER

gl.createBuffer =
gl.bindBuffer =
gl.bufferData =
gl.bufferSubData =
gl.deleteBuffer =
gl.getBufferParameter =
gl.isBuffer = null;


//VERTEX ARRAY OBJECTS

gl.createVertexArray =
gl.deleteVertexArray =
gl.bindVertexArray =
gl.isVertexArray = null;

/*--------------------------------------------------------------------------------------------*/
// CONSTANTS
/*--------------------------------------------------------------------------------------------*/

gl.ZERO =
gl.FALSE =
gl.LOGIC_OP =
gl.NONE =
gl.TEXTURE_COMPONENTS =
gl.NO_ERROR =
gl.POINTS =
gl.CURRENT_BIT =
gl.TRUE =
gl.ONE =
gl.CLIENT_PIXEL_STORE_BIT =
gl.LINES =
gl.LINE_LOOP =
gl.POINT_BIT =
gl.CLIENT_VERTEX_ARRAY_BIT =
gl.LINE_STRIP =
gl.LINE_BIT =
gl.TRIANGLES =
gl.TRIANGLE_STRIP =
gl.TRIANGLE_FAN =
gl.QUADS =
gl.QUAD_STRIP =
gl.POLYGON_BIT =
gl.POLYGON =
gl.POLYGON_STIPPLE_BIT =
gl.PIXEL_MODE_BIT =
gl.LIGHTING_BIT =
gl.FOG_BIT =
gl.DEPTH_BUFFER_BIT =
gl.ACCUM =
gl.LOAD =
gl.RETURN =
gl.MULT =
gl.ADD =
gl.NEVER =
gl.ACCUM_BUFFER_BIT =
gl.LESS =
gl.EQUAL =
gl.LEQUAL =
gl.GREATER =
gl.NOTEQUAL =
gl.GEQUAL =
gl.ALWAYS =
gl.SRC_COLOR =
gl.ONE_MINUS_SRC_COLOR =
gl.SRC_ALPHA =
gl.ONE_MINUS_SRC_ALPHA =
gl.DST_ALPHA =
gl.ONE_MINUS_DST_ALPHA =
gl.DST_COLOR =
gl.ONE_MINUS_DST_COLOR =
gl.SRC_ALPHA_SATURATE =
gl.STENCIL_BUFFER_BIT =
gl.FRONT_LEFT =
gl.FRONT_RIGHT =
gl.BACK_LEFT =
gl.BACK_RIGHT =
gl.FRONT =
gl.BACK =
gl.LEFT =
gl.RIGHT =
gl.FRONT_AND_BACK =
gl.AUX0 =
gl.AUX1 =
gl.AUX2 =
gl.AUX3 =
gl.INVALID_ENUM =
gl.INVALID_VALUE =
gl.INVALID_OPERATION =
gl.STACK_OVERFLOW =
gl.STACK_UNDERFLOW =
gl.OUT_OF_MEMORY =
gl._2D =
gl._3D =
gl._3D_COLOR =
gl._3D_COLOR_TEXTURE =
gl._4D_COLOR_TEXTURE =
gl.PASS_THROUGH_TOKEN =
gl.POINT_TOKEN =
gl.LINE_TOKEN =
gl.POLYGON_TOKEN =
gl.BITMAP_TOKEN =
gl.DRAW_PIXEL_TOKEN =
gl.COPY_PIXEL_TOKEN =
gl.LINE_RESET_TOKEN =
gl.EXP =
gl.VIEWPORT_BIT =
gl.EXP2 =
gl.CW =
gl.CCW =
gl.COEFF =
gl.ORDER =
gl.DOMAIN =
gl.CURRENT_COLOR =
gl.CURRENT_INDEX =
gl.CURRENT_NORMAL =
gl.CURRENT_TEXTURE_COORDS =
gl.CURRENT_RASTER_COLOR =
gl.CURRENT_RASTER_INDEX =
gl.CURRENT_RASTER_TEXTURE_COORDS =
gl.CURRENT_RASTER_POSITION =
gl.CURRENT_RASTER_POSITION_VALID =
gl.CURRENT_RASTER_DISTANCE =
gl.POINT_SMOOTH =
gl.POINT_SIZE =
gl.POINT_SIZE_RANGE =
gl.POINT_SIZE_GRANULARITY =
gl.LINE_SMOOTH =
gl.LINE_WIDTH =
gl.LINE_WIDTH_RANGE =
gl.LINE_WIDTH_GRANULARITY =
gl.LINE_STIPPLE =
gl.LINE_STIPPLE_PATTERN =
gl.LINE_STIPPLE_REPEAT =
gl.LIST_MODE =
gl.MAX_LIST_NESTING =
gl.LIST_BASE =
gl.LIST_INDEX =
gl.POLYGON_MODE =
gl.POLYGON_SMOOTH =
gl.POLYGON_STIPPLE =
gl.EDGE_FLAG =
gl.CULL_FACE =
gl.CULL_FACE_MODE =
gl.FRONT_FACE =
gl.LIGHTING =
gl.LIGHT_MODEL_LOCAL_VIEWER =
gl.LIGHT_MODEL_TWO_SIDE =
gl.LIGHT_MODEL_AMBIENT =
gl.SHADE_MODEL =
gl.COLOR_MATERIAL_FACE =
gl.COLOR_MATERIAL_PARAMETER =
gl.COLOR_MATERIAL =
gl.FOG =
gl.FOG_INDEX =
gl.FOG_DENSITY =
gl.FOG_START =
gl.FOG_END =
gl.FOG_MODE =
gl.FOG_COLOR =
gl.DEPTH_RANGE =
gl.DEPTH_TEST =
gl.DEPTH_WRITEMASK =
gl.DEPTH_CLEAR_VALUE =
gl.DEPTH_FUNC =
gl.ACCUM_CLEAR_VALUE =
gl.STENCIL_TEST =
gl.STENCIL_CLEAR_VALUE =
gl.STENCIL_FUNC =
gl.STENCIL_VALUE_MASK =
gl.STENCIL_FAIL =
gl.STENCIL_PASS_DEPTH_FAIL =
gl.STENCIL_PASS_DEPTH_PASS =
gl.STENCIL_REF =
gl.STENCIL_WRITEMASK =
gl.MATRIX_MODE =
gl.NORMALIZE =
gl.VIEWPORT =
gl.MODELVIEW_STACK_DEPTH =
gl.PROJECTION_STACK_DEPTH =
gl.TEXTURE_STACK_DEPTH =
gl.MODELVIEW_MATRIX =
gl.PROJECTION_MATRIX =
gl.TEXTURE_MATRIX =
gl.ATTRIB_STACK_DEPTH =
gl.CLIENT_ATTRIB_STACK_DEPTH =
gl.ALPHA_TEST =
gl.ALPHA_TEST_FUNC =
gl.ALPHA_TEST_REF =
gl.DITHER =
gl.BLEND_DST =
gl.BLEND_SRC =
gl.BLEND =
gl.LOGIC_OP_MODE =
gl.INDEX_LOGIC_OP =
gl.COLOR_LOGIC_OP =
gl.AUX_BUFFERS =
gl.DRAW_BUFFER =
gl.READ_BUFFER =
gl.SCISSOR_BOX =
gl.SCISSOR_TEST =
gl.INDEX_CLEAR_VALUE =
gl.INDEX_WRITEMASK =
gl.COLOR_CLEAR_VALUE =
gl.COLOR_WRITEMASK =
gl.INDEX_MODE =
gl.RGBA_MODE =
gl.DOUBLEBUFFER =
gl.STEREO =
gl.RENDER_MODE =
gl.PERSPECTIVE_CORRECTION_HINT =
gl.POINT_SMOOTH_HINT =
gl.LINE_SMOOTH_HINT =
gl.POLYGON_SMOOTH_HINT =
gl.FOG_HINT =
gl.TEXTURE_GEN_S =
gl.TEXTURE_GEN_T =
gl.TEXTURE_GEN_R =
gl.TEXTURE_GEN_Q =
gl.PIXEL_MAP_I_TO_I =
gl.PIXEL_MAP_S_TO_S =
gl.PIXEL_MAP_I_TO_R =
gl.PIXEL_MAP_I_TO_G =
gl.PIXEL_MAP_I_TO_B =
gl.PIXEL_MAP_I_TO_A =
gl.PIXEL_MAP_R_TO_R =
gl.PIXEL_MAP_G_TO_G =
gl.PIXEL_MAP_B_TO_B =
gl.PIXEL_MAP_A_TO_A =
gl.PIXEL_MAP_I_TO_I_SIZE =
gl.PIXEL_MAP_S_TO_S_SIZE =
gl.PIXEL_MAP_I_TO_R_SIZE =
gl.PIXEL_MAP_I_TO_G_SIZE =
gl.PIXEL_MAP_I_TO_B_SIZE =
gl.PIXEL_MAP_I_TO_A_SIZE =
gl.PIXEL_MAP_R_TO_R_SIZE =
gl.PIXEL_MAP_G_TO_G_SIZE =
gl.PIXEL_MAP_B_TO_B_SIZE =
gl.PIXEL_MAP_A_TO_A_SIZE =
gl.UNPACK_SWAP_BYTES =
gl.UNPACK_LSB_FIRST =
gl.UNPACK_ROW_LENGTH =
gl.UNPACK_SKIP_ROWS =
gl.UNPACK_SKIP_PIXELS =
gl.UNPACK_ALIGNMENT =
gl.PACK_SWAP_BYTES =
gl.PACK_LSB_FIRST =
gl.PACK_ROW_LENGTH =
gl.PACK_SKIP_ROWS =
gl.PACK_SKIP_PIXELS =
gl.PACK_ALIGNMENT =
gl.MAP_COLOR =
gl.MAP_STENCIL =
gl.INDEX_SHIFT =
gl.INDEX_OFFSET =
gl.RED_SCALE =
gl.RED_BIAS =
gl.ZOOM_X =
gl.ZOOM_Y =
gl.GREEN_SCALE =
gl.GREEN_BIAS =
gl.BLUE_SCALE =
gl.BLUE_BIAS =
gl.ALPHA_SCALE =
gl.ALPHA_BIAS =
gl.DEPTH_SCALE =
gl.DEPTH_BIAS =
gl.MAX_EVAL_ORDER =
gl.MAX_LIGHTS =
gl.MAX_CLIP_PLANES =
gl.MAX_TEXTURE_SIZE =
gl.MAX_PIXEL_MAP_TABLE =
gl.MAX_ATTRIB_STACK_DEPTH =
gl.MAX_MODELVIEW_STACK_DEPTH =
gl.MAX_NAME_STACK_DEPTH =
gl.MAX_PROJECTION_STACK_DEPTH =
gl.MAX_TEXTURE_STACK_DEPTH =
gl.MAX_VIEWPORT_DIMS =
gl.MAX_CLIENT_ATTRIB_STACK_DEPTH =
gl.SUBPIXEL_BITS =
gl.INDEX_BITS =
gl.RED_BITS =
gl.GREEN_BITS =
gl.BLUE_BITS =
gl.ALPHA_BITS =
gl.DEPTH_BITS =
gl.STENCIL_BITS =
gl.ACCUM_RED_BITS =
gl.ACCUM_GREEN_BITS =
gl.ACCUM_BLUE_BITS =
gl.ACCUM_ALPHA_BITS =
gl.NAME_STACK_DEPTH =
gl.AUTO_NORMAL =
gl.MAP1_COLOR_4 =
gl.MAP1_INDEX =
gl.MAP1_NORMAL =
gl.MAP1_TEXTURE_COORD_1 =
gl.MAP1_TEXTURE_COORD_2 =
gl.MAP1_TEXTURE_COORD_3 =
gl.MAP1_TEXTURE_COORD_4 =
gl.MAP1_VERTEX_3 =
gl.MAP1_VERTEX_4 =
gl.MAP2_COLOR_4 =
gl.MAP2_INDEX =
gl.MAP2_NORMAL =
gl.MAP2_TEXTURE_COORD_1 =
gl.MAP2_TEXTURE_COORD_2 =
gl.MAP2_TEXTURE_COORD_3 =
gl.MAP2_TEXTURE_COORD_4 =
gl.MAP2_VERTEX_3 =
gl.MAP2_VERTEX_4 =
gl.MAP1_GRID_DOMAIN =
gl.MAP1_GRID_SEGMENTS =
gl.MAP2_GRID_DOMAIN =
gl.MAP2_GRID_SEGMENTS =
gl.TEXTURE_1D =
gl.TEXTURE_2D =
gl.FEEDBACK_BUFFER_POINTER =
gl.FEEDBACK_BUFFER_SIZE =
gl.FEEDBACK_BUFFER_TYPE =
gl.SELECTION_BUFFER_POINTER =
gl.SELECTION_BUFFER_SIZE =
gl.TEXTURE_WIDTH =
gl.TRANSFORM_BIT =
gl.TEXTURE_HEIGHT =
gl.TEXTURE_INTERNAL_FORMAT =
gl.TEXTURE_BORDER_COLOR =
gl.TEXTURE_BORDER =
gl.DONT_CARE =
gl.FASTEST =
gl.NICEST =
gl.AMBIENT =
gl.DIFFUSE =
gl.SPECULAR =
gl.POSITION =
gl.SPOT_DIRECTION =
gl.SPOT_EXPONENT =
gl.SPOT_CUTOFF =
gl.CONSTANT_ATTENUATION =
gl.LINEAR_ATTENUATION =
gl.QUADRATIC_ATTENUATION =
gl.COMPILE =
gl.COMPILE_AND_EXECUTE =
gl.BYTE =
gl.UNSIGNED_BYTE =
gl.SHORT =
gl.UNSIGNED_SHORT =
gl.INT =
gl.UNSIGNED_INT =
gl.FLOAT =
gl._2_BYTES =
gl._3_BYTES =
gl._4_BYTES =
gl.DOUBLE =
gl.CLEAR =
gl.AND =
gl.AND_REVERSE =
gl.COPY =
gl.AND_INVERTED =
gl.NOOP =
gl.XOR =
gl.OR =
gl.NOR =
gl.EQUIV =
gl.INVERT =
gl.OR_REVERSE =
gl.COPY_INVERTED =
gl.OR_INVERTED =
gl.NAND =
gl.SET =
gl.EMISSION =
gl.SHININESS =
gl.AMBIENT_AND_DIFFUSE =
gl.COLOR_INDEXES =
gl.MODELVIEW =
gl.PROJECTION =
gl.TEXTURE =
gl.COLOR =
gl.DEPTH =
gl.STENCIL =
gl.COLOR_INDEX =
gl.STENCIL_INDEX =
gl.DEPTH_COMPONENT =
gl.RED =
gl.GREEN =
gl.BLUE =
gl.ALPHA =
gl.RGB =
gl.RGBA =
gl.LUMINANCE =
gl.LUMINANCE_ALPHA =
gl.BITMAP =
gl.POINT =
gl.LINE =
gl.FILL =
gl.RENDER =
gl.FEEDBACK =
gl.SELECT =
gl.FLAT =
gl.SMOOTH =
gl.KEEP =
gl.REPLACE =
gl.INCR =
gl.DECR =
gl.VENDOR =
gl.RENDERER =
gl.VERSION =
gl.EXTENSIONS =
gl.S =
gl.ENABLE_BIT =
gl.T =
gl.R =
gl.Q =
gl.MODULATE =
gl.DECAL =
gl.TEXTURE_ENV_MODE =
gl.TEXTURE_ENV_COLOR =
gl.TEXTURE_ENV =
gl.EYE_LINEAR =
gl.OBJECT_LINEAR =
gl.SPHERE_MAP =
gl.TEXTURE_GEN_MODE =
gl.OBJECT_PLANE =
gl.EYE_PLANE =
gl.NEAREST =
gl.LINEAR =
gl.NEAREST_MIPMAP_NEAREST =
gl.LINEAR_MIPMAP_NEAREST =
gl.NEAREST_MIPMAP_LINEAR =
gl.LINEAR_MIPMAP_LINEAR =
gl.TEXTURE_MAG_FILTER =
gl.TEXTURE_MIN_FILTER =
gl.TEXTURE_WRAP_S =
gl.TEXTURE_WRAP_T =
gl.CLAMP =
gl.REPEAT =
gl.POLYGON_OFFSET_UNITS =
gl.POLYGON_OFFSET_POINT =
gl.POLYGON_OFFSET_LINE =
gl.R3_G3_B2 =
gl.V2F =
gl.V3F =
gl.C4UB_V2F =
gl.C4UB_V3F =
gl.C3F_V3F =
gl.N3F_V3F =
gl.C4F_N3F_V3F =
gl.T2F_V3F =
gl.T4F_V4F =
gl.T2F_C4UB_V3F =
gl.T2F_C3F_V3F =
gl.T2F_N3F_V3F =
gl.T2F_C4F_N3F_V3F =
gl.T4F_C4F_N3F_V4F =
gl.CLIP_PLANE0 =
gl.CLIP_PLANE1 =
gl.CLIP_PLANE2 =
gl.CLIP_PLANE3 =
gl.CLIP_PLANE4 =
gl.CLIP_PLANE5 =
gl.LIGHT0 =
gl.COLOR_BUFFER_BIT =
gl.LIGHT1 =
gl.LIGHT2 =
gl.LIGHT3 =
gl.LIGHT4 =
gl.LIGHT5 =
gl.LIGHT6 =
gl.LIGHT7 =
gl.HINT_BIT =
gl.POLYGON_OFFSET_FILL =
gl.POLYGON_OFFSET_FACTOR =
gl.ALPHA4 =
gl.ALPHA8 =
gl.ALPHA12 =
gl.ALPHA16 =
gl.LUMINANCE4 =
gl.LUMINANCE8 =
gl.LUMINANCE12 =
gl.LUMINANCE16 =
gl.LUMINANCE4_ALPHA4 =
gl.LUMINANCE6_ALPHA2 =
gl.LUMINANCE8_ALPHA8 =
gl.LUMINANCE12_ALPHA4 =
gl.LUMINANCE12_ALPHA12 =
gl.LUMINANCE16_ALPHA16 =
gl.INTENSITY =
gl.INTENSITY4 =
gl.INTENSITY8 =
gl.INTENSITY12 =
gl.INTENSITY16 =
gl.RGB4 =
gl.RGB5 =
gl.RGB8 =
gl.RGB10 =
gl.RGB12 =
gl.RGB16 =
gl.RGBA2 =
gl.RGBA4 =
gl.RGB5_A1 =
gl.RGBA8 =
gl.RGB10_A2 =
gl.RGBA12 =
gl.RGBA16 =
gl.TEXTURE_RED_SIZE =
gl.TEXTURE_GREEN_SIZE =
gl.TEXTURE_BLUE_SIZE =
gl.TEXTURE_ALPHA_SIZE =
gl.TEXTURE_LUMINANCE_SIZE =
gl.TEXTURE_INTENSITY_SIZE =
gl.PROXY_TEXTURE_1D =
gl.PROXY_TEXTURE_2D =
gl.TEXTURE_PRIORITY =
gl.TEXTURE_RESIDENT =
gl.TEXTURE_BINDING_1D =
gl.TEXTURE_BINDING_2D =
gl.VERTEX_ARRAY =
gl.NORMAL_ARRAY =
gl.COLOR_ARRAY =
gl.INDEX_ARRAY =
gl.TEXTURE_COORD_ARRAY =
gl.EDGE_FLAG_ARRAY =
gl.VERTEX_ARRAY_SIZE =
gl.VERTEX_ARRAY_TYPE =
gl.VERTEX_ARRAY_STRIDE =
gl.NORMAL_ARRAY_TYPE =
gl.NORMAL_ARRAY_STRIDE =
gl.COLOR_ARRAY_SIZE =
gl.COLOR_ARRAY_TYPE =
gl.COLOR_ARRAY_STRIDE =
gl.INDEX_ARRAY_TYPE =
gl.INDEX_ARRAY_STRIDE =
gl.TEXTURE_COORD_ARRAY_SIZE =
gl.TEXTURE_COORD_ARRAY_TYPE =
gl.TEXTURE_COORD_ARRAY_STRIDE =
gl.EDGE_FLAG_ARRAY_STRIDE =
gl.VERTEX_ARRAY_POINTER =
gl.NORMAL_ARRAY_POINTER =
gl.COLOR_ARRAY_POINTER =
gl.INDEX_ARRAY_POINTER =
gl.TEXTURE_COORD_ARRAY_POINTER =
gl.EDGE_FLAG_ARRAY_POINTER =
gl.COLOR_INDEX1_EXT =
gl.COLOR_INDEX2_EXT =
gl.COLOR_INDEX4_EXT =
gl.COLOR_INDEX8_EXT =
gl.COLOR_INDEX12_EXT =
gl.COLOR_INDEX16_EXT =
gl.EVAL_BIT =
gl.LIST_BIT =
gl.TEXTURE_BIT =
gl.SCISSOR_BIT =
gl.ALL_ATTRIB_BITS =
gl.CLIENT_ALL_ATTRIB_BITS = null;

gl.SMOOTH_POINT_SIZE_RANGE =
gl.SMOOTH_POINT_SIZE_GRANULARITY =
gl.SMOOTH_LINE_WIDTH_RANGE =
gl.SMOOTH_LINE_WIDTH_GRANULARITY =
gl.UNSIGNED_BYTE_3_3_2 =
gl.UNSIGNED_SHORT_4_4_4_4 =
gl.UNSIGNED_SHORT_5_5_5_1 =
gl.UNSIGNED_INT_8_8_8_8 =
gl.UNSIGNED_INT_10_10_10_2 =
gl.RESCALE_NORMAL =
gl.TEXTURE_BINDING_3D =
gl.PACK_SKIP_IMAGES =
gl.PACK_IMAGE_HEIGHT =
gl.UNPACK_SKIP_IMAGES =
gl.UNPACK_IMAGE_HEIGHT =
gl.TEXTURE_3D =
gl.PROXY_TEXTURE_3D =
gl.TEXTURE_DEPTH =
gl.TEXTURE_WRAP_R =
gl.MAX_3D_TEXTURE_SIZE =
gl.BGR =
gl.BGRA =
gl.MAX_ELEMENTS_VERTICES =
gl.MAX_ELEMENTS_INDICES =
gl.CLAMP_TO_EDGE =
gl.TEXTURE_MIN_LOD =
gl.TEXTURE_MAX_LOD =
gl.TEXTURE_BASE_LEVEL =
gl.TEXTURE_MAX_LEVEL =
gl.LIGHT_MODEL_COLOR_CONTROL =
gl.SINGLE_COLOR =
gl.SEPARATE_SPECULAR_COLOR =
gl.UNSIGNED_BYTE_2_3_3_REV =
gl.UNSIGNED_SHORT_5_6_5 =
gl.UNSIGNED_SHORT_5_6_5_REV =
gl.UNSIGNED_SHORT_4_4_4_4_REV =
gl.UNSIGNED_SHORT_1_5_5_5_REV =
gl.UNSIGNED_INT_8_8_8_8_REV =
gl.ALIASED_POINT_SIZE_RANGE =
gl.ALIASED_LINE_WIDTH_RANGE = null;

gl.MULTISAMPLE =
gl.SAMPLE_ALPHA_TO_COVERAGE =
gl.SAMPLE_ALPHA_TO_ONE =
gl.SAMPLE_COVERAGE =
gl.SAMPLE_BUFFERS =
gl.SAMPLES =
gl.SAMPLE_COVERAGE_VALUE =
gl.SAMPLE_COVERAGE_INVERT =
gl.CLAMP_TO_BORDER =
gl.TEXTURE0 =
gl.TEXTURE1 =
gl.TEXTURE2 =
gl.TEXTURE3 =
gl.TEXTURE4 =
gl.TEXTURE5 =
gl.TEXTURE6 =
gl.TEXTURE7 =
gl.TEXTURE8 =
gl.TEXTURE9 =
gl.TEXTURE10 =
gl.TEXTURE11 =
gl.TEXTURE12 =
gl.TEXTURE13 =
gl.TEXTURE14 =
gl.TEXTURE15 =
gl.TEXTURE16 =
gl.TEXTURE17 =
gl.TEXTURE18 =
gl.TEXTURE19 =
gl.TEXTURE20 =
gl.TEXTURE21 =
gl.TEXTURE22 =
gl.TEXTURE23 =
gl.TEXTURE24 =
gl.TEXTURE25 =
gl.TEXTURE26 =
gl.TEXTURE27 =
gl.TEXTURE28 =
gl.TEXTURE29 =
gl.TEXTURE30 =
gl.TEXTURE31 =
gl.ACTIVE_TEXTURE =
gl.CLIENT_ACTIVE_TEXTURE =
gl.MAX_TEXTURE_UNITS =
gl.TRANSPOSE_MODELVIEW_MATRIX =
gl.TRANSPOSE_PROJECTION_MATRIX =
gl.TRANSPOSE_TEXTURE_MATRIX =
gl.TRANSPOSE_COLOR_MATRIX =
gl.SUBTRACT =
gl.COMPRESSED_ALPHA =
gl.COMPRESSED_LUMINANCE =
gl.COMPRESSED_LUMINANCE_ALPHA =
gl.COMPRESSED_INTENSITY =
gl.COMPRESSED_RGB =
gl.COMPRESSED_RGBA =
gl.TEXTURE_COMPRESSION_HINT =
gl.NORMAL_MAP =
gl.REFLECTION_MAP =
gl.TEXTURE_CUBE_MAP =
gl.TEXTURE_BINDING_CUBE_MAP =
gl.TEXTURE_CUBE_MAP_POSITIVE_X =
gl.TEXTURE_CUBE_MAP_NEGATIVE_X =
gl.TEXTURE_CUBE_MAP_POSITIVE_Y =
gl.TEXTURE_CUBE_MAP_NEGATIVE_Y =
gl.TEXTURE_CUBE_MAP_POSITIVE_Z =
gl.TEXTURE_CUBE_MAP_NEGATIVE_Z =
gl.PROXY_TEXTURE_CUBE_MAP =
gl.MAX_CUBE_MAP_TEXTURE_SIZE =
gl.COMBINE =
gl.COMBINE_RGB =
gl.COMBINE_ALPHA =
gl.RGB_SCALE =
gl.ADD_SIGNED =
gl.INTERPOLATE =
gl.CONSTANT =
gl.PRIMARY_COLOR =
gl.PREVIOUS =
gl.SOURCE0_RGB =
gl.SOURCE1_RGB =
gl.SOURCE2_RGB =
gl.SOURCE0_ALPHA =
gl.SOURCE1_ALPHA =
gl.SOURCE2_ALPHA =
gl.OPERAND0_RGB =
gl.OPERAND1_RGB =
gl.OPERAND2_RGB =
gl.OPERAND0_ALPHA =
gl.OPERAND1_ALPHA =
gl.OPERAND2_ALPHA =
gl.TEXTURE_COMPRESSED_IMAGE_SIZE =
gl.TEXTURE_COMPRESSED =
gl.NUM_COMPRESSED_TEXTURE_FORMATS =
gl.COMPRESSED_TEXTURE_FORMATS =
gl.DOT3_RGB =
gl.DOT3_RGBA =
gl.MULTISAMPLE_BIT = null;

gl.BLEND_DST_RGB =
gl.BLEND_SRC_RGB =
gl.BLEND_DST_ALPHA =
gl.BLEND_SRC_ALPHA =
gl.POINT_SIZE_MIN =
gl.POINT_SIZE_MAX =
gl.POINT_FADE_THRESHOLD_SIZE =
gl.POINT_DISTANCE_ATTENUATION =
gl.GENERATE_MIPMAP =
gl.GENERATE_MIPMAP_HINT =
gl.DEPTH_COMPONENT16 =
gl.DEPTH_COMPONENT24 =
gl.DEPTH_COMPONENT32 =
gl.MIRRORED_REPEAT =
gl.FOG_COORDINATE_SOURCE =
gl.FOG_COORDINATE =
gl.FRAGMENT_DEPTH =
gl.CURRENT_FOG_COORDINATE =
gl.FOG_COORDINATE_ARRAY_TYPE =
gl.FOG_COORDINATE_ARRAY_STRIDE =
gl.FOG_COORDINATE_ARRAY_POINTER =
gl.FOG_COORDINATE_ARRAY =
gl.COLOR_SUM =
gl.CURRENT_SECONDARY_COLOR =
gl.SECONDARY_COLOR_ARRAY_SIZE =
gl.SECONDARY_COLOR_ARRAY_TYPE =
gl.SECONDARY_COLOR_ARRAY_STRIDE =
gl.SECONDARY_COLOR_ARRAY_POINTER =
gl.SECONDARY_COLOR_ARRAY =
gl.MAX_TEXTURE_LOD_BIAS =
gl.TEXTURE_FILTER_CONTROL =
gl.TEXTURE_LOD_BIAS =
gl.INCR_WRAP =
gl.DECR_WRAP =
gl.TEXTURE_DEPTH_SIZE =
gl.DEPTH_TEXTURE_MODE =
gl.TEXTURE_COMPARE_MODE =
gl.TEXTURE_COMPARE_FUNC =
gl.COMPARE_R_TO_TEXTURE = null;

gl.CURRENT_FOG_COORD =
gl.FOG_COORD =
gl.FOG_COORD_ARRAY =
gl.FOG_COORD_ARRAY_BUFFER_BINDING =
gl.FOG_COORD_ARRAY_POINTER =
gl.FOG_COORD_ARRAY_STRIDE =
gl.FOG_COORD_ARRAY_TYPE =
gl.FOG_COORD_SRC =
gl.SRC0_ALPHA =
gl.SRC0_RGB =
gl.SRC1_ALPHA =
gl.SRC1_RGB =
gl.SRC2_ALPHA =
gl.SRC2_RGB =
gl.BUFFER_SIZE =
gl.BUFFER_USAGE =
gl.QUERY_COUNTER_BITS =
gl.CURRENT_QUERY =
gl.QUERY_RESULT =
gl.QUERY_RESULT_AVAILABLE =
gl.ARRAY_BUFFER =
gl.ELEMENT_ARRAY_BUFFER =
gl.ARRAY_BUFFER_BINDING =
gl.ELEMENT_ARRAY_BUFFER_BINDING =
gl.VERTEX_ARRAY_BUFFER_BINDING =
gl.NORMAL_ARRAY_BUFFER_BINDING =
gl.COLOR_ARRAY_BUFFER_BINDING =
gl.INDEX_ARRAY_BUFFER_BINDING =
gl.TEXTURE_COORD_ARRAY_BUFFER_BINDING =
gl.EDGE_FLAG_ARRAY_BUFFER_BINDING =
gl.SECONDARY_COLOR_ARRAY_BUFFER_BINDING =
gl.FOG_COORDINATE_ARRAY_BUFFER_BINDING =
gl.WEIGHT_ARRAY_BUFFER_BINDING =
gl.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING =
gl.READ_ONLY =
gl.WRITE_ONLY =
gl.READ_WRITE =
gl.BUFFER_ACCESS =
gl.BUFFER_MAPPED =
gl.BUFFER_MAP_POINTER =
gl.STREAM_DRAW =
gl.STREAM_READ =
gl.STREAM_COPY =
gl.STATIC_DRAW =
gl.STATIC_READ =
gl.STATIC_COPY =
gl.DYNAMIC_DRAW =
gl.DYNAMIC_READ =
gl.DYNAMIC_COPY =
gl.SAMPLES_PASSED = null;

gl.BLEND_EQUATION_RGB =
gl.VERTEX_ATTRIB_ARRAY_ENABLED =
gl.VERTEX_ATTRIB_ARRAY_SIZE =
gl.VERTEX_ATTRIB_ARRAY_STRIDE =
gl.VERTEX_ATTRIB_ARRAY_TYPE =
gl.CURRENT_VERTEX_ATTRIB =
gl.VERTEX_PROGRAM_POINT_SIZE =
gl.VERTEX_PROGRAM_TWO_SIDE =
gl.VERTEX_ATTRIB_ARRAY_POINTER =
gl.STENCIL_BACK_FUNC =
gl.STENCIL_BACK_FAIL =
gl.STENCIL_BACK_PASS_DEPTH_FAIL =
gl.STENCIL_BACK_PASS_DEPTH_PASS =
gl.MAX_DRAW_BUFFERS =
gl.DRAW_BUFFER0 =
gl.DRAW_BUFFER1 =
gl.DRAW_BUFFER2 =
gl.DRAW_BUFFER3 =
gl.DRAW_BUFFER4 =
gl.DRAW_BUFFER5 =
gl.DRAW_BUFFER6 =
gl.DRAW_BUFFER7 =
gl.DRAW_BUFFER8 =
gl.DRAW_BUFFER9 =
gl.DRAW_BUFFER10 =
gl.DRAW_BUFFER11 =
gl.DRAW_BUFFER12 =
gl.DRAW_BUFFER13 =
gl.DRAW_BUFFER14 =
gl.DRAW_BUFFER15 =
gl.BLEND_EQUATION_ALPHA =
gl.POINT_SPRITE =
gl.COORD_REPLACE =
gl.MAX_VERTEX_ATTRIBS =
gl.VERTEX_ATTRIB_ARRAY_NORMALIZED =
gl.MAX_TEXTURE_COORDS =
gl.MAX_TEXTURE_IMAGE_UNITS =
gl.FRAGMENT_SHADER =
gl.VERTEX_SHADER =
gl.MAX_FRAGMENT_UNIFORM_COMPONENTS =
gl.MAX_VERTEX_UNIFORM_COMPONENTS =
gl.MAX_VARYING_FLOATS =
gl.MAX_VERTEX_TEXTURE_IMAGE_UNITS =
gl.MAX_COMBINED_TEXTURE_IMAGE_UNITS =
gl.SHADER_TYPE =
gl.FLOAT_VEC2 =
gl.FLOAT_VEC3 =
gl.FLOAT_VEC4 =
gl.INT_VEC2 =
gl.INT_VEC3 =
gl.INT_VEC4 =
gl.BOOL =
gl.BOOL_VEC2 =
gl.BOOL_VEC3 =
gl.BOOL_VEC4 =
gl.FLOAT_MAT2 =
gl.FLOAT_MAT3 =
gl.FLOAT_MAT4 =
gl.SAMPLER_1D =
gl.SAMPLER_2D =
gl.SAMPLER_3D =
gl.SAMPLER_CUBE =
gl.SAMPLER_1D_SHADOW =
gl.SAMPLER_2D_SHADOW =
gl.DELETE_STATUS =
gl.COMPILE_STATUS =
gl.LINK_STATUS =
gl.VALIDATE_STATUS =
gl.INFO_LOG_LENGTH =
gl.ATTACHED_SHADERS =
gl.ACTIVE_UNIFORMS =
gl.ACTIVE_UNIFORM_MAX_LENGTH =
gl.SHADER_SOURCE_LENGTH =
gl.ACTIVE_ATTRIBUTES =
gl.ACTIVE_ATTRIBUTE_MAX_LENGTH =
gl.FRAGMENT_SHADER_DERIVATIVE_HINT =
gl.SHADING_LANGUAGE_VERSION =
gl.CURRENT_PROGRAM =
gl.POINT_SPRITE_COORD_ORIGIN =
gl.LOWER_LEFT =
gl.UPPER_LEFT =
gl.STENCIL_BACK_REF =
gl.STENCIL_BACK_VALUE_MASK =
gl.STENCIL_BACK_WRITEMASK = null;

gl.CURRENT_RASTER_SECONDARY_COLOR =
gl.PIXEL_PACK_BUFFER =
gl.PIXEL_UNPACK_BUFFER =
gl.PIXEL_PACK_BUFFER_BINDING =
gl.PIXEL_UNPACK_BUFFER_BINDING =
gl.FLOAT_MAT2x3 =
gl.FLOAT_MAT2x4 =
gl.FLOAT_MAT3x2 =
gl.FLOAT_MAT3x4 =
gl.FLOAT_MAT4x2 =
gl.FLOAT_MAT4x3 =
gl.SRGB =
gl.SRGB8 =
gl.SRGB_ALPHA =
gl.SRGB8_ALPHA8 =
gl.SLUMINANCE_ALPHA =
gl.SLUMINANCE8_ALPHA8 =
gl.SLUMINANCE =
gl.SLUMINANCE8 =
gl.COMPRESSED_SRGB =
gl.COMPRESSED_SRGB_ALPHA =
gl.COMPRESSED_SLUMINANCE =
gl.COMPRESSED_SLUMINANCE_ALPHA = null;

gl.CLIP_DISTANCE0 =
gl.CLIP_DISTANCE1 =
gl.CLIP_DISTANCE2 =
gl.CLIP_DISTANCE3 =
gl.CLIP_DISTANCE4 =
//    gl.CLIP_DISTANCE =
gl.COMPARE_REF_TO_TEXTURE =
gl.MAX_CLIP_DISTANCES =
gl.MAX_VARYING_COMPONENTS = null;


gl.CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT =
gl.MAJOR_VERSION =
gl.MINOR_VERSION =
gl.NUM_EXTENSIONS =
gl.CONTEXT_FLAGS =
gl.DEPTH_BUFFER =
gl.STENCIL_BUFFER =
gl.RGBA32F =
gl.RGB32F =
gl.RGBA16F =
gl.RGB16F =
gl.VERTEX_ATTRIB_ARRAY_INTEGER =
gl.MAX_ARRAY_TEXTURE_LAYERS =
gl.MIN_PROGRAM_TEXEL_OFFSET =
gl.MAX_PROGRAM_TEXEL_OFFSET =
gl.CLAMP_VERTEX_COLOR =
gl.CLAMP_FRAGMENT_COLOR =
gl.CLAMP_READ_COLOR =
gl.FIXED_ONLY =
gl.TEXTURE_RED_TYPE =
gl.TEXTURE_GREEN_TYPE =
gl.TEXTURE_BLUE_TYPE =
gl.TEXTURE_ALPHA_TYPE =
gl.TEXTURE_LUMINANCE_TYPE =
gl.TEXTURE_INTENSITY_TYPE =
gl.TEXTURE_DEPTH_TYPE =
gl.TEXTURE_1D_ARRAY =
gl.PROXY_TEXTURE_1D_ARRAY =
gl.TEXTURE_2D_ARRAY =
gl.PROXY_TEXTURE_2D_ARRAY =
gl.TEXTURE_BINDING_1D_ARRAY =
gl.TEXTURE_BINDING_2D_ARRAY =
gl.R11F_G11F_B10F =
gl.UNSIGNED_INT_10F_11F_11F_REV =
gl.RGB9_E5 =
gl.UNSIGNED_INT_5_9_9_9_REV =
gl.TEXTURE_SHARED_SIZE =
gl.TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH =
gl.TRANSFORM_FEEDBACK_BUFFER_MODE =
gl.MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS =
gl.TRANSFORM_FEEDBACK_VARYINGS =
gl.TRANSFORM_FEEDBACK_BUFFER_START =
gl.TRANSFORM_FEEDBACK_BUFFER_SIZE =
gl.PRIMITIVES_GENERATED =
gl.TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN =
gl.RASTERIZER_DISCARD =
gl.MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS =
gl.MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS =
gl.INTERLEAVED_ATTRIBS =
gl.SEPARATE_ATTRIBS =
gl.TRANSFORM_FEEDBACK_BUFFER =
gl.TRANSFORM_FEEDBACK_BUFFER_BINDING =
gl.RGBA32UI =
gl.RGB32UI =
gl.RGBA16UI =
gl.RGB16UI =
gl.RGBA8UI =
gl.RGB8UI =
gl.RGBA32I =
gl.RGB32I =
gl.RGBA16I =
gl.RGB16I =
gl.RGBA8I =
gl.RGB8I =
gl.RED_INTEGER =
gl.GREEN_INTEGER =
gl.BLUE_INTEGER =
gl.ALPHA_INTEGER =
gl.RGB_INTEGER =
gl.RGBA_INTEGER =
gl.BGR_INTEGER =
gl.BGRA_INTEGER =
gl.SAMPLER_1D_ARRAY =
gl.SAMPLER_2D_ARRAY =
gl.SAMPLER_1D_ARRAY_SHADOW =
gl.SAMPLER_2D_ARRAY_SHADOW =
gl.SAMPLER_CUBE_SHADOW =
gl.UNSIGNED_INT_VEC2 =
gl.UNSIGNED_INT_VEC3 =
gl.UNSIGNED_INT_VEC4 =
gl.INT_SAMPLER_1D =
gl.INT_SAMPLER_2D =
gl.INT_SAMPLER_3D =
gl.INT_SAMPLER_CUBE =
gl.INT_SAMPLER_1D_ARRAY =
gl.INT_SAMPLER_2D_ARRAY =
gl.UNSIGNED_INT_SAMPLER_1D =
gl.UNSIGNED_INT_SAMPLER_2D =
gl.UNSIGNED_INT_SAMPLER_3D =
gl.UNSIGNED_INT_SAMPLER_CUBE =
gl.UNSIGNED_INT_SAMPLER_1D_ARRAY =
gl.UNSIGNED_INT_SAMPLER_2D_ARRAY =
gl.QUERY_WAIT =
gl.QUERY_NO_WAIT =
gl.QUERY_BY_REGION_WAIT =
gl.QUERY_BY_REGION_NO_WAIT = null;

gl.TEXTURE_RECTANGLE =
gl.TEXTURE_BINDING_RECTANGLE =
gl.PROXY_TEXTURE_RECTANGLE =
gl.MAX_RECTANGLE_TEXTURE_SIZE =
gl.SAMPLER_2D_RECT =
gl.SAMPLER_2D_RECT_SHADOW =
gl.TEXTURE_BUFFER =
gl.MAX_TEXTURE_BUFFER_SIZE =
gl.TEXTURE_BINDING_BUFFER =
gl.TEXTURE_BUFFER_DATA_STORE_BINDING =
gl.TEXTURE_BUFFER_FORMAT =
gl.SAMPLER_BUFFER =
gl.INT_SAMPLER_2D_RECT =
gl.INT_SAMPLER_BUFFER =
gl.UNSIGNED_INT_SAMPLER_2D_RECT =
gl.UNSIGNED_INT_SAMPLER_BUFFER =
gl.RED_SNORM =
gl.RG_SNORM =
gl.RGB_SNORM =
gl.RGBA_SNORM =
gl.R8_SNORM =
gl.RG8_SNORM =
gl.RGB8_SNORM =
gl.RGBA8_SNORM =
gl.R16_SNORM =
gl.RG16_SNORM =
gl.RGB16_SNORM =
gl.RGBA16_SNORM =
gl.SIGNED_NORMALIZED =
gl.PRIMITIVE_RESTART =
gl.PRIMITIVE_RESTART_INDEX =
gl.BUFFER_ACCESS_FLAGS =
gl.BUFFER_MAP_LENGTH =
gl.BUFFER_MAP_OFFSET = null;

gl.CONTEXT_CORE_PROFILE_BIT =
gl.CONTEXT_COMPATIBILITY_PROFILE_BIT =
gl.LINES_ADJACENCY =
gl.LINE_STRIP_ADJACENCY =
gl.TRIANGLES_ADJACENCY =
gl.TRIANGLE_STRIP_ADJACENCY =
gl.PROGRAM_POINT_SIZE =
gl.GEOMETRY_VERTICES_OUT =
gl.GEOMETRY_INPUT_TYPE =
gl.GEOMETRY_OUTPUT_TYPE =
gl.MAX_GEOMETRY_TEXTURE_IMAGE_UNITS =
gl.FRAMEBUFFER_ATTACHMENT_LAYERED =
gl.FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS =
gl.GEOMETRY_SHADER =
gl.MAX_GEOMETRY_UNIFORM_COMPONENTS =
gl.MAX_GEOMETRY_OUTPUT_VERTICES =
gl.MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS =
gl.MAX_VERTEX_OUTPUT_COMPONENTS =
gl.MAX_GEOMETRY_INPUT_COMPONENTS =
gl.MAX_GEOMETRY_OUTPUT_COMPONENTS =
gl.MAX_FRAGMENT_INPUT_COMPONENTS =
gl.CONTEXT_PROFILE_MASK = null;

gl.SAMPLE_SHADING =
gl.MIN_SAMPLE_SHADING_VALUE =
gl.MIN_PROGRAM_TEXTURE_GATHER_OFFSET =
gl.MAX_PROGRAM_TEXTURE_GATHER_OFFSET =
gl.MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS =
gl.TEXTURE_CUBE_MAP_ARRAY =
gl.TEXTURE_BINDING_CUBE_MAP_ARRAY =
gl.PROXY_TEXTURE_CUBE_MAP_ARRAY =
gl.SAMPLER_CUBE_MAP_ARRAY =
gl.SAMPLER_CUBE_MAP_ARRAY_SHADOW =
gl.INT_SAMPLER_CUBE_MAP_ARRAY =
gl.UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY = null;

gl.COMPRESSED_RGBA_BPTC_UNORM =
gl.COMPRESSED_SRGB_ALPHA_BPTC_UNORM =
gl.COMPRESSED_RGB_BPTC_SIGNED_FLOAT =
gl.COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = null;

gl.NUM_SHADING_LANGUAGE_VERSIONS =
gl.VERTEX_ATTRIB_ARRAY_LONG = null;

gl.PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED =
gl.MAX_VERTEX_ATTRIB_STRIDE =
gl.TEXTURE_BUFFER_BINDING = null;

gl.MULTISAMPLE_3DFX =
gl.SAMPLE_BUFFERS_3DFX =
gl.SAMPLES_3DFX =
gl.MULTISAMPLE_BIT_3DFX = null;

gl.COMPRESSED_RGB_FXT1_3DFX =
gl.COMPRESSED_RGBA_FXT1_3DFX = null;

gl.MAX_DEBUG_MESSAGE_LENGTH_AMD =
gl.MAX_DEBUG_LOGGED_MESSAGES_AMD =
gl.DEBUG_LOGGED_MESSAGES_AMD =
gl.DEBUG_SEVERITY_HIGH_AMD =
gl.DEBUG_SEVERITY_MEDIUM_AMD =
gl.DEBUG_SEVERITY_LOW_AMD =
gl.DEBUG_CATEGORY_API_ERROR_AMD =
gl.DEBUG_CATEGORY_WINDOW_SYSTEM_AMD =
gl.DEBUG_CATEGORY_DEPRECATION_AMD =
gl.DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD =
gl.DEBUG_CATEGORY_PERFORMANCE_AMD =
gl.DEBUG_CATEGORY_SHADER_COMPILER_AMD =
gl.DEBUG_CATEGORY_APPLICATION_AMD =
gl.DEBUG_CATEGORY_OTHER_AMD = null;

gl.DRAW_FRAMEBUFFER_BINDING_ANGLE =
gl.READ_FRAMEBUFFER_ANGLE =
gl.DRAW_FRAMEBUFFER_ANGLE =
gl.READ_FRAMEBUFFER_BINDING_ANGLE = null;

gl.RENDERBUFFER_SAMPLES_ANGLE =
gl.FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_ANGLE =
gl.MAX_SAMPLES_ANGLE = null;

gl.VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE = null;

gl.PACK_REVERSE_ROW_ORDER_ANGLE = null;

gl.PROGRAM_BINARY_ANGLE = null;

gl.COMPRESSED_RGB_S3TC_DXT1_ANGLE =
gl.COMPRESSED_RGBA_S3TC_DXT1_ANGLE =
gl.COMPRESSED_RGBA_S3TC_DXT3_ANGLE =
gl.COMPRESSED_RGBA_S3TC_DXT5_ANGLE = null;

gl.TEXTURE_USAGE_ANGLE =
gl.FRAMEBUFFER_ATTACHMENT_ANGLE = null;

gl.QUERY_COUNTER_BITS_ANGLE =
gl.CURRENT_QUERY_ANGLE =
gl.QUERY_RESULT_ANGLE =
gl.QUERY_RESULT_AVAILABLE_ANGLE =
gl.TIME_ELAPSED_ANGLE =
gl.TIMESTAMP_ANGLE = null;

gl.TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE = null;

gl.AUX_DEPTH_STENCIL_APPLE = null;

gl.UNPACK_CLIENT_STORAGE_APPLE = null;

gl.ELEMENT_ARRAY_APPLE =
gl.ELEMENT_ARRAY_TYPE_APPLE =
gl.ELEMENT_ARRAY_POINTER_APPLE = null;

gl.DRAW_PIXELS_APPLE =
gl.FENCE_APPLE = null;

gl.HALF_APPLE =
gl.RGBA_FLOAT32_APPLE =
gl.RGB_FLOAT32_APPLE =
gl.ALPHA_FLOAT32_APPLE =
gl.INTENSITY_FLOAT32_APPLE =
gl.LUMINANCE_FLOAT32_APPLE =
gl.LUMINANCE_ALPHA_FLOAT32_APPLE =
gl.RGBA_FLOAT16_APPLE =
gl.RGB_FLOAT16_APPLE =
gl.ALPHA_FLOAT16_APPLE =
gl.INTENSITY_FLOAT16_APPLE =
gl.LUMINANCE_FLOAT16_APPLE =
gl.LUMINANCE_ALPHA_FLOAT16_APPLE =
gl.COLOR_FLOAT_APPLE = null;

gl.BUFFER_SERIALIZED_MODIFY_APPLE =
gl.BUFFER_FLUSHING_UNMAP_APPLE = null;

gl.BUFFER_OBJECT_APPLE =
gl.RELEASED_APPLE =
gl.VOLATILE_APPLE =
gl.RETAINED_APPLE =
gl.UNDEFINED_APPLE =
gl.PURGEABLE_APPLE = null;

gl.MIN_PBUFFER_VIEWPORT_DIMS_APPLE = null;

gl.UNSIGNED_SHORT_8_8_APPLE =
gl.UNSIGNED_SHORT_8_8_REV_APPLE =
gl.RGB_422_APPLE =
gl.RGB_RAW_422_APPLE = null;

gl.PACK_ROW_BYTES_APPLE =
gl.UNPACK_ROW_BYTES_APPLE = null;

gl.LIGHT_MODEL_SPECULAR_VECTOR_APPLE = null;

gl.TEXTURE_RANGE_LENGTH_APPLE =
gl.TEXTURE_RANGE_POINTER_APPLE =
gl.TEXTURE_STORAGE_HINT_APPLE =
gl.STORAGE_PRIVATE_APPLE =
gl.STORAGE_CACHED_APPLE =
gl.STORAGE_SHARED_APPLE = null;

gl.TRANSFORM_HINT_APPLE = null;

gl.VERTEX_ARRAY_BINDING_APPLE = null;

gl.VERTEX_ARRAY_RANGE_APPLE =
gl.VERTEX_ARRAY_RANGE_LENGTH_APPLE =
gl.VERTEX_ARRAY_STORAGE_HINT_APPLE =
gl.MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE =
gl.VERTEX_ARRAY_RANGE_POINTER_APPLE =
gl.STORAGE_CLIENT_APPLE = null;

gl.VERTEX_ATTRIB_MAP1_APPLE =
gl.VERTEX_ATTRIB_MAP2_APPLE =
gl.VERTEX_ATTRIB_MAP1_SIZE_APPLE =
gl.VERTEX_ATTRIB_MAP1_COEFF_APPLE =
gl.VERTEX_ATTRIB_MAP1_ORDER_APPLE =
gl.VERTEX_ATTRIB_MAP1_DOMAIN_APPLE =
gl.VERTEX_ATTRIB_MAP2_SIZE_APPLE =
gl.VERTEX_ATTRIB_MAP2_COEFF_APPLE =
gl.VERTEX_ATTRIB_MAP2_ORDER_APPLE =
gl.VERTEX_ATTRIB_MAP2_DOMAIN_APPLE = null;

gl.YCBCR_422_APPLE = null;

gl.FIXED =
gl.IMPLEMENTATION_COLOR_READ_TYPE =
gl.IMPLEMENTATION_COLOR_READ_FORMAT =
gl.RGB565 =
gl.LOW_FLOAT =
gl.MEDIUM_FLOAT =
gl.HIGH_FLOAT =
gl.LOW_INT =
gl.MEDIUM_INT =
gl.HIGH_INT =
gl.SHADER_BINARY_FORMATS =
gl.NUM_SHADER_BINARY_FORMATS =
gl.SHADER_COMPILER =
gl.MAX_VERTEX_UNIFORM_VECTORS =
gl.MAX_VARYING_VECTORS =
gl.MAX_FRAGMENT_UNIFORM_VECTORS = null;

gl.TEXTURE_IMMUTABLE_LEVELS =
gl.PRIMITIVE_RESTART_FIXED_INDEX =
gl.ANY_SAMPLES_PASSED_CONSERVATIVE =
gl.MAX_ELEMENT_INDEX =
gl.COMPRESSED_R11_EAC =
gl.COMPRESSED_SIGNED_R11_EAC =
gl.COMPRESSED_RG11_EAC =
gl.COMPRESSED_SIGNED_RG11_EAC =
gl.COMPRESSED_RGB8_ETC2 =
gl.COMPRESSED_SRGB8_ETC2 =
gl.COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 =
gl.COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 =
gl.COMPRESSED_RGBA8_ETC2_EAC =
gl.COMPRESSED_SRGB8_ALPHA8_ETC2_EAC = null;

gl.UNSIGNED_INT64_ARB =

gl.SRC1_COLOR =
gl.ONE_MINUS_SRC1_COLOR =
gl.ONE_MINUS_SRC1_ALPHA =
gl.MAX_DUAL_SOURCE_DRAW_BUFFERS = null;

gl.MAP_READ_BIT =
gl.MAP_WRITE_BIT =
gl.MAP_PERSISTENT_BIT =
gl.MAP_COHERENT_BIT =
gl.DYNAMIC_STORAGE_BIT =
gl.CLIENT_STORAGE_BIT =
gl.CLIENT_MAPPED_BUFFER_BARRIER_BIT =
gl.BUFFER_IMMUTABLE_STORAGE =
gl.BUFFER_STORAGE_FLAGS = null;

gl.SYNC_CL_EVENT_ARB =
gl.SYNC_CL_EVENT_COMPLETE_ARB = null;

gl.CLEAR_TEXTURE = null;

gl.LOWER_LEFT =
gl.UPPER_LEFT =
gl.CLIP_ORIGIN =
gl.CLIP_DEPTH_MODE =
gl.NEGATIVE_ONE_TO_ONE =
gl.ZERO_TO_ONE = null;

gl.RGBA_FLOAT_MODE_ARB =
gl.CLAMP_VERTEX_COLOR_ARB =
gl.CLAMP_FRAGMENT_COLOR_ARB =
gl.CLAMP_READ_COLOR_ARB =
gl.FIXED_ONLY_ARB = null;

gl.UNPACK_COMPRESSED_BLOCK_WIDTH =
gl.UNPACK_COMPRESSED_BLOCK_HEIGHT =
gl.UNPACK_COMPRESSED_BLOCK_DEPTH =
gl.UNPACK_COMPRESSED_BLOCK_SIZE =
gl.PACK_COMPRESSED_BLOCK_WIDTH =
gl.PACK_COMPRESSED_BLOCK_HEIGHT =
gl.PACK_COMPRESSED_BLOCK_DEPTH =
gl.PACK_COMPRESSED_BLOCK_SIZE = null;

gl.COMPUTE_SHADER_BIT =
gl.MAX_COMPUTE_SHARED_MEMORY_SIZE =
gl.MAX_COMPUTE_UNIFORM_COMPONENTS =
gl.MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS =
gl.MAX_COMPUTE_ATOMIC_COUNTERS =
gl.MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS =
gl.COMPUTE_WORK_GROUP_SIZE =
gl.MAX_COMPUTE_WORK_GROUP_INVOCATIONS =
gl.UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER =
gl.DISPATCH_INDIRECT_BUFFER =
gl.DISPATCH_INDIRECT_BUFFER_BINDING =
gl.COMPUTE_SHADER =
gl.MAX_COMPUTE_UNIFORM_BLOCKS =
gl.MAX_COMPUTE_TEXTURE_IMAGE_UNITS =
gl.MAX_COMPUTE_IMAGE_UNIFORMS =
gl.MAX_COMPUTE_WORK_GROUP_COUNT =
gl.MAX_COMPUTE_WORK_GROUP_SIZE = null;

gl.MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB =
gl.MAX_COMPUTE_FIXED_GROUP_SIZE_ARB =
gl.MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB =
gl.MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB = null;

gl.QUERY_WAIT_INVERTED =
gl.QUERY_NO_WAIT_INVERTED =
gl.QUERY_BY_REGION_WAIT_INVERTED =
gl.QUERY_BY_REGION_NO_WAIT_INVERTED = null;

gl.COPY_READ_BUFFER =
gl.COPY_WRITE_BUFFER = null;

gl.MAX_CULL_DISTANCES =
gl.MAX_COMBINED_CLIP_AND_CULL_DISTANCES = null;

gl.DEBUG_OUTPUT_SYNCHRONOUS_ARB =
gl.DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB =
gl.DEBUG_CALLBACK_FUNCTION_ARB =
gl.DEBUG_CALLBACK_USER_PARAM_ARB =
gl.DEBUG_SOURCE_API_ARB =
gl.DEBUG_SOURCE_WINDOW_SYSTEM_ARB =
gl.DEBUG_SOURCE_SHADER_COMPILER_ARB =
gl.DEBUG_SOURCE_THIRD_PARTY_ARB =
gl.DEBUG_SOURCE_APPLICATION_ARB =
gl.DEBUG_SOURCE_OTHER_ARB =
gl.DEBUG_TYPE_ERROR_ARB =
gl.DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB =
gl.DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB =
gl.DEBUG_TYPE_PORTABILITY_ARB =
gl.DEBUG_TYPE_PERFORMANCE_ARB =
gl.DEBUG_TYPE_OTHER_ARB =
gl.MAX_DEBUG_MESSAGE_LENGTH_ARB =
gl.MAX_DEBUG_LOGGED_MESSAGES_ARB =
gl.DEBUG_LOGGED_MESSAGES_ARB =
gl.DEBUG_SEVERITY_HIGH_ARB =
gl.DEBUG_SEVERITY_MEDIUM_ARB =
gl.DEBUG_SEVERITY_LOW_ARB = null;

gl.DEPTH_COMPONENT32F =
gl.DEPTH32F_STENCIL8 =
gl.FLOAT_32_UNSIGNED_INT_24_8_REV = null;

gl.DEPTH_CLAMP = null;

gl.DEPTH_COMPONENT16_ARB =
gl.DEPTH_COMPONENT24_ARB =
gl.DEPTH_COMPONENT32_ARB =
gl.TEXTURE_DEPTH_SIZE_ARB =
gl.DEPTH_TEXTURE_MODE_ARB = null;

gl.TEXTURE_TARGET =
gl.QUERY_TARGET =
gl.TEXTURE_BINDING = null;

gl.MAX_DRAW_BUFFERS_ARB =
gl.DRAW_BUFFER0_ARB =
gl.DRAW_BUFFER1_ARB =
gl.DRAW_BUFFER2_ARB =
gl.DRAW_BUFFER3_ARB =
gl.DRAW_BUFFER4_ARB =
gl.DRAW_BUFFER5_ARB =
gl.DRAW_BUFFER6_ARB =
gl.DRAW_BUFFER7_ARB =
gl.DRAW_BUFFER8_ARB =
gl.DRAW_BUFFER9_ARB =
gl.DRAW_BUFFER10_ARB =
gl.DRAW_BUFFER11_ARB =
gl.DRAW_BUFFER12_ARB =
gl.DRAW_BUFFER13_ARB =
gl.DRAW_BUFFER14_ARB =
gl.DRAW_BUFFER15_ARB = null;

gl.DRAW_INDIRECT_BUFFER =
gl.DRAW_INDIRECT_BUFFER_BINDING = null;

gl.LOCATION_COMPONENT =
gl.TRANSFORM_FEEDBACK_BUFFER_INDEX =
gl.TRANSFORM_FEEDBACK_BUFFER_STRIDE = null;

gl.MAX_UNIFORM_LOCATIONS = null;

gl.FRAGMENT_PROGRAM_ARB =
gl.PROGRAM_ALU_INSTRUCTIONS_ARB =
gl.PROGRAM_TEX_INSTRUCTIONS_ARB =
gl.PROGRAM_TEX_INDIRECTIONS_ARB =
gl.PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB =
gl.PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB =
gl.PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB =
gl.MAX_PROGRAM_ALU_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_TEX_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_TEX_INDIRECTIONS_ARB =
gl.MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB =
gl.MAX_TEXTURE_COORDS_ARB =
gl.MAX_TEXTURE_IMAGE_UNITS_ARB = null;

gl.FRAGMENT_SHADER_ARB =
gl.MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB =
gl.FRAGMENT_SHADER_DERIVATIVE_HINT_ARB = null;

gl.FRAMEBUFFER_DEFAULT_WIDTH =
gl.FRAMEBUFFER_DEFAULT_HEIGHT =
gl.FRAMEBUFFER_DEFAULT_LAYERS =
gl.FRAMEBUFFER_DEFAULT_SAMPLES =
gl.FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS =
gl.MAX_FRAMEBUFFER_WIDTH =
gl.MAX_FRAMEBUFFER_HEIGHT =
gl.MAX_FRAMEBUFFER_LAYERS =
gl.MAX_FRAMEBUFFER_SAMPLES = null;

gl.INVALID_FRAMEBUFFER_OPERATION =
gl.FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING =
gl.FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE =
gl.FRAMEBUFFER_ATTACHMENT_RED_SIZE =
gl.FRAMEBUFFER_ATTACHMENT_GREEN_SIZE =
gl.FRAMEBUFFER_ATTACHMENT_BLUE_SIZE =
gl.FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE =
gl.FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE =
gl.FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE =
gl.FRAMEBUFFER_DEFAULT =
gl.FRAMEBUFFER_UNDEFINED =
gl.DEPTH_STENCIL_ATTACHMENT =
gl.INDEX =
gl.MAX_RENDERBUFFER_SIZE =
gl.DEPTH_STENCIL =
gl.UNSIGNED_INT_24_8 =
gl.DEPTH24_STENCIL8 =
gl.TEXTURE_STENCIL_SIZE =
gl.UNSIGNED_NORMALIZED =
gl.SRGB =
gl.DRAW_FRAMEBUFFER_BINDING =
gl.FRAMEBUFFER_BINDING =
gl.RENDERBUFFER_BINDING =
gl.READ_FRAMEBUFFER =
gl.DRAW_FRAMEBUFFER =
gl.READ_FRAMEBUFFER_BINDING =
gl.RENDERBUFFER_SAMPLES =
gl.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE =
gl.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME =
gl.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL =
gl.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE =
gl.FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER =
gl.FRAMEBUFFER_COMPLETE =
gl.FRAMEBUFFER_INCOMPLETE_ATTACHMENT =
gl.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT =
gl.FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER =
gl.FRAMEBUFFER_INCOMPLETE_READ_BUFFER =
gl.FRAMEBUFFER_UNSUPPORTED =
gl.MAX_COLOR_ATTACHMENTS =
gl.COLOR_ATTACHMENT0 =
gl.COLOR_ATTACHMENT1 =
gl.COLOR_ATTACHMENT2 =
gl.COLOR_ATTACHMENT3 =
gl.COLOR_ATTACHMENT4 =
gl.COLOR_ATTACHMENT5 =
gl.COLOR_ATTACHMENT6 =
gl.COLOR_ATTACHMENT7 =
gl.COLOR_ATTACHMENT8 =
gl.COLOR_ATTACHMENT9 =
gl.COLOR_ATTACHMENT10 =
gl.COLOR_ATTACHMENT11 =
gl.COLOR_ATTACHMENT12 =
gl.COLOR_ATTACHMENT13 =
gl.COLOR_ATTACHMENT14 =
gl.COLOR_ATTACHMENT15 =
gl.DEPTH_ATTACHMENT =
gl.STENCIL_ATTACHMENT =
gl.FRAMEBUFFER =
gl.RENDERBUFFER =
gl.RENDERBUFFER_WIDTH =
gl.RENDERBUFFER_HEIGHT =
gl.RENDERBUFFER_INTERNAL_FORMAT =
gl.STENCIL_INDEX1 =
gl.STENCIL_INDEX4 =
gl.STENCIL_INDEX8 =
gl.STENCIL_INDEX16 =
gl.RENDERBUFFER_RED_SIZE =
gl.RENDERBUFFER_GREEN_SIZE =
gl.RENDERBUFFER_BLUE_SIZE =
gl.RENDERBUFFER_ALPHA_SIZE =
gl.RENDERBUFFER_DEPTH_SIZE =
gl.RENDERBUFFER_STENCIL_SIZE =
gl.FRAMEBUFFER_INCOMPLETE_MULTISAMPLE =
gl.MAX_SAMPLES = null;

gl.FRAMEBUFFER_SRGB = null;

gl.LINES_ADJACENCY_ARB =
gl.LINE_STRIP_ADJACENCY_ARB =
gl.TRIANGLES_ADJACENCY_ARB =
gl.TRIANGLE_STRIP_ADJACENCY_ARB =
gl.PROGRAM_POINT_SIZE_ARB =
gl.MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB =
gl.FRAMEBUFFER_ATTACHMENT_LAYERED_ARB =
gl.FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB =
gl.FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB =
gl.GEOMETRY_SHADER_ARB =
gl.GEOMETRY_VERTICES_OUT_ARB =
gl.GEOMETRY_INPUT_TYPE_ARB =
gl.GEOMETRY_OUTPUT_TYPE_ARB =
gl.MAX_GEOMETRY_VARYING_COMPONENTS_ARB =
gl.MAX_VERTEX_VARYING_COMPONENTS_ARB =
gl.MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB =
gl.MAX_GEOMETRY_OUTPUT_VERTICES_ARB =
gl.MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB = null;

gl.PROGRAM_BINARY_RETRIEVABLE_HINT =
gl.PROGRAM_BINARY_LENGTH =
gl.NUM_PROGRAM_BINARY_FORMATS =
gl.PROGRAM_BINARY_FORMATS = null;

gl.GEOMETRY_SHADER_INVOCATIONS =
gl.MAX_GEOMETRY_SHADER_INVOCATIONS =
gl.MIN_FRAGMENT_INTERPOLATION_OFFSET =
gl.MAX_FRAGMENT_INTERPOLATION_OFFSET =
gl.FRAGMENT_INTERPOLATION_OFFSET_BITS =
gl.MAX_VERTEX_STREAMS = null;

gl.DOUBLE_MAT2 =
gl.DOUBLE_MAT3 =
gl.DOUBLE_MAT4 =
gl.DOUBLE_MAT2x3 =
gl.DOUBLE_MAT2x4 =
gl.DOUBLE_MAT3x2 =
gl.DOUBLE_MAT3x4 =
gl.DOUBLE_MAT4x2 =
gl.DOUBLE_MAT4x3 =
gl.DOUBLE_VEC2 =
gl.DOUBLE_VEC3 =
gl.DOUBLE_VEC4 = null;

gl.HALF_FLOAT_ARB = null;

gl.HALF_FLOAT = null;

gl.CONSTANT_COLOR =
gl.ONE_MINUS_CONSTANT_COLOR =
gl.CONSTANT_ALPHA =
gl.ONE_MINUS_CONSTANT_ALPHA =
gl.BLEND_COLOR =
gl.FUNC_ADD =
gl.MIN =
gl.MAX =
gl.BLEND_EQUATION =
gl.FUNC_SUBTRACT =
gl.FUNC_REVERSE_SUBTRACT =
gl.CONVOLUTION_1D =
gl.CONVOLUTION_2D =
gl.SEPARABLE_2D =
gl.CONVOLUTION_BORDER_MODE =
gl.CONVOLUTION_FILTER_SCALE =
gl.CONVOLUTION_FILTER_BIAS =
gl.REDUCE =
gl.CONVOLUTION_FORMAT =
gl.CONVOLUTION_WIDTH =
gl.CONVOLUTION_HEIGHT =
gl.MAX_CONVOLUTION_WIDTH =
gl.MAX_CONVOLUTION_HEIGHT =
gl.POST_CONVOLUTION_RED_SCALE =
gl.POST_CONVOLUTION_GREEN_SCALE =
gl.POST_CONVOLUTION_BLUE_SCALE =
gl.POST_CONVOLUTION_ALPHA_SCALE =
gl.POST_CONVOLUTION_RED_BIAS =
gl.POST_CONVOLUTION_GREEN_BIAS =
gl.POST_CONVOLUTION_BLUE_BIAS =
gl.POST_CONVOLUTION_ALPHA_BIAS =
gl.HISTOGRAM =
gl.PROXY_HISTOGRAM =
gl.HISTOGRAM_WIDTH =
gl.HISTOGRAM_FORMAT =
gl.HISTOGRAM_RED_SIZE =
gl.HISTOGRAM_GREEN_SIZE =
gl.HISTOGRAM_BLUE_SIZE =
gl.HISTOGRAM_ALPHA_SIZE =
gl.HISTOGRAM_LUMINANCE_SIZE =
gl.HISTOGRAM_SINK =
gl.MINMAX =
gl.MINMAX_FORMAT =
gl.MINMAX_SINK =
gl.TABLE_TOO_LARGE =
gl.COLOR_MATRIX =
gl.COLOR_MATRIX_STACK_DEPTH =
gl.MAX_COLOR_MATRIX_STACK_DEPTH =
gl.POST_COLOR_MATRIX_RED_SCALE =
gl.POST_COLOR_MATRIX_GREEN_SCALE =
gl.POST_COLOR_MATRIX_BLUE_SCALE =
gl.POST_COLOR_MATRIX_ALPHA_SCALE =
gl.POST_COLOR_MATRIX_RED_BIAS =
gl.POST_COLOR_MATRIX_GREEN_BIAS =
gl.POST_COLOR_MATRIX_BLUE_BIAS =
gl.POST_COLOR_MATRIX_ALPHA_BIAS =
gl.COLOR_TABLE =
gl.POST_CONVOLUTION_COLOR_TABLE =
gl.POST_COLOR_MATRIX_COLOR_TABLE =
gl.PROXY_COLOR_TABLE =
gl.PROXY_POST_CONVOLUTION_COLOR_TABLE =
gl.PROXY_POST_COLOR_MATRIX_COLOR_TABLE =
gl.COLOR_TABLE_SCALE =
gl.COLOR_TABLE_BIAS =
gl.COLOR_TABLE_FORMAT =
gl.COLOR_TABLE_WIDTH =
gl.COLOR_TABLE_RED_SIZE =
gl.COLOR_TABLE_GREEN_SIZE =
gl.COLOR_TABLE_BLUE_SIZE =
gl.COLOR_TABLE_ALPHA_SIZE =
gl.COLOR_TABLE_LUMINANCE_SIZE =
gl.COLOR_TABLE_INTENSITY_SIZE =
gl.IGNORE_BORDER =
gl.CONSTANT_BORDER =
gl.WRAP_BORDER =
gl.REPLICATE_BORDER =
gl.CONVOLUTION_BORDER_COLOR =
gl.PARAMETER_BUFFER_ARB =
gl.PARAMETER_BUFFER_BINDING_ARB = null;

gl.VERTEX_ATTRIB_ARRAY_DIVISOR_ARB = null;

gl.NUM_SAMPLE_COUNTS = null;

gl.INTERNALFORMAT_SUPPORTED =
gl.INTERNALFORMAT_PREFERRED =
gl.INTERNALFORMAT_RED_SIZE =
gl.INTERNALFORMAT_GREEN_SIZE =
gl.INTERNALFORMAT_BLUE_SIZE =
gl.INTERNALFORMAT_ALPHA_SIZE =
gl.INTERNALFORMAT_DEPTH_SIZE =
gl.INTERNALFORMAT_STENCIL_SIZE =
gl.INTERNALFORMAT_SHARED_SIZE =
gl.INTERNALFORMAT_RED_TYPE =
gl.INTERNALFORMAT_GREEN_TYPE =
gl.INTERNALFORMAT_BLUE_TYPE =
gl.INTERNALFORMAT_ALPHA_TYPE =
gl.INTERNALFORMAT_DEPTH_TYPE =
gl.INTERNALFORMAT_STENCIL_TYPE =
gl.MAX_WIDTH =
gl.MAX_HEIGHT =
gl.MAX_DEPTH =
gl.MAX_LAYERS =
gl.MAX_COMBINED_DIMENSIONS =
gl.COLOR_COMPONENTS =
gl.DEPTH_COMPONENTS =
gl.STENCIL_COMPONENTS =
gl.COLOR_RENDERABLE =
gl.DEPTH_RENDERABLE =
gl.STENCIL_RENDERABLE =
gl.FRAMEBUFFER_RENDERABLE =
gl.FRAMEBUFFER_RENDERABLE_LAYERED =
gl.FRAMEBUFFER_BLEND =
gl.READ_PIXELS =
gl.READ_PIXELS_FORMAT =
gl.READ_PIXELS_TYPE =
gl.TEXTURE_IMAGE_FORMAT =
gl.TEXTURE_IMAGE_TYPE =
gl.GET_TEXTURE_IMAGE_FORMAT =
gl.GET_TEXTURE_IMAGE_TYPE =
gl.MIPMAP =
gl.MANUAL_GENERATE_MIPMAP =
gl.AUTO_GENERATE_MIPMAP =
gl.COLOR_ENCODING =
gl.SRGB_READ =
gl.SRGB_WRITE =
gl.SRGB_DECODE_ARB =
gl.FILTER =
gl.VERTEX_TEXTURE =
gl.TESS_CONTROL_TEXTURE =
gl.TESS_EVALUATION_TEXTURE =
gl.GEOMETRY_TEXTURE =
gl.FRAGMENT_TEXTURE =
gl.COMPUTE_TEXTURE =
gl.TEXTURE_SHADOW =
gl.TEXTURE_GATHER =
gl.TEXTURE_GATHER_SHADOW =
gl.SHADER_IMAGE_LOAD =
gl.SHADER_IMAGE_STORE =
gl.SHADER_IMAGE_ATOMIC =
gl.IMAGE_TEXEL_SIZE =
gl.IMAGE_COMPATIBILITY_CLASS =
gl.IMAGE_PIXEL_FORMAT =
gl.IMAGE_PIXEL_TYPE =
gl.SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST =
gl.SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST =
gl.SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE =
gl.SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE =
gl.TEXTURE_COMPRESSED_BLOCK_WIDTH =
gl.TEXTURE_COMPRESSED_BLOCK_HEIGHT =
gl.TEXTURE_COMPRESSED_BLOCK_SIZE =
gl.CLEAR_BUFFER =
gl.TEXTURE_VIEW =
gl.VIEW_COMPATIBILITY_CLASS =
gl.FULL_SUPPORT =
gl.CAVEAT_SUPPORT =
gl.IMAGE_CLASS_4_X_32 =
gl.IMAGE_CLASS_2_X_32 =
gl.IMAGE_CLASS_1_X_32 =
gl.IMAGE_CLASS_4_X_16 =
gl.IMAGE_CLASS_2_X_16 =
gl.IMAGE_CLASS_1_X_16 =
gl.IMAGE_CLASS_4_X_8 =
gl.IMAGE_CLASS_2_X_8 =
gl.IMAGE_CLASS_1_X_8 =
gl.IMAGE_CLASS_11_11_10 =
gl.IMAGE_CLASS_10_10_10_2 =
gl.VIEW_CLASS_128_BITS =
gl.VIEW_CLASS_96_BITS =
gl.VIEW_CLASS_64_BITS =
gl.VIEW_CLASS_48_BITS =
gl.VIEW_CLASS_32_BITS =
gl.VIEW_CLASS_24_BITS =
gl.VIEW_CLASS_16_BITS =
gl.VIEW_CLASS_8_BITS =
gl.VIEW_CLASS_S3TC_DXT1_RGB =
gl.VIEW_CLASS_S3TC_DXT1_RGBA =
gl.VIEW_CLASS_S3TC_DXT3_RGBA =
gl.VIEW_CLASS_S3TC_DXT5_RGBA =
gl.VIEW_CLASS_RGTC1_RED =
gl.VIEW_CLASS_RGTC2_RG =
gl.VIEW_CLASS_BPTC_UNORM =
gl.VIEW_CLASS_BPTC_FLOAT = null;

gl.MIN_MAP_BUFFER_ALIGNMENT = null;

gl.MAP_READ_BIT =
gl.MAP_WRITE_BIT =
gl.MAP_INVALIDATE_RANGE_BIT =
gl.MAP_INVALIDATE_BUFFER_BIT =
gl.MAP_FLUSH_EXPLICIT_BIT =
gl.MAP_UNSYNCHRONIZED_BIT = null;

gl.MATRIX_PALETTE_ARB =
gl.MAX_MATRIX_PALETTE_STACK_DEPTH_ARB =
gl.MAX_PALETTE_MATRICES_ARB =
gl.CURRENT_PALETTE_MATRIX_ARB =
gl.MATRIX_INDEX_ARRAY_ARB =
gl.CURRENT_MATRIX_INDEX_ARB =
gl.MATRIX_INDEX_ARRAY_SIZE_ARB =
gl.MATRIX_INDEX_ARRAY_TYPE_ARB =
gl.MATRIX_INDEX_ARRAY_STRIDE_ARB =
gl.MATRIX_INDEX_ARRAY_POINTER_ARB = null;

gl.MULTISAMPLE_ARB =
gl.SAMPLE_ALPHA_TO_COVERAGE_ARB =
gl.SAMPLE_ALPHA_TO_ONE_ARB =
gl.SAMPLE_COVERAGE_ARB =
gl.SAMPLE_BUFFERS_ARB =
gl.SAMPLES_ARB =
gl.SAMPLE_COVERAGE_VALUE_ARB =
gl.SAMPLE_COVERAGE_INVERT_ARB =
gl.MULTISAMPLE_BIT_ARB = null;

gl.TEXTURE0_ARB =
gl.TEXTURE1_ARB =
gl.TEXTURE2_ARB =
gl.TEXTURE3_ARB =
gl.TEXTURE4_ARB =
gl.TEXTURE5_ARB =
gl.TEXTURE6_ARB =
gl.TEXTURE7_ARB =
gl.TEXTURE8_ARB =
gl.TEXTURE9_ARB =
gl.TEXTURE10_ARB =
gl.TEXTURE11_ARB =
gl.TEXTURE12_ARB =
gl.TEXTURE13_ARB =
gl.TEXTURE14_ARB =
gl.TEXTURE15_ARB =
gl.TEXTURE16_ARB =
gl.TEXTURE17_ARB =
gl.TEXTURE18_ARB =
gl.TEXTURE19_ARB =
gl.TEXTURE20_ARB =
gl.TEXTURE21_ARB =
gl.TEXTURE22_ARB =
gl.TEXTURE23_ARB =
gl.TEXTURE24_ARB =
gl.TEXTURE25_ARB =
gl.TEXTURE26_ARB =
gl.TEXTURE27_ARB =
gl.TEXTURE28_ARB =
gl.TEXTURE29_ARB =
gl.TEXTURE30_ARB =
gl.TEXTURE31_ARB =
gl.ACTIVE_TEXTURE_ARB =
gl.CLIENT_ACTIVE_TEXTURE_ARB =
gl.MAX_TEXTURE_UNITS_ARB = null;

gl.QUERY_COUNTER_BITS_ARB =
gl.CURRENT_QUERY_ARB =
gl.QUERY_RESULT_ARB =
gl.QUERY_RESULT_AVAILABLE_ARB =
gl.SAMPLES_PASSED_ARB = null;

gl.ANY_SAMPLES_PASSED =

gl.VERTICES_SUBMITTED_ARB =
gl.PRIMITIVES_SUBMITTED_ARB =
gl.VERTEX_SHADER_INVOCATIONS_ARB =
gl.TESS_CONTROL_SHADER_PATCHES_ARB =
gl.TESS_EVALUATION_SHADER_INVOCATIONS_ARB =
gl.GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB =
gl.FRAGMENT_SHADER_INVOCATIONS_ARB =
gl.COMPUTE_SHADER_INVOCATIONS_ARB =
gl.CLIPPING_INPUT_PRIMITIVES_ARB =
gl.CLIPPING_OUTPUT_PRIMITIVES_ARB = null;

gl.PIXEL_PACK_BUFFER_ARB =
gl.PIXEL_UNPACK_BUFFER_ARB =
gl.PIXEL_PACK_BUFFER_BINDING_ARB =
gl.PIXEL_UNPACK_BUFFER_BINDING_ARB = null;

gl.POINT_SIZE_MIN_ARB =
gl.POINT_SIZE_MAX_ARB =
gl.POINT_FADE_THRESHOLD_SIZE_ARB =
gl.POINT_DISTANCE_ATTENUATION_ARB = null;

gl.POINT_SPRITE_ARB =
gl.COORD_REPLACE_ARB = null;

gl.UNIFORM =
gl.UNIFORM_BLOCK =
gl.PROGRAM_INPUT =
gl.PROGRAM_OUTPUT =
gl.BUFFER_VARIABLE =
gl.SHADER_STORAGE_BLOCK =
gl.IS_PER_PATCH =
gl.VERTEX_SUBROUTINE =
gl.TESS_CONTROL_SUBROUTINE =
gl.TESS_EVALUATION_SUBROUTINE =
gl.GEOMETRY_SUBROUTINE =
gl.FRAGMENT_SUBROUTINE =
gl.COMPUTE_SUBROUTINE =
gl.VERTEX_SUBROUTINE_UNIFORM =
gl.TESS_CONTROL_SUBROUTINE_UNIFORM =
gl.TESS_EVALUATION_SUBROUTINE_UNIFORM =
gl.GEOMETRY_SUBROUTINE_UNIFORM =
gl.FRAGMENT_SUBROUTINE_UNIFORM =
gl.COMPUTE_SUBROUTINE_UNIFORM =
gl.TRANSFORM_FEEDBACK_VARYING =
gl.ACTIVE_RESOURCES =
gl.MAX_NAME_LENGTH =
gl.MAX_NUM_ACTIVE_VARIABLES =
gl.MAX_NUM_COMPATIBLE_SUBROUTINES =
gl.NAME_LENGTH =
gl.TYPE =
gl.ARRAY_SIZE =
gl.OFFSET =
gl.BLOCK_INDEX =
gl.ARRAY_STRIDE =
gl.MATRIX_STRIDE =
gl.IS_ROW_MAJOR =
gl.ATOMIC_COUNTER_BUFFER_INDEX =
gl.BUFFER_BINDING =
gl.BUFFER_DATA_SIZE =
gl.NUM_ACTIVE_VARIABLES =
gl.ACTIVE_VARIABLES =
gl.REFERENCED_BY_VERTEX_SHADER =
gl.REFERENCED_BY_TESS_CONTROL_SHADER =
gl.REFERENCED_BY_TESS_EVALUATION_SHADER =
gl.REFERENCED_BY_GEOMETRY_SHADER =
gl.REFERENCED_BY_FRAGMENT_SHADER =
gl.REFERENCED_BY_COMPUTE_SHADER =
gl.TOP_LEVEL_ARRAY_SIZE =
gl.TOP_LEVEL_ARRAY_STRIDE =
gl.LOCATION =
gl.LOCATION_INDEX = null;

gl.QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION =
gl.FIRST_VERTEX_CONVENTION =
gl.LAST_VERTEX_CONVENTION =
gl.PROVOKING_VERTEX = null;

gl.QUERY_BUFFER_BARRIER_BIT =
gl.QUERY_BUFFER =
gl.QUERY_BUFFER_BINDING =
gl.QUERY_RESULT_NO_WAIT = null;

gl.CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB =
gl.LOSE_CONTEXT_ON_RESET_ARB =
gl.GUILTY_CONTEXT_RESET_ARB =
gl.INNOCENT_CONTEXT_RESET_ARB =
gl.UNKNOWN_CONTEXT_RESET_ARB =
gl.RESET_NOTIFICATION_STRATEGY_ARB =
gl.NO_RESET_NOTIFICATION_ARB = null;

gl.SAMPLE_SHADING_ARB =
gl.MIN_SAMPLE_SHADING_VALUE_ARB = null;

gl.SAMPLER_BINDING = null;

gl.TEXTURE_CUBE_MAP_SEAMLESS = null;

gl.VERTEX_SHADER_BIT =
gl.FRAGMENT_SHADER_BIT =
gl.GEOMETRY_SHADER_BIT =
gl.TESS_CONTROL_SHADER_BIT =
gl.TESS_EVALUATION_SHADER_BIT =
gl.PROGRAM_SEPARABLE =
gl.ACTIVE_PROGRAM =
gl.PROGRAM_PIPELINE_BINDING =
gl.ALL_SHADER_BITS = null;

gl.ATOMIC_COUNTER_BUFFER =
gl.ATOMIC_COUNTER_BUFFER_BINDING =
gl.ATOMIC_COUNTER_BUFFER_START =
gl.ATOMIC_COUNTER_BUFFER_SIZE =
gl.ATOMIC_COUNTER_BUFFER_DATA_SIZE =
gl.ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS =
gl.ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER =
gl.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER =
gl.MAX_VERTEX_ATOMIC_COUNTER_BUFFERS =
gl.MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS =
gl.MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS =
gl.MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS =
gl.MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS =
gl.MAX_COMBINED_ATOMIC_COUNTER_BUFFERS =
gl.MAX_VERTEX_ATOMIC_COUNTERS =
gl.MAX_TESS_CONTROL_ATOMIC_COUNTERS =
gl.MAX_TESS_EVALUATION_ATOMIC_COUNTERS =
gl.MAX_GEOMETRY_ATOMIC_COUNTERS =
gl.MAX_FRAGMENT_ATOMIC_COUNTERS =
gl.MAX_COMBINED_ATOMIC_COUNTERS =
gl.MAX_ATOMIC_COUNTER_BUFFER_SIZE =
gl.ACTIVE_ATOMIC_COUNTER_BUFFERS =
gl.UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX =
gl.UNSIGNED_INT_ATOMIC_COUNTER =
gl.MAX_ATOMIC_COUNTER_BUFFER_BINDINGS = null;

gl.VERTEX_ATTRIB_ARRAY_BARRIER_BIT =
gl.ELEMENT_ARRAY_BARRIER_BIT =
gl.UNIFORM_BARRIER_BIT =
gl.TEXTURE_FETCH_BARRIER_BIT =
gl.SHADER_IMAGE_ACCESS_BARRIER_BIT =
gl.COMMAND_BARRIER_BIT =
gl.PIXEL_BUFFER_BARRIER_BIT =
gl.TEXTURE_UPDATE_BARRIER_BIT =
gl.BUFFER_UPDATE_BARRIER_BIT =
gl.FRAMEBUFFER_BARRIER_BIT =
gl.TRANSFORM_FEEDBACK_BARRIER_BIT =
gl.ATOMIC_COUNTER_BARRIER_BIT =
gl.MAX_IMAGE_UNITS =
gl.MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS =
gl.IMAGE_BINDING_NAME =
gl.IMAGE_BINDING_LEVEL =
gl.IMAGE_BINDING_LAYERED =
gl.IMAGE_BINDING_LAYER =
gl.IMAGE_BINDING_ACCESS =
gl.IMAGE_1D =
gl.IMAGE_2D =
gl.IMAGE_3D =
gl.IMAGE_2D_RECT =
gl.IMAGE_CUBE =
gl.IMAGE_BUFFER =
gl.IMAGE_1D_ARRAY =
gl.IMAGE_2D_ARRAY =
gl.IMAGE_CUBE_MAP_ARRAY =
gl.IMAGE_2D_MULTISAMPLE =
gl.IMAGE_2D_MULTISAMPLE_ARRAY =
gl.INT_IMAGE_1D =
gl.INT_IMAGE_2D =
gl.INT_IMAGE_3D =
gl.INT_IMAGE_2D_RECT =
gl.INT_IMAGE_CUBE =
gl.INT_IMAGE_BUFFER =
gl.INT_IMAGE_1D_ARRAY =
gl.INT_IMAGE_2D_ARRAY =
gl.INT_IMAGE_CUBE_MAP_ARRAY =
gl.INT_IMAGE_2D_MULTISAMPLE =
gl.INT_IMAGE_2D_MULTISAMPLE_ARRAY =
gl.UNSIGNED_INT_IMAGE_1D =
gl.UNSIGNED_INT_IMAGE_2D =
gl.UNSIGNED_INT_IMAGE_3D =
gl.UNSIGNED_INT_IMAGE_2D_RECT =
gl.UNSIGNED_INT_IMAGE_CUBE =
gl.UNSIGNED_INT_IMAGE_BUFFER =
gl.UNSIGNED_INT_IMAGE_1D_ARRAY =
gl.UNSIGNED_INT_IMAGE_2D_ARRAY =
gl.UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY =
gl.UNSIGNED_INT_IMAGE_2D_MULTISAMPLE =
gl.UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY =
gl.MAX_IMAGE_SAMPLES =
gl.IMAGE_BINDING_FORMAT =
gl.IMAGE_FORMAT_COMPATIBILITY_TYPE =
gl.IMAGE_FORMAT_COMPATIBILITY_BY_SIZE =
gl.IMAGE_FORMAT_COMPATIBILITY_BY_CLASS =
gl.MAX_VERTEX_IMAGE_UNIFORMS =
gl.MAX_TESS_CONTROL_IMAGE_UNIFORMS =
gl.MAX_TESS_EVALUATION_IMAGE_UNIFORMS =
gl.MAX_GEOMETRY_IMAGE_UNIFORMS =
gl.MAX_FRAGMENT_IMAGE_UNIFORMS =
gl.MAX_COMBINED_IMAGE_UNIFORMS =
gl.ALL_BARRIER_BITS = null;

gl.PROGRAM_OBJECT_ARB =
gl.SHADER_OBJECT_ARB =
gl.OBJECT_TYPE_ARB =
gl.OBJECT_SUBTYPE_ARB =
gl.FLOAT_VEC2_ARB =
gl.FLOAT_VEC3_ARB =
gl.FLOAT_VEC4_ARB =
gl.INT_VEC2_ARB =
gl.INT_VEC3_ARB =
gl.INT_VEC4_ARB =
gl.BOOL_ARB =
gl.BOOL_VEC2_ARB =
gl.BOOL_VEC3_ARB =
gl.BOOL_VEC4_ARB =
gl.FLOAT_MAT2_ARB =
gl.FLOAT_MAT3_ARB =
gl.FLOAT_MAT4_ARB =
gl.SAMPLER_1D_ARB =
gl.SAMPLER_2D_ARB =
gl.SAMPLER_3D_ARB =
gl.SAMPLER_CUBE_ARB =
gl.SAMPLER_1D_SHADOW_ARB =
gl.SAMPLER_2D_SHADOW_ARB =
gl.SAMPLER_2D_RECT_ARB =
gl.SAMPLER_2D_RECT_SHADOW_ARB =
gl.OBJECT_DELETE_STATUS_ARB =
gl.OBJECT_COMPILE_STATUS_ARB =
gl.OBJECT_LINK_STATUS_ARB =
gl.OBJECT_VALIDATE_STATUS_ARB =
gl.OBJECT_INFO_LOG_LENGTH_ARB =
gl.OBJECT_ATTACHED_OBJECTS_ARB =
gl.OBJECT_ACTIVE_UNIFORMS_ARB =
gl.OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB =
gl.OBJECT_SHADER_SOURCE_LENGTH_ARB = null;

gl.SHADER_STORAGE_BARRIER_BIT =
gl.MAX_COMBINED_SHADER_OUTPUT_RESOURCES =
gl.SHADER_STORAGE_BUFFER =
gl.SHADER_STORAGE_BUFFER_BINDING =
gl.SHADER_STORAGE_BUFFER_START =
gl.SHADER_STORAGE_BUFFER_SIZE =
gl.MAX_VERTEX_SHADER_STORAGE_BLOCKS =
gl.MAX_GEOMETRY_SHADER_STORAGE_BLOCKS =
gl.MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS =
gl.MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS =
gl.MAX_FRAGMENT_SHADER_STORAGE_BLOCKS =
gl.MAX_COMPUTE_SHADER_STORAGE_BLOCKS =
gl.MAX_COMBINED_SHADER_STORAGE_BLOCKS =
gl.MAX_SHADER_STORAGE_BUFFER_BINDINGS =
gl.MAX_SHADER_STORAGE_BLOCK_SIZE =
gl.SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT = null;

gl.ACTIVE_SUBROUTINES =
gl.ACTIVE_SUBROUTINE_UNIFORMS =
gl.MAX_SUBROUTINES =
gl.MAX_SUBROUTINE_UNIFORM_LOCATIONS =
gl.ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS =
gl.ACTIVE_SUBROUTINE_MAX_LENGTH =
gl.ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH =
gl.NUM_COMPATIBLE_SUBROUTINES =
gl.COMPATIBLE_SUBROUTINES = null;

gl.SHADING_LANGUAGE_VERSION_ARB = null;

gl.SHADER_INCLUDE_ARB =
gl.NAMED_STRING_LENGTH_ARB =
gl.NAMED_STRING_TYPE_ARB = null;

gl.TEXTURE_COMPARE_MODE_ARB =
gl.TEXTURE_COMPARE_FUNC_ARB =
gl.COMPARE_R_TO_TEXTURE_ARB = null;

gl.TEXTURE_COMPARE_FAIL_VALUE_ARB = null;

gl.SPARSE_STORAGE_BIT_ARB =
gl.SPARSE_BUFFER_PAGE_SIZE_ARB = null;

gl.VIRTUAL_PAGE_SIZE_X_ARB =
gl.VIRTUAL_PAGE_SIZE_Y_ARB =
gl.VIRTUAL_PAGE_SIZE_Z_ARB =
gl.MAX_SPARSE_TEXTURE_SIZE_ARB =
gl.MAX_SPARSE_3D_TEXTURE_SIZE_ARB =
gl.MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB =
gl.TEXTURE_SPARSE_ARB =
gl.VIRTUAL_PAGE_SIZE_INDEX_ARB =
gl.NUM_VIRTUAL_PAGE_SIZES_ARB =
gl.SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB =
gl.NUM_SPARSE_LEVELS_ARB = null;

gl.DEPTH_STENCIL_TEXTURE_MODE = null;

gl.SYNC_FLUSH_COMMANDS_BIT =
gl.MAX_SERVER_WAIT_TIMEOUT =
gl.OBJECT_TYPE =
gl.SYNC_CONDITION =
gl.SYNC_STATUS =
gl.SYNC_FLAGS =
gl.SYNC_FENCE =
gl.SYNC_GPU_COMMANDS_COMPLETE =
gl.UNSIGNALED =
gl.SIGNALED =
gl.ALREADY_SIGNALED =
gl.TIMEOUT_EXPIRED =
gl.CONDITION_SATISFIED =
gl.WAIT_FAILED =
gl.TIMEOUT_IGNORED = null;

gl.PATCHES =
gl.UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER =
gl.UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER =
gl.MAX_TESS_CONTROL_INPUT_COMPONENTS =
gl.MAX_TESS_EVALUATION_INPUT_COMPONENTS =
gl.MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS =
gl.MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS =
gl.PATCH_VERTICES =
gl.PATCH_DEFAULT_INNER_LEVEL =
gl.PATCH_DEFAULT_OUTER_LEVEL =
gl.TESS_CONTROL_OUTPUT_VERTICES =
gl.TESS_GEN_MODE =
gl.TESS_GEN_SPACING =
gl.TESS_GEN_VERTEX_ORDER =
gl.TESS_GEN_POINT_MODE =
gl.ISOLINES =
gl.FRACTIONAL_ODD =
gl.FRACTIONAL_EVEN =
gl.MAX_PATCH_VERTICES =
gl.MAX_TESS_GEN_LEVEL =
gl.MAX_TESS_CONTROL_UNIFORM_COMPONENTS =
gl.MAX_TESS_EVALUATION_UNIFORM_COMPONENTS =
gl.MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS =
gl.MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS =
gl.MAX_TESS_CONTROL_OUTPUT_COMPONENTS =
gl.MAX_TESS_PATCH_COMPONENTS =
gl.MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS =
gl.MAX_TESS_EVALUATION_OUTPUT_COMPONENTS =
gl.TESS_EVALUATION_SHADER =
gl.TESS_CONTROL_SHADER =
gl.MAX_TESS_CONTROL_UNIFORM_BLOCKS =
gl.MAX_TESS_EVALUATION_UNIFORM_BLOCKS = null;

gl.CLAMP_TO_BORDER_ARB = null;

gl.TEXTURE_BUFFER_ARB =
gl.MAX_TEXTURE_BUFFER_SIZE_ARB =
gl.TEXTURE_BINDING_BUFFER_ARB =
gl.TEXTURE_BUFFER_DATA_STORE_BINDING_ARB =
gl.TEXTURE_BUFFER_FORMAT_ARB = null;

gl.TEXTURE_BUFFER_OFFSET =
gl.TEXTURE_BUFFER_SIZE =
gl.TEXTURE_BUFFER_OFFSET_ALIGNMENT = null;

gl.COMPRESSED_ALPHA_ARB =
gl.COMPRESSED_LUMINANCE_ARB =
gl.COMPRESSED_LUMINANCE_ALPHA_ARB =
gl.COMPRESSED_INTENSITY_ARB =
gl.COMPRESSED_RGB_ARB =
gl.COMPRESSED_RGBA_ARB =
gl.TEXTURE_COMPRESSION_HINT_ARB =
gl.TEXTURE_COMPRESSED_IMAGE_SIZE_ARB =
gl.TEXTURE_COMPRESSED_ARB =
gl.NUM_COMPRESSED_TEXTURE_FORMATS_ARB =
gl.COMPRESSED_TEXTURE_FORMATS_ARB = null;

gl.COMPRESSED_RGBA_BPTC_UNORM_ARB =
gl.COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB =
gl.COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB =
gl.COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB = null;

gl.COMPRESSED_RED_RGTC1 =
gl.COMPRESSED_SIGNED_RED_RGTC1 =
gl.COMPRESSED_RG_RGTC2 =
gl.COMPRESSED_SIGNED_RG_RGTC2 = null;

gl.NORMAL_MAP_ARB =
gl.REFLECTION_MAP_ARB =
gl.TEXTURE_CUBE_MAP_ARB =
gl.TEXTURE_BINDING_CUBE_MAP_ARB =
gl.TEXTURE_CUBE_MAP_POSITIVE_X_ARB =
gl.TEXTURE_CUBE_MAP_NEGATIVE_X_ARB =
gl.TEXTURE_CUBE_MAP_POSITIVE_Y_ARB =
gl.TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB =
gl.TEXTURE_CUBE_MAP_POSITIVE_Z_ARB =
gl.TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB =
gl.PROXY_TEXTURE_CUBE_MAP_ARB =
gl.MAX_CUBE_MAP_TEXTURE_SIZE_ARB = null;

gl.TEXTURE_CUBE_MAP_ARRAY_ARB =
gl.TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB =
gl.PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB =
gl.SAMPLER_CUBE_MAP_ARRAY_ARB =
gl.SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB =
gl.INT_SAMPLER_CUBE_MAP_ARRAY_ARB =
gl.UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB = null;

gl.SUBTRACT_ARB =
gl.COMBINE_ARB =
gl.COMBINE_RGB_ARB =
gl.COMBINE_ALPHA_ARB =
gl.RGB_SCALE_ARB =
gl.ADD_SIGNED_ARB =
gl.INTERPOLATE_ARB =
gl.CONSTANT_ARB =
gl.PRIMARY_COLOR_ARB =
gl.PREVIOUS_ARB =
gl.SOURCE0_RGB_ARB =
gl.SOURCE1_RGB_ARB =
gl.SOURCE2_RGB_ARB =
gl.SOURCE0_ALPHA_ARB =
gl.SOURCE1_ALPHA_ARB =
gl.SOURCE2_ALPHA_ARB =
gl.OPERAND0_RGB_ARB =
gl.OPERAND1_RGB_ARB =
gl.OPERAND2_RGB_ARB =
gl.OPERAND0_ALPHA_ARB =
gl.OPERAND1_ALPHA_ARB =
gl.OPERAND2_ALPHA_ARB = null;

gl.DOT3_RGB_ARB =
gl.DOT3_RGBA_ARB = null;

gl.RGBA32F_ARB =
gl.RGB32F_ARB =
gl.ALPHA32F_ARB =
gl.INTENSITY32F_ARB =
gl.LUMINANCE32F_ARB =
gl.LUMINANCE_ALPHA32F_ARB =
gl.RGBA16F_ARB =
gl.RGB16F_ARB =
gl.ALPHA16F_ARB =
gl.INTENSITY16F_ARB =
gl.LUMINANCE16F_ARB =
gl.LUMINANCE_ALPHA16F_ARB =
gl.TEXTURE_RED_TYPE_ARB =
gl.TEXTURE_GREEN_TYPE_ARB =
gl.TEXTURE_BLUE_TYPE_ARB =
gl.TEXTURE_ALPHA_TYPE_ARB =
gl.TEXTURE_LUMINANCE_TYPE_ARB =
gl.TEXTURE_INTENSITY_TYPE_ARB =
gl.TEXTURE_DEPTH_TYPE_ARB =
gl.UNSIGNED_NORMALIZED_ARB = null;

gl.MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB =
gl.MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB =
gl.MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB = null;

gl.MIRROR_CLAMP_TO_EDGE = null;

gl.MIRRORED_REPEAT_ARB = null;

gl.SAMPLE_POSITION =
gl.SAMPLE_MASK =
gl.SAMPLE_MASK_VALUE =
gl.MAX_SAMPLE_MASK_WORDS =
gl.TEXTURE_2D_MULTISAMPLE =
gl.PROXY_TEXTURE_2D_MULTISAMPLE =
gl.TEXTURE_2D_MULTISAMPLE_ARRAY =
gl.PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY =
gl.TEXTURE_BINDING_2D_MULTISAMPLE =
gl.TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY =
gl.TEXTURE_SAMPLES =
gl.TEXTURE_FIXED_SAMPLE_LOCATIONS =
gl.SAMPLER_2D_MULTISAMPLE =
gl.INT_SAMPLER_2D_MULTISAMPLE =
gl.UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE =
gl.SAMPLER_2D_MULTISAMPLE_ARRAY =
gl.INT_SAMPLER_2D_MULTISAMPLE_ARRAY =
gl.UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY =
gl.MAX_COLOR_TEXTURE_SAMPLES =
gl.MAX_DEPTH_TEXTURE_SAMPLES =
gl.MAX_INTEGER_SAMPLES = null;

gl.TEXTURE_RECTANGLE_ARB =
gl.TEXTURE_BINDING_RECTANGLE_ARB =
gl.PROXY_TEXTURE_RECTANGLE_ARB =
gl.MAX_RECTANGLE_TEXTURE_SIZE_ARB = null;

gl.COMPRESSED_RED =
gl.COMPRESSED_RG =
gl.RG =
gl.RG_INTEGER =
gl.R8 =
gl.R16 =
gl.RG8 =
gl.RG16 =
gl.R16F =
gl.R32F =
gl.RG16F =
gl.RG32F =
gl.R8I =
gl.R8UI =
gl.R16I =
gl.R16UI =
gl.R32I =
gl.R32UI =
gl.RG8I =
gl.RG8UI =
gl.RG16I =
gl.RG16UI =
gl.RG32I =
gl.RG32UI = null;

gl.RGB10_A2UI = null;

gl.STENCIL_INDEX = null;

gl.TEXTURE_IMMUTABLE_FORMAT = null;

gl.TEXTURE_SWIZZLE_R =
gl.TEXTURE_SWIZZLE_G =
gl.TEXTURE_SWIZZLE_B =
gl.TEXTURE_SWIZZLE_A =
gl.TEXTURE_SWIZZLE_RGBA = null;

gl.TEXTURE_VIEW_MIN_LEVEL =
gl.TEXTURE_VIEW_NUM_LEVELS =
gl.TEXTURE_VIEW_MIN_LAYER =
gl.TEXTURE_VIEW_NUM_LAYERS = null;

gl.TIME_ELAPSED =
gl.TIMESTAMP = null;

gl.TRANSFORM_FEEDBACK =
gl.TRANSFORM_FEEDBACK_BUFFER_PAUSED =
gl.TRANSFORM_FEEDBACK_BUFFER_ACTIVE =
gl.TRANSFORM_FEEDBACK_BINDING = null;

gl.MAX_TRANSFORM_FEEDBACK_BUFFERS = null;

gl.TRANSFORM_FEEDBACK_OVERFLOW_ARB =
gl.TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB = null;

gl.TRANSPOSE_MODELVIEW_MATRIX_ARB =
gl.TRANSPOSE_PROJECTION_MATRIX_ARB =
gl.TRANSPOSE_TEXTURE_MATRIX_ARB =
gl.TRANSPOSE_COLOR_MATRIX_ARB = null;

gl.UNIFORM_BUFFER =
gl.UNIFORM_BUFFER_BINDING =
gl.UNIFORM_BUFFER_START =
gl.UNIFORM_BUFFER_SIZE =
gl.MAX_VERTEX_UNIFORM_BLOCKS =
gl.MAX_GEOMETRY_UNIFORM_BLOCKS =
gl.MAX_FRAGMENT_UNIFORM_BLOCKS =
gl.MAX_COMBINED_UNIFORM_BLOCKS =
gl.MAX_UNIFORM_BUFFER_BINDINGS =
gl.MAX_UNIFORM_BLOCK_SIZE =
gl.MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS =
gl.MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS =
gl.MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS =
gl.UNIFORM_BUFFER_OFFSET_ALIGNMENT =
gl.ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH =
gl.ACTIVE_UNIFORM_BLOCKS =
gl.UNIFORM_TYPE =
gl.UNIFORM_SIZE =
gl.UNIFORM_NAME_LENGTH =
gl.UNIFORM_BLOCK_INDEX =
gl.UNIFORM_OFFSET =
gl.UNIFORM_ARRAY_STRIDE =
gl.UNIFORM_MATRIX_STRIDE =
gl.UNIFORM_IS_ROW_MAJOR =
gl.UNIFORM_BLOCK_BINDING =
gl.UNIFORM_BLOCK_DATA_SIZE =
gl.UNIFORM_BLOCK_NAME_LENGTH =
gl.UNIFORM_BLOCK_ACTIVE_UNIFORMS =
gl.UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES =
gl.UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER =
gl.UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER =
gl.UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER =
gl.INVALID_INDEX = null;

gl.BGRA = null;

gl.VERTEX_ARRAY_BINDING = null;

gl.VERTEX_ATTRIB_BINDING =
gl.VERTEX_ATTRIB_RELATIVE_OFFSET =
gl.VERTEX_BINDING_DIVISOR =
gl.VERTEX_BINDING_OFFSET =
gl.VERTEX_BINDING_STRIDE =
gl.MAX_VERTEX_ATTRIB_RELATIVE_OFFSET =
gl.MAX_VERTEX_ATTRIB_BINDINGS =
gl.VERTEX_BINDING_BUFFER = null;

gl.MODELVIEW0_ARB =
gl.MODELVIEW1_ARB =
gl.MAX_VERTEX_UNITS_ARB =
gl.ACTIVE_VERTEX_UNITS_ARB =
gl.WEIGHT_SUM_UNITY_ARB =
gl.VERTEX_BLEND_ARB =
gl.CURRENT_WEIGHT_ARB =
gl.WEIGHT_ARRAY_TYPE_ARB =
gl.WEIGHT_ARRAY_STRIDE_ARB =
gl.WEIGHT_ARRAY_SIZE_ARB =
gl.WEIGHT_ARRAY_POINTER_ARB =
gl.WEIGHT_ARRAY_ARB =
gl.MODELVIEW2_ARB =
gl.MODELVIEW3_ARB =
gl.MODELVIEW4_ARB =
gl.MODELVIEW5_ARB =
gl.MODELVIEW6_ARB =
gl.MODELVIEW7_ARB =
gl.MODELVIEW8_ARB =
gl.MODELVIEW9_ARB =
gl.MODELVIEW10_ARB =
gl.MODELVIEW11_ARB =
gl.MODELVIEW12_ARB =
gl.MODELVIEW13_ARB =
gl.MODELVIEW14_ARB =
gl.MODELVIEW15_ARB =
gl.MODELVIEW16_ARB =
gl.MODELVIEW17_ARB =
gl.MODELVIEW18_ARB =
gl.MODELVIEW19_ARB =
gl.MODELVIEW20_ARB =
gl.MODELVIEW21_ARB =
gl.MODELVIEW22_ARB =
gl.MODELVIEW23_ARB =
gl.MODELVIEW24_ARB =
gl.MODELVIEW25_ARB =
gl.MODELVIEW26_ARB =
gl.MODELVIEW27_ARB =
gl.MODELVIEW28_ARB =
gl.MODELVIEW29_ARB =
gl.MODELVIEW30_ARB =
gl.MODELVIEW31_ARB = null;

gl.BUFFER_SIZE_ARB =
gl.BUFFER_USAGE_ARB =
gl.ARRAY_BUFFER_ARB =
gl.ELEMENT_ARRAY_BUFFER_ARB =
gl.ARRAY_BUFFER_BINDING_ARB =
gl.ELEMENT_ARRAY_BUFFER_BINDING_ARB =
gl.VERTEX_ARRAY_BUFFER_BINDING_ARB =
gl.NORMAL_ARRAY_BUFFER_BINDING_ARB =
gl.COLOR_ARRAY_BUFFER_BINDING_ARB =
gl.INDEX_ARRAY_BUFFER_BINDING_ARB =
gl.TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB =
gl.EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB =
gl.SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB =
gl.FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB =
gl.WEIGHT_ARRAY_BUFFER_BINDING_ARB =
gl.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB =
gl.READ_ONLY_ARB =
gl.WRITE_ONLY_ARB =
gl.READ_WRITE_ARB =
gl.BUFFER_ACCESS_ARB =
gl.BUFFER_MAPPED_ARB =
gl.BUFFER_MAP_POINTER_ARB =
gl.STREAM_DRAW_ARB =
gl.STREAM_READ_ARB =
gl.STREAM_COPY_ARB =
gl.STATIC_DRAW_ARB =
gl.STATIC_READ_ARB =
gl.STATIC_COPY_ARB =
gl.DYNAMIC_DRAW_ARB =
gl.DYNAMIC_READ_ARB =
gl.DYNAMIC_COPY_ARB = null;

gl.COLOR_SUM_ARB =
gl.VERTEX_PROGRAM_ARB =
gl.VERTEX_ATTRIB_ARRAY_ENABLED_ARB =
gl.VERTEX_ATTRIB_ARRAY_SIZE_ARB =
gl.VERTEX_ATTRIB_ARRAY_STRIDE_ARB =
gl.VERTEX_ATTRIB_ARRAY_TYPE_ARB =
gl.CURRENT_VERTEX_ATTRIB_ARB =
gl.PROGRAM_LENGTH_ARB =
gl.PROGRAM_STRING_ARB =
gl.MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB =
gl.MAX_PROGRAM_MATRICES_ARB =
gl.CURRENT_MATRIX_STACK_DEPTH_ARB =
gl.CURRENT_MATRIX_ARB =
gl.VERTEX_PROGRAM_POINT_SIZE_ARB =
gl.VERTEX_PROGRAM_TWO_SIDE_ARB =
gl.VERTEX_ATTRIB_ARRAY_POINTER_ARB =
gl.PROGRAM_ERROR_POSITION_ARB =
gl.PROGRAM_BINDING_ARB =
gl.MAX_VERTEX_ATTRIBS_ARB =
gl.VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB =
gl.PROGRAM_ERROR_STRING_ARB =
gl.PROGRAM_FORMAT_ASCII_ARB =
gl.PROGRAM_FORMAT_ARB =
gl.PROGRAM_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_INSTRUCTIONS_ARB =
gl.PROGRAM_NATIVE_INSTRUCTIONS_ARB =
gl.MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB =
gl.PROGRAM_TEMPORARIES_ARB =
gl.MAX_PROGRAM_TEMPORARIES_ARB =
gl.PROGRAM_NATIVE_TEMPORARIES_ARB =
gl.MAX_PROGRAM_NATIVE_TEMPORARIES_ARB =
gl.PROGRAM_PARAMETERS_ARB =
gl.MAX_PROGRAM_PARAMETERS_ARB =
gl.PROGRAM_NATIVE_PARAMETERS_ARB =
gl.MAX_PROGRAM_NATIVE_PARAMETERS_ARB =
gl.PROGRAM_ATTRIBS_ARB =
gl.MAX_PROGRAM_ATTRIBS_ARB =
gl.PROGRAM_NATIVE_ATTRIBS_ARB =
gl.MAX_PROGRAM_NATIVE_ATTRIBS_ARB =
gl.PROGRAM_ADDRESS_REGISTERS_ARB =
gl.MAX_PROGRAM_ADDRESS_REGISTERS_ARB =
gl.PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB =
gl.MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB =
gl.MAX_PROGRAM_LOCAL_PARAMETERS_ARB =
gl.MAX_PROGRAM_ENV_PARAMETERS_ARB =
gl.PROGRAM_UNDER_NATIVE_LIMITS_ARB =
gl.TRANSPOSE_CURRENT_MATRIX_ARB =
gl.MATRIX0_ARB =
gl.MATRIX1_ARB =
gl.MATRIX2_ARB =
gl.MATRIX3_ARB =
gl.MATRIX4_ARB =
gl.MATRIX5_ARB =
gl.MATRIX6_ARB =
gl.MATRIX7_ARB =
gl.MATRIX8_ARB =
gl.MATRIX9_ARB =
gl.MATRIX10_ARB =
gl.MATRIX11_ARB =
gl.MATRIX12_ARB =
gl.MATRIX13_ARB =
gl.MATRIX14_ARB =
gl.MATRIX15_ARB =
gl.MATRIX16_ARB =
gl.MATRIX17_ARB =
gl.MATRIX18_ARB =
gl.MATRIX19_ARB =
gl.MATRIX20_ARB =
gl.MATRIX21_ARB =
gl.MATRIX22_ARB =
gl.MATRIX23_ARB =
gl.MATRIX24_ARB =
gl.MATRIX25_ARB =
gl.MATRIX26_ARB =
gl.MATRIX27_ARB =
gl.MATRIX28_ARB =
gl.MATRIX29_ARB =
gl.MATRIX30_ARB =
gl.MATRIX31_ARB = null;

gl.VERTEX_SHADER_ARB =
gl.MAX_VERTEX_UNIFORM_COMPONENTS_ARB =
gl.MAX_VARYING_FLOATS_ARB =
gl.MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB =
gl.MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB =
gl.OBJECT_ACTIVE_ATTRIBUTES_ARB = null;

gl.UNSIGNED_INT_10F_11F_11F_REV = null;

gl.UNSIGNED_INT_2_10_10_10_REV =
gl.INT_2_10_10_10_REV = null;

gl.DEPTH_RANGE =
gl.VIEWPORT =
gl.SCISSOR_BOX =
gl.SCISSOR_TEST =
gl.MAX_VIEWPORTS =
gl.VIEWPORT_SUBPIXEL_BITS =
gl.VIEWPORT_BOUNDS_RANGE =
gl.LAYER_PROVOKING_VERTEX =
gl.VIEWPORT_INDEX_PROVOKING_VERTEX =
gl.UNDEFINED_VERTEX = null;

for(var p in gl){
    gl[p] = gl_[p];
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

module.exports = gl;