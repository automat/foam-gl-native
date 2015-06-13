var fs   = require('fs');
var path = require('path');

var Utils    = require('../_common/Utils');
var glu      = require('../_common/glu');
var Vec3     = require('../_common/Vec3');
var Matrix44 = require('../_common/Matrix44');
var Context = require('../../index.js');
var gl = Context.gl;


var VERT_SRC =
    "#version 330\n" +
    "layout(location = 0) in vec4 vposition;\n" +
    "void main() {\n" +
    "   gl_Position = vposition;\n" +
    "}\n";

var GEOM_SRC =
    "#version 330\n" +
    "uniform mat4 View;\n" +
    "uniform mat4 Projection;\n" +
    "layout (points) in;\n" +
    "layout (triangle_strip, max_vertices = 4) out;\n" +
    "out vec2 txcoord;\n" +
    "void main() {\n" +
    "   vec4 pos = View*gl_in[0].gl_Position;\n" +
    "   txcoord = vec2(-1,-1);\n" +
    "   gl_Position = Projection*(pos+0.75*vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2( 1,-1);\n" +
    "   gl_Position = Projection*(pos+0.75*vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2(-1, 1);\n" +
    "   gl_Position = Projection*(pos+0.75*vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2( 1, 1);\n" +
    "   gl_Position = Projection*(pos+0.75*vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "}\n";

var FRAG_SRC =
    "#version 330\n" +
    "in vec2 txcoord;\n" +
    "layout(location = 0) out vec4 FragColor;\n" +
    "void main() {\n" +
    "   float s = 0.2*(1/(1+15.*dot(txcoord, txcoord))-1/16.);\n" +
    "   FragColor = vec4(s,s,s,1.0);\n" +
    "}\n";

var ERROR_STR = {};
ERROR_STR[gl.INVALID_OPERATION] = 'INVALID_OPERATION';
ERROR_STR[gl.INVALID_ENUM] = 'INVALID_ENUM';
ERROR_STR[gl.INVALID_VALUE] = 'INVALID_VALUE';
ERROR_STR[gl.OUT_OF_MEMORY] = 'OUT_OF_MEMORY';
ERROR_STR[gl.INVALID_FRAMEBUFFER_OPERATION] = 'INVALID_FRAMEBUFFER_OPERATION';

function getGLError(){
    var error = gl.getError();
    if(error != gl.NO_ERROR){
        error = ERROR_STR[error] === undefined ? error : ERROR_STR[error];
        console.log("GL_ERROR",error);
    }
}

function setup(){
    this.initWindow(800,600);

    var program = this._program = Utils.createProgramv(gl,[
        {type : gl.VERTEX_SHADER,   src : VERT_SRC},
        {type : gl.GEOMETRY_SHADER, src : GEOM_SRC},
        {type : gl.FRAGMENT_SHADER, src : FRAG_SRC}
    ]);

    gl.useProgram(program);

    this._uniformLocationView       = gl.getUniformLocation(program,'View');
    this._uniformLocationProjection = gl.getUniformLocation(program,'Projection');

    var particles  = this._particles = 128 * 1024;
    var vertexData = this._vertexData = new Float32Array(particles * 3);
    var velocity   = this._veloctiy   = new Float32Array(particles * 3);
    for(var i = 0, offset = new Vec3(0,40,0),vec = new Vec3(); i < particles; ++i){
        vec = vec.setf(
            -0.5 + Math.random(),
            -0.5 + Math.random(),
            -0.5 + Math.random()
        ).scale(5.0).add(offset);

        vertexData[i * 3 + 0] = vec.x;
        vertexData[i * 3 + 1] = vec.y;
        vertexData[i * 3 + 2] = vec.z;

        velocity[i * 3 + 0] = -0.5 + Math.random();
        velocity[i * 3 + 1] = -0.5 + Math.random();
        velocity[i * 3 + 2] = -0.5 + Math.random();
    }

    var numBuffers = this._numBuffers = 1;
    var vao = this._vao = gl.genVertexArrays(numBuffers);
    var vbo = this._vbo = gl.genBuffers(numBuffers);

    for(var i = 0; i < numBuffers; ++i){
        gl.bindVertexArray(vao[i]);
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo[i]);
        gl.bufferData(gl.ARRAY_BUFFER,vertexData,gl.DYNAMIC_DRAW);
        gl.enableVertexAttribArray(0);
        gl.vertexAttribPointer(0,3,gl.FLOAT,gl.FALSE,0,0);
    }

    gl.disable(gl.DEPTH_TEST);
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.ONE,gl.ONE);

    this._spheres = 3;
    this._center  = new Float32Array([0,12,1, -3,0,0, 5,-10,0]);
    this._radius  = new Float32Array([3, 7, 12]);

    this._currentBuffer = 0;
}

