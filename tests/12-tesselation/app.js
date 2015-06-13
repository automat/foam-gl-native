var fs   = require('fs');
var path = require('path');

var Utils    = require('../_common/Utils');
var glu      = require('../_common/glu');
var Vec3     = require('../_common/Vec3');
var Matrix44 = require('../_common/Matrix44');
var Context = require('../../index.js');
var gl = Context.gl;

var Perlin = require('perlin-simplex');

var VERT_SRC =
    "#version 400\n" +
    "uniform uint width;\n" +
    "uniform uint height;\n" +
    "out vec4 tposition;\n" +
    "const vec2 quad_offsets[6] = vec2[](\n" +
    "   vec2(0,0),vec2(1,0),vec2(1,1),\n" +
    "   vec2(0,0),vec2(1,1),vec2(0,1)\n" +
    ");\n" +
    "void main() {\n" +
    "   vec2 base = vec2(gl_InstanceID/width, gl_InstanceID%width);\n" +
    "   vec2 offset = quad_offsets[gl_VertexID];\n" +
    "   vec2 pos = (base + offset)/vec2(width+1, height+1);\n" +
    "   tposition = vec4(pos,0,1);\n" +
    "}\n";

var TESS_CONTROL_SRC =
    "#version 400\n" +
    "uniform vec3 ViewPosition;\n" +
    "uniform float tess_scale;\n" +
    "layout(vertices = 3) out;\n" +
    "in vec4 tposition[];\n" +
    "out vec4 tcposition[];\n" +
    "void main()\n" +
    "{\n" +
    "   tcposition[gl_InvocationID] = tposition[gl_InvocationID];\n" +
    "   if(gl_InvocationID == 0) {\n" +
    "       vec3 terrainpos = ViewPosition;\n" +
    "       terrainpos.z -= clamp(terrainpos.z,-0.1, 0.1);\n" +
    "       vec4 center = (tposition[1]+tposition[2])/2.0;\n" +
    "       gl_TessLevelOuter[0] = min(6.0, 1+tess_scale*0.5/distance(center.xyz, terrainpos));\n" +
    "       center = (tposition[2]+tposition[0])/2.0;\n" +
    "       gl_TessLevelOuter[1] = min(6.0, 1+tess_scale*0.5/distance(center.xyz, terrainpos));\n" +
    "       center = (tposition[0]+tposition[1])/2.0;\n" +
    "       gl_TessLevelOuter[2] = min(6.0, 1+tess_scale*0.5/distance(center.xyz, terrainpos));\n" +
    "       center = (tposition[0]+tposition[1]+tposition[2])/3.0;\n" +
    "       gl_TessLevelInner[0] = min(7.0, 1+tess_scale*0.7/distance(center.xyz, terrainpos));\n" +
    "   }\n" +
    "}\n";

var TESS_EVAL_SRC =
    "#version 400\n" +
    "uniform mat4 ViewProjection;\n" +
    "uniform sampler2D displacement;\n" +
    "layout(triangles, equal_spacing, cw) in;\n" +
    "in vec4 tcposition[];\n" +
    "out vec2 tecoord;\n" +
    "out vec4 teposition;\n" +
    "void main()\n" +
    "{\n" +
    "   teposition = gl_TessCoord.x * tcposition[0];\n" +
    "   teposition += gl_TessCoord.y * tcposition[1];\n" +
    "   teposition += gl_TessCoord.z * tcposition[2];\n" +
    "   tecoord = teposition.xy;\n" +
    "   vec3 offset = texture(displacement, tecoord).xyz;\n" +
    "   teposition.xyz = offset;\n" +
    "   gl_Position = ViewProjection*teposition;\n" +
    "}\n";

var FRAG_SRC =
    "#version 400\n" +
    "uniform vec3 ViewPosition;\n" +
    "uniform sampler2D displacement;\n" +
    "in vec4 teposition;\n" +
    "in vec2 tecoord;\n" +
    "layout(location = 0) out vec4 FragColor;\n" +
    "void main() {\n" +
    "   vec3 x = textureOffset(displacement, tecoord, ivec2(0,0)).xyz;\n" +
    "   vec3 t0 = x-textureOffset(displacement, tecoord, ivec2(1,0)).xyz;\n" +
    "   vec3 t1 = x-textureOffset(displacement, tecoord, ivec2(0,1)).xyz;\n" +
    "   vec3 normal = (gl_FrontFacing?1:-1)*normalize(cross(t0, t1));\n" +
    "   vec3 light = normalize(vec3(2, -1, 3));\n" +
    "   vec3 reflected = reflect(normalize(ViewPosition-teposition.xyz), normal);\n" +
    "   float ambient = 0.1;\n" +
    "   float diffuse = max(0,dot(normal, light));\n" +
    "   float specular = pow(max(0,dot(reflected, light)), 64);\n" +
    "   FragColor = vec4(vec3(ambient + 0.5*diffuse + 0.4*specular), 1);\n" +
    "}\n";

