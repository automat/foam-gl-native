{
  "targets": [{
      "target_name": "foam_gl_native",
      "sources": [
            "./src/main.cpp",
            "./src/gl/glfw.cpp",
            "./src/gl/glConsts.cpp",
            "./src/gl/gl.cpp"
      ],
      "conditions": [
            [ 'OS=="mac"', {
                "xcode_settings": {
                    'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                     'OTHER_LDFLAGS': ['-stdlib=libc++'],
                     'MACOSX_DEPLOYMENT_TARGET': '10.10'
                }
            }]
      ],
      "libraries" : [
            "-lglfw3",
            "-lGLEW",
            "-framework OpenGL"
      ],
      "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "/usr/local/include",
            "./src",
      ],
      "library_dirs": [
            "/usr/local/lib"
      ]
    }
  ]
}