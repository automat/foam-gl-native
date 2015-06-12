function compileShader(gl,type,strShader){
    var types = {};
    types[gl.VERTEX_SHADER] = 'VERTEX_SHADER';
    types[gl.FRAGMENT_SHADER] = 'FRAGMENT_SHADER';
    types[gl.GEOMETRY_SHADER] = 'GEOMETRY_SHADER';
    types[gl.TESS_CONTROL_SHADER] = 'TESS_CONTROL_SHADER';
    types[gl.TESS_EVALUATION_SHADER] = 'TESS_EVALUATION_SHADER';
    types[gl.COMPUTE_SHADER] = 'COMPUTE_SHADER';

    if(!types[type]){
        throw new Error('Wrong shader type: ' + type + '.');
    }

    var out = gl.createShader(type);
    gl.shaderSource(out,strShader);
    gl.compileShader(out);
    if(!gl.getShaderParameter(out,gl.COMPILE_STATUS)){
        throw new Error(types[type] + ' ' + gl.getShaderInfoLog(out));
    }
    gl.compileShader(out);

    return out;
}

function checkProgramLinkStatus(gl,program){
    if(!gl.getProgramParameter(program,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(program));
    }
}

function bindAttribLocations(gl,program,locationsToBind){
    for(var location in locationsToBind){
        gl.bindAttribLocation(program, location, locationsToBind[location]);
    }
}

function bindFragDataLocations(gl,program,locationsToBind){
    for(var location in locationsToBind){
        gl.bindFragDataLocation(program, location, locationsToBind[location]);
    }
}

function createProgram(gl,strVertShader,strFragShader,attribLocationsToBind,fragDataLocationsToBind){
    attribLocationsToBind   = attribLocationsToBind   === undefined ? {} : attribLocationsToBind;
    fragDataLocationsToBind = fragDataLocationsToBind === undefined ? {} : fragDataLocationsToBind;

    var vertShader = compileShader(gl,gl.VERTEX_SHADER,strVertShader);
    var fragShader = compileShader(gl,gl.FRAGMENT_SHADER, strFragShader);

    var out = gl.createProgram();
    gl.attachShader(out,vertShader);
    gl.attachShader(out,fragShader);

    bindAttribLocations(gl,out,attribLocationsToBind);
    bindFragDataLocations(gl,out,fragDataLocationsToBind);

    gl.linkProgram(out);
    checkProgramLinkStatus(gl,out);

    return out;
}

function createProgramv(gl,shaders,attribLocationsToBind,fragDataLocationsToBind){
    attribLocationsToBind   = attribLocationsToBind   === undefined ? {} : attribLocationsToBind;
    fragDataLocationsToBind = fragDataLocationsToBind === undefined ? {} : fragDataLocationsToBind;

    var out = gl.createProgram();
    var numShaders = shaders.length;
    for(var i = 0, shader; i < numShaders; ++i){
        shader = shaders[i];
        gl.attachShader(out,compileShader(gl,shader.type,shader.src));
    }

    bindAttribLocations(gl,out,attribLocationsToBind);
    bindFragDataLocations(gl,out,fragDataLocationsToBind);

    gl.linkProgram(out);
    checkProgramLinkStatus(gl,out);

    return out;
}

function getError(gl){
    var ERROR_STR = {};
    ERROR_STR[gl.INVALID_OPERATION] = 'INVALID_OPERATION';
    ERROR_STR[gl.INVALID_ENUM] = 'INVALID_ENUM';
    ERROR_STR[gl.INVALID_VALUE] = 'INVALID_VALUE';
    ERROR_STR[gl.OUT_OF_MEMORY] = 'OUT_OF_MEMORY';
    ERROR_STR[gl.INVALID_FRAMEBUFFER_OPERATION] = 'INVALID_FRAMEBUFFER_OPERATION';

    var error = gl.getError();
    if(error === gl.NO_ERROR){
        return;
    }

    error = ERROR_STR[error] === undefined ? error : ERROR_STR[error];
    console.log("GL_ERROR",error);
}


module.exports = {
    compileShader : compileShader,
    bindAttribLocations : bindAttribLocations,
    bindFragDataLocations : bindFragDataLocations,
    checkProgramLinkStatus : checkProgramLinkStatus,
    createProgram : createProgram,
    createProgramv : createProgramv,
    getError : getError
};