function update(){
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    var t = this.getSecondsElapsed();
    var particles  = this._particles;
    var spheres    = this._spheres;
    var center     = this._center;
    var radius     = this._radius;
    var vertexData = this._vertexData;
    var velocity   = this._veloctiy;

    var dt = 1.0 / 30.0;
    var g  = new Vec3(0,-9.81,0);
    var bounce = 1.2;

    for(var i = 0, vert = new Vec3(), vel = new Vec3(), diff = new Vec3(), dist, dot, i3, j3; i < particles; ++i){
        i3 = i * 3;

        vert.setf(
            vertexData[i3    ],
            vertexData[i3 + 1],
            vertexData[i3 + 2]
        );

        vel.setf(
            velocity[i3    ],
            velocity[i3 + 1],
            velocity[i3 + 2]
        );

        for(var j = 0; j < spheres; ++j){
            j3 = j * 3;

            diff.setf(
                vert.x - center[j3    ],
                vert.y - center[j3 + 1],
                vert.z - center[j3 + 2]
            );

            dist = diff.length();
            dot  = diff.dot(vel);

            if((dist < radius[j]) && (dot < 0.0)){
                var dist2 = dist * dist;
                vel.x -= bounce * diff.x / dist2 * dot;
                vel.y -= bounce * diff.y / dist2 * dot;
                vel.z -= bounce * diff.z / dist2 * dot;
            }
        }

        vel.x += dt * g.x;
        vel.y += dt * g.y;
        vel.z += dt * g.z;

        vert.x += dt * vel.x;
        vert.y += dt * vel.y;
        vert.z += dt * vel.z;

        if(vert.y < -30){
            vert.setf(
                -0.5 + Math.random(),
                -0.5 + Math.random(),
                -0.5 + Math.random()
            ).scale(5.0).addf(0,40,0);
            vel.setf(
                -0.5 + Math.random(),
                -0.5 + Math.random(),
                -0.5 + Math.random()
            ).scale(5);
        }

        vertexData[i3    ] = vert.x;
        vertexData[i3 + 1] = vert.y;
        vertexData[i3 + 2] = vert.z;

        velocity[i3    ] = vel.x;
        velocity[i3 + 1] = vel.y;
        velocity[i3 + 2] = vel.z;
    }

    var vbo = this._vbo;
    var vao = this._vao;
    var numBuffers = this._numBuffers;
    var currentBuffer = this._currentBuffer;

    gl.bindBuffer(gl.ARRAY_BUFFER,vbo[(currentBuffer + numBuffers - 1)%numBuffers]);
    gl.bufferData(gl.ARRAY_BUFFER,vertexData,gl.DYNAMIC_DRAW);
   // var mapped = gl.mapBufferRange(gl.ARRAY_BUFFER,0,vertexData.length,gl.MAP_INVALIDATE_RANGE_BIT | gl.MAP_WRITE_BIT ,gl.FLOAT);
   // //console.log('mapped','byteLength',mapped.byteLength,'length',mapped.length);
   // //console.log('vertexData','byteLength',vertexData.byteLength,'length',vertexData.length);
   //// mapped.set(vertexData);
   //
   // for(var i = 0, l = vertexData.length; i < l; ++i){
   //     mapped[i] = vertexData[i];
   //     //console.log(mapped[i], vertexData[i]);
   // }

    //gl.unmapBuffer(gl.ARRAY_BUFFER);

    var projection = new Float32Array(16);
    glu.perspective(
        projection,
        45,this.getWindowAspectRatio(),0.1,100.0
    );

    var view = new Float32Array(16);
    glu.lookAt(view,
        Math.cos(t ) * 80.0,0,Math.sin(t) * 80.0,
        0,0,0,
        0,1,0
    );

    gl.uniformMatrix4fv(this._uniformLocationView,gl.FALSE,view);
    gl.uniformMatrix4fv(this._uniformLocationProjection,gl.FALSE,projection);

    gl.bindVertexArray(vao[this._currentBuffer]);
    gl.drawArrays(gl.POINTS,0,this._particles);

    getGLError();

    this._currentBuffer = (currentBuffer + 1) % numBuffers;
}

Context.new({setup:setup,update:update});