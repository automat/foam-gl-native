function compileShader(gl,type,strShader){
    if(type != gl.VERTEX_SHADER && type != gl.FRAGMENT_SHADER && type != gl.GEOMETRY_SHADER){
        throw new Error('Wrong shader type: ' + type + '.');
    }
    var out = gl.createShader(type);
    gl.shaderSource(out,strShader);
    gl.compileShader(out);
    if(!gl.getShaderParameter(out,gl.COMPILE_STATUS)){
        throw new Error((type == gl.VERTEX_SHADER ? 'VERTEX' : type == gl.FRAGMENT_SHADER ? 'FRAGMENT' : 'GEOMETRY') + ' ' + gl.getShaderInfoLog(out));
    }
    gl.compileShader(out);
    return out;
}

function createProgram(gl,strVertShader,strFragShader,attribLocationsToBind,fragDataLocationsToBind){
    attribLocationsToBind   = attribLocationsToBind   === undefined ? {} : attribLocationsToBind;
    fragDataLocationsToBind = fragDataLocationsToBind === undefined ? {} : fragDataLocationsToBind;

    var vertShader = compileShader(gl,gl.VERTEX_SHADER,strVertShader);
    var fragShader = compileShader(gl,gl.FRAGMENT_SHADER, strFragShader);

    var out = gl.createProgram();
    gl.attachShader(out,vertShader);
    gl.attachShader(out,fragShader);

    for(var attribLocation in attribLocationsToBind){
        gl.bindAttribLocation(out,attribLocation[0],attribLocation[1]);
    }

    for(var fragDataLocations in fragDataLocationsToBind){
        gl.bindFragDataLocation(out,fragDataLocations[0],fragDataLocations[1]);
    }

    gl.linkProgram(out);

    if(!gl.getProgramParameter(out,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(out));
    }

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

    for(var attribLocation in attribLocationsToBind){
        gl.bindAttribLocation(out,attribLocation[0],attribLocation[1]);
    }

    for(var fragDataLocations in fragDataLocationsToBind){
        gl.bindFragDataLocation(out,fragDataLocations[0],fragDataLocations[1]);
    }

    gl.linkProgram(out);

    if(!gl.getProgramParameter(out,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(out));
    }

    return out;
}


module.exports = {
    compileShader : compileShader,
    createProgram : createProgram,
    createProgramv : createProgramv
};