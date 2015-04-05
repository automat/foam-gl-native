attribute vec3 aVertexPosition;
uniform   mat4 uProjectionMatrix;
uniform   mat4 uModelViewMatrix;
uniform  float uPointSize;
void main(){
    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aVertexPosition,1.0);
    gl_PointSize = uPointSize;
}