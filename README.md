E X P E R I M E N T A L

##Build
    
    brew install glfw
    
    brew install glew
    
    npm install --save nan
    
    "include_dirs" : [
        "<!(node -e \"require('nan')\")"
    ]
    
Create the gyp build file

    node-gyp configure
    
Rebuild the addon
    
    node-gyp build
    
Rebuild the addon while testing it within an application 

    (node-gyp build; node app.js)