function setup(){
    this.initWindow(800,600);

    this._vao = gl.genVertexArrays(1)[0];
    gl.bindVertexArray(this._vao);

    var program = this._program = Utils.createProgramv(gl,[
        {type : gl.VERTEX_SHADER,          src : VERT_SRC},
        {type : gl.TESS_CONTROL_SHADER,    src : TESS_CONTROL_SRC},
        {type : gl.TESS_EVALUATION_SHADER, src : TESS_EVAL_SRC},
        {type : gl.FRAGMENT_SHADER,        src : FRAG_SRC}
    ]);

    this._uniformLocationWidth          = gl.getUniformLocation(program,'width');
    this._uniformLocationHeight         = gl.getUniformLocation(program,'height');
    this._uniformLocationViewProjection = gl.getUniformLocation(program,'ViewProjection');
    this._uniformLocationViewPosition   = gl.getUniformLocation(program,'ViewPosition');
    this._uniformLocationDisplacement   = gl.getUniformLocation(program,'displacement');
    this._uniformLocationTessScale      = gl.getUniformLocation(program,'tess_scale');

    var terrainWidth = this._terrainWidth = 1024;
    var terrainHeight = this._terrainHeight = 1024;
    var displacementData = new Float32Array(terrainWidth * terrainHeight * 3);

    var layernorm = new Vec3(0.1,0.3,1.0).normalize();
    var layerdir  = new Vec3(0,0,1);

    layerdir.sub(layernorm.scaled(layernorm.dot(layerdir))).normalize();

    var perlin = new Perlin();

    for(var y = 0, x, pos = new Vec3(), tmp = new Vec3(), res = new Vec3(), index; y < terrainHeight; ++y){
        for(x = 0; x < terrainWidth; ++x){
            pos.x = x / terrainWidth;
            pos.y = y / terrainHeight;

            tmp.x = pos.x;
            tmp.y = pos.y;
            tmp.z = perlin.noise(pos.x * 5.0,pos.y * 5.0) * 0.15;

            res.set(tmp.add(layerdir.scaled(perlin.noise(30.0 * layernorm.dot(tmp),5.0) * 0.04)));

            index = (y * terrainWidth * x) * 3;

            displacementData[index  ] = res.x;
            displacementData[index+1] = res.y;
            displacementData[index+2] = res.z;
        }
    }

    this._displacement = gl.genTextures(1)[0];

    gl.bindTexture(gl.TEXTURE_2D,this._displacement);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB32F, terrainWidth, terrainHeight, 0, gl.RGB, gl.FLOAT, displacementData);

    gl.enable(gl.DEPTH_TEST);



    this._time = this.getSecondsElapsed();
}

function update(){
    var t     = this.getSecondsElapsed();
    var dt   = t - this._time;
    var time = this._time = t;

    var matrixProjection = new Matrix44();
    var matrixView       = new Matrix44();
    var matrixProjectionView = new Matrix44();


    var position = new Vec3(Math.cos(time),1,Math.sin(time));

    glu.perspective(matrixProjection.m,5.0,this.getWindowAspectRatio(),0.001,10.0);
    matrixView.translate(position.scaled(-1));
    matrixProjection.multiplied(matrixView,matrixProjectionView);

    gl.polygonMode(gl.FRONT_AND_BACK,gl.FILL);
    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D,this._displacement);

    gl.useProgram(this._program);
    gl.uniform1ui(this._uniformLocationWidth,64);
    gl.uniform1ui(this._uniformLocationHeight,64);
    gl.uniformMatrix4fv(this._uniformLocationViewProjection, gl.FALSE, new Float32Array(matrixProjectionView.m));
    gl.uniform3fv(this._uniformLocationViewPosition, new Float32Array([position.x,position.y,position.z]));

    gl.uniform1f(this._uniformLocationTessScale,1.0);
    gl.uniform1i(this._uniformLocationDisplacement, 0);

    gl.drawArraysInstanced(gl.PATCHES, 0, 6, 64 * 64);

    Utils.getError(gl);

}

Context.new({setup:setup,update:update});