var canvas = null;
var gl = null;
var bFullScreen = false;
var canvas_original_width;
var canvas_original_height;

// WebGL Related Variables
const VertexAttributeEnem = {
                                AMC_ATTRIBUTE_POSITION:0,
                                AMC_ATTRIBUTE_COLOR:1,
                                AMC_ATTRIBUTE_TEXCOORD:2,
                                AMC_ATTRIBUTE_NORMAL:3
                            };

var shaderProgramObject = null;

var vao = null;
var vbo = null;

var lightAmbient = [0.1 , 0.1 , 0.1 , 1.0];   
var lightDiffused = [1.0 , 1.0 , 1.0 , 1.0];  // White light
var lightSpecular = [1.0 , 1.0 , 1.0 , 1.0];  // White light
var lightPosition = [0.0 , 0.0 , 2.0 , 1.0];  

var materialAmbient = [0.0 , 0.0 , 0.0 , 1.0];
var materialDiffuse = [1.0 , 1.0 , 1.0 , 1.0];
var materialSpecular = [1.0 , 1.0 , 1.0, 1.0];
var materialShinniness = 50.0;

var anglePyramid = 0.0;

var ModelMatrixUniform;
var ProjectionMatrixUniform;
var ViewMatrixUniform;
var lightAmbientUniform;
var lightSpecularUniform;
var materialAmbientUniform;
var materialSpecularUniform;
var materialShininessUniform;

var lightDiffuseUniform;
var MaterialDiffuseUniform;
var lightPositionUniform;
var lightDiffusedUniform;
var keyPressUniform;

var Texture_Marble;
var textureObject;
var textureSamplerUniform;

var bLightingEnabled = true;

var texture_smiley = 0;

var perspectiveProjectionMatrix;

var requestAnimationFrame = window.requestAnimationFrame ||
                             window.webkitRequestAnimationFrame ||
                             window.mozRequestAnimationFrame ||
                             window.oRequestAnimationFrame ||
                             window.msRequesAnimationFrame;

// Our main function
function main()
{
    // Get Canvas
    canvas = document.getElementById("VMK");
    if (canvas == null)
        console.log("Getting Canvas Failed !!!\n");

    else
        console.log("Getting Canvas Succeeded !!!\n");

    // Set Canvas width and height for future use
    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // Register for keyboard events
    window.addEventListener("keydown" , keyDown , false);

    // Register for mouse events
    window.addEventListener("click" , mouseDown , false);

    window.addEventListener("resize" , resize , false);

    initialize();

    resize();

    display();
}

function keyDown(event)
{
    // code 
    switch(event.keyCode)
    {
        case 81:
        case 113:
            uninitialize();
            window.close();
            break;

        case 70:
        case 102:
            toggleFullScreen();
            break;
    }    
}

function mouseDown()
{
    
}

function toggleFullScreen()
{
    var fullScreen_element = document.fullscreenElement || 
                             document.webkitFullscreenElement ||
                             document.mozFullScreenElement ||
                             document.msFullscreenElement ||
                             null;

    if (fullScreen_element == null)
    {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();

        else if(canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();

        else if(canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();

        else if(canvas.msRequestFullscreen)
            canvas.msRequestFullscreen();

        bFullScreen = true;
    }

    // if already full screen
    else
    {
        if (document.exitFullscreen)
            document.exitFullscreen();

        else if(document.webkitExitFullscreen)
            document.webkitExitFullscreen();

        else if(document.mozCancelFullScreen)
            document.mozCancelFullScreen();

        else if(document.msExitFullscreen)
            document.msExitFullscreen();

        bFullScreen = false;
    }
}

function initialize()
{
    // code
    // Getting context from above canvas
    gl = canvas.getContext("webgl2");
    if (gl == null)
        console.log("Getting webgl2 context Failed !!!\n");

    else
        console.log("Getting webgl2 Context Successeded!!!\n");

    // Set WebGL2's context view width and view height properties
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height; 

    // VertexShader
    var VertexShaderSourceCode = (
                                    "#version 300 es" +
                                    "\n" +
                                    "precision mediump int;" +
                                    "in vec3 aNormal;" +
                                    "uniform mat4 uModelMatrix;" +
                                    "uniform mat4 uViewMatrix;" +
                                    "uniform mat4 uProjectionMatrix;" +
                                    "uniform vec4 uLightPosition;" +
                                    "uniform int uKeyPress;" +
                                    "in vec4 aPosition;" +
                                    "uniform mat4 uMVPMatrix;" +
                                    "out vec3 oTransformedNormals;" +
                                    "out vec3 oLightDirection;" +
                                    "out vec3 oViewerVector;" +
                                    "out vec2 oTexCoord;" +
                                    "in vec2 aTexCoord;" +
                                    "out vec4 oColor;" +
                                    "in vec4 aColor;" +
                                    "void main(void)" +
                                    "{" +
                                    "if (uKeyPress == 1) " +
                                    "{" +
                                    "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" +
                                    "oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;" +
                                    "oLightDirection = vec3(uLightPosition - eyeCoordinates);" +
                                    "oViewerVector = -eyeCoordinates.xyz;" +
                                    "}" +
                                    "else" +
                                    "{" +
                                    "oTransformedNormals = vec3(0.0 , 0.0 , 0.0);" +
                                    "oLightDirection = vec3(0.0 , 0.0 , 0.0);" +
                                    "oViewerVector = vec3(0.0 , 0.0 , 0.0);" +
                                    "}" +
                                    "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;" +
                                    "oTexCoord=aTexCoord;" +
                                    "oColor = aColor;" +
                                    "}"
                                );

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject , VertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if(gl.getShaderParameter(vertexShaderObject , gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if(error.length > 0)
        {
            var log = "VertexShader compilation error : " + error;
            alert(log);
            uninitialize();
        }
    }    
    
    else
    {
        console.log("VertexShader compiled successfully");
    }

    // Fragment Shader
    var fragmentShaderSourceCode = (
                                    "#version 300 es"+
                                    "\n"+
                                    "precision highp float;"+
                                    "precision mediump int;" +
                                    "in vec3 oTransformedNormals;" +
                                    "in vec3 oLightDirection;" +
                                    "in vec3 oViewerVector;" +
                                    "uniform vec3 uLightAmbient;" +
                                    "uniform vec3 uLightSpecular;" +
                                    "uniform vec3 uMaterialAmbient;" +
                                    "uniform vec3 uMaterialSpecular;" +
                                    "uniform float uMaterialShininess;" +
                                    "uniform vec3 uLightDiffuse;" +
                                    "uniform vec3 uMaterialDiffuse;" +
                                    "uniform int uKeyPress;" +
                                    "out vec4 FragColor;" +
                                    "in vec2 oTexCoord;" +
                                    "in vec4 oColor;" +
                                    "uniform sampler2D uTextureSampler;" +
                                    "void main(void)" +
                                    "{" +
                                    "vec3 Phong_ADS_Light;" +                                           
                                    "if(uKeyPress == 1)" +
                                    "{" +
                                    "vec3 normalizedTransformedNormal = normalize(oTransformedNormals);" +
                                    "vec3 normalizedLightDirection = normalize(oLightDirection); " +
                                    "vec3 normalizedViewerVector = normalize(oViewerVector); " +
                                    "vec3 reflectionVector = reflect(-normalizedLightDirection , normalizedTransformedNormal);" +
                                    "vec3 ambientLight = uLightAmbient * uMaterialAmbient;" +
                                    "vec3 diffuseLight = uLightDiffuse * uMaterialDiffuse * max(dot(normalizedLightDirection , normalizedTransformedNormal),0.0);" +
                                    "vec3 lightSpecular = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector , normalizedViewerVector) , 0.0) , uMaterialShininess);" +
                                    "Phong_ADS_Light = ambientLight + diffuseLight + lightSpecular;" +
                                    "}" +
                                    "else" +
                                    "{" +
                                    "Phong_ADS_Light = vec3(1.0f , 1.0f , 1.0f);" +
                                    "}" +
                                    "vec3 Tex = vec3(texture(uTextureSampler , oTexCoord));" +
                                    "FragColor = vec4(Tex * Phong_ADS_Light * vec3(oColor) , 1.0);" +
                                    "}" 
                                    )

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject , fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if(gl.getShaderParameter(fragmentShaderObject , gl.COMPILE_STATUS) == false)
    {
        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if(error.length > 0)
        {
            var log = "FragmentShader compilation error : \n" + error;
            alert(log);
            uninitialize();
        }
    }    
        
    else
    {
        console.log("FragmentShader compiled successfully\n");
    }

    // Shader Program
    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject , vertexShaderObject);
    gl.attachShader(shaderProgramObject , fragmentShaderObject);

    gl.bindAttribLocation(shaderProgramObject , VertexAttributeEnem.AMC_ATTRIBUTE_POSITION , "aPosition");
    gl.bindAttribLocation(shaderProgramObject , VertexAttributeEnem.AMC_ATTRIBUTE_COLOR , "aColor");
    gl.bindAttribLocation(shaderProgramObject , VertexAttributeEnem.AMC_ATTRIBUTE_TEXCOORD , "aTexCoord");
    gl.bindAttribLocation(shaderProgramObject , VertexAttributeEnem.AMC_ATTRIBUTE_NORMAL , "aNormal");

    gl.linkProgram(shaderProgramObject);

    if(gl.getProgramParameter(shaderProgramObject , gl.LINK_STATUS) == false)
    {
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length > 0)
        {
            var log = "shaderProgramObject linking error : \n" + error;
            alert(log);
            uninitialize();
        }
    }    
            
    else
    {
        console.log("shaderProgramObject linking successfully\n");
    }

    // Get Uniform Shader Locations
    ModelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uModelMatrix");
    ViewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "uViewMatrix");
    ProjectionMatrixUniform = gl.getUniformLocation(shaderProgramObject , "uProjectionMatrix");
    lightPositionUniform = gl.getUniformLocation(shaderProgramObject , "uLightPosition");
    keyPressUniform = gl.getUniformLocation(shaderProgramObject , "uKeyPress");
    lightDiffusedUniform = gl.getUniformLocation(shaderProgramObject, "uLightDiffuse");
    MaterialDiffuseUniform = gl.getUniformLocation(shaderProgramObject, "uMaterialDiffuse");
    lightAmbientUniform = gl.getUniformLocation(shaderProgramObject , "uLightAmbient");
    lightSpecularUniform = gl.getUniformLocation(shaderProgramObject , "uLightSpecular");
    materialAmbientUniform = gl.getUniformLocation(shaderProgramObject , "uMaterialAmbient");
    materialSpecularUniform = gl.getUniformLocation(shaderProgramObject , "uMaterialSpecular");
    materialShininessUniform = gl.getUniformLocation(shaderProgramObject , "uMaterialShininess");

    // Variables / Geometry / SHape
    var cube_PCNT = new Float32Array([// front
        // position				// color			 // normals				// texcoords
        1.0,  1.0,  1.0,		1.0, 0.0, 0.0,	 0.0,  0.0,  1.0,	1.0, 1.0,
        -1.0,  1.0,  1.0,	1.0, 0.0, 0.0,	 0.0,  0.0,  1.0,	0.0, 1.0,
        -1.0, -1.0,  1.0,	1.0, 0.0, 0.0,	 0.0,  0.0,  1.0,	0.0, 0.0,
        1.0, -1.0,  1.0,		1.0, 0.0, 0.0,	 0.0,  0.0,  1.0,	1.0, 0.0,
                            
        // right			 
        // position				// color			 // normals				// texcoords
        1.0,  1.0, -1.0,		0.0, 0.0, 1.0,	 1.0,  0.0,  0.0,	1.0, 1.0,
        1.0,  1.0,  1.0,		0.0, 0.0, 1.0,	 1.0,  0.0,  0.0,	0.0, 1.0,
        1.0, -1.0,  1.0,		0.0, 0.0, 1.0,	 1.0,  0.0,  0.0,	0.0, 0.0,
        1.0, -1.0, -1.0,		0.0, 0.0, 1.0,	 1.0,  0.0,  0.0,	1.0, 0.0,
                            
        // back				 
        // position				// color			 // normals				// texcoords
        1.0,  1.0, -1.0,		1.0, 1.0, 0.0,	 0.0,  0.0, -1.0,	1.0, 1.0,
        -1.0,  1.0, -1.0,	1.0, 1.0, 0.0,	 0.0,  0.0, -1.0,	0.0, 1.0,
        -1.0, -1.0, -1.0,	1.0, 1.0, 0.0,	 0.0,  0.0, -1.0,	0.0, 0.0,
        1.0, -1.0, -1.0,		1.0, 1.0, 0.0,	 0.0,  0.0, -1.0,	1.0, 0.0,
                            
        // let				 
        // position				// color			 // normals				// texcoords
        -1.0,  1.0,  1.0,	1.0, 0.0, 1.0,	-1.0,  0.0,  0.0,	1.0, 1.0,
        -1.0,  1.0, -1.0,	1.0, 0.0, 1.0,	-1.0,  0.0,  0.0,	0.0, 1.0,
        -1.0, -1.0, -1.0,	1.0, 0.0, 1.0,	-1.0,  0.0,  0.0,	0.0, 0.0,
        -1.0, -1.0,  1.0,	1.0, 0.0, 1.0,	-1.0,  0.0,  0.0,	1.0, 0.0,
                            
        // top				 
        // position				// color			 // normals				// texcoords
        1.0,  1.0, -1.0,		0.0, 1.0, 0.0,	 0.0,  1.0,  0.0,	1.0, 1.0,
        -1.0,  1.0, -1.0,	0.0, 1.0, 0.0,	 0.0,  1.0,  0.0,	0.0, 1.0,
        -1.0,  1.0,  1.0,	0.0, 1.0, 0.0,	 0.0,  1.0,  0.0,	0.0, 0.0,
        1.0,  1.0,  1.0,		0.0, 1.0, 0.0,	 0.0,  1.0,  0.0,	1.0, 0.0,
                            
        // bottom			 
        // position				// color			 // normals				// texcoords
        1.0, -1.0,  1.0,		1.0, 0.5, 0.0,	 0.0, -1.0,  0.0,	1.0, 1.0,
        -1.0, -1.0,  1.0,	1.0, 0.5, 0.0,	 0.0, -1.0,  0.0,	0.0, 1.0,
        -1.0, -1.0, -1.0,	1.0, 0.5, 0.0,	 0.0, -1.0,  0.0,	0.0, 0.0,
        1.0, -1.0, -1.0,		1.0, 0.5, 0.0,	 0.0, -1.0,  0.0,	1.0, 0.0 ]);

    // // VAO
    vao = gl.createVertexArray();
    gl.bindVertexArray(vao);

    // VBO
    vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER , vbo);
    
    gl.bufferData(gl.ARRAY_BUFFER , cube_PCNT , gl.STATIC_DRAW);
    gl.vertexAttribPointer(VertexAttributeEnem.AMC_ATTRIBUTE_POSITION , 3 , gl.FLOAT , false , 11 * 4 , 0 * 4);
    gl.enableVertexAttribArray(VertexAttributeEnem.AMC_ATTRIBUTE_POSITION);

    gl.vertexAttribPointer(VertexAttributeEnem.AMC_ATTRIBUTE_COLOR , 3 , gl.FLOAT , false , 11 * 4 , 3 * 4);
    gl.enableVertexAttribArray(VertexAttributeEnem.AMC_ATTRIBUTE_COLOR);

    gl.vertexAttribPointer(VertexAttributeEnem.AMC_ATTRIBUTE_NORMAL , 3 , gl.FLOAT , false , 11 * 4 , 6 * 4);
    gl.enableVertexAttribArray(VertexAttributeEnem.AMC_ATTRIBUTE_NORMAL);

    gl.vertexAttribPointer(VertexAttributeEnem.AMC_ATTRIBUTE_TEXCOORD , 2 , gl.FLOAT , false , 11 * 4 , 9 * 4);
    gl.enableVertexAttribArray(VertexAttributeEnem.AMC_ATTRIBUTE_TEXCOORD);

    gl.bindBuffer(gl.ARRAY_BUFFER , null);

    gl.bindVertexArray(null);

    Texture_Marble = LoadGLTexture("marble.bmp");

    // gl.enable(gl.TEXTURE_2D);

    // Depth Setting
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // Set Clear Color
    gl.clearColor(0.0 , 0.0 , 0.0 , 1.0);

    // Initialize Projectionmatrix
    perspectiveProjectionMatrix = mat4.create();
}

function LoadGLTexture(imageFileName)
{
    var texture = gl.createTexture();

    texture.image = new Image();

    texture.image.src = imageFileName;

    texture.image.onload = function()
    {
        gl.bindTexture(gl.TEXTURE_2D , texture);

        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL , true);

        gl.texParameteri(gl.TEXTURE_2D , gl.TEXTURE_MAG_FILTER , gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D , gl.TEXTURE_MIN_FILTER , gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D , 0 , gl.RGBA , gl.RGBA , gl.UNSIGNED_BYTE , texture.image);

        gl.generateMipmap(gl.TEXTURE_2D);

        gl.bindTexture(gl.TEXTURE_2D , null);
    }

    return texture;
}

function resize()
{
    // code
    if (bFullScreen == true)
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }

    else
    {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // Set Viewport
    gl.viewport(0 , 0 , canvas.width , canvas.height);

    // set perspective projection
    mat4.perspective(perspectiveProjectionMatrix , 45.0 , parseFloat(canvas.width) / parseFloat(canvas.height) , 0.1 , 100.0 );
}

function degToRad(degrees)
{
    return (degrees * Math.PI/180.0);
}

function display()
{
    // code
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    // Transformations
    var ModelMatrix = mat4.create();

    var ViewMatrix = mat4.create();

    mat4.translate(ModelMatrix , ModelMatrix , [0.0 , 0.0 , -6.0]);

    mat4.rotateY(ModelMatrix , ModelMatrix , degToRad(anglePyramid));

    gl.uniformMatrix4fv(ModelMatrixUniform , false , ModelMatrix);
    gl.uniformMatrix4fv(ViewMatrixUniform , false , ViewMatrix);
    gl.uniformMatrix4fv(ProjectionMatrixUniform , false , perspectiveProjectionMatrix);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D , Texture_Marble);
    gl.uniform1i(textureSamplerUniform , 0);

    if (bLightingEnabled == true)
    {
        gl.uniform1i(keyPressUniform , 1);
        gl.uniform3f(lightAmbientUniform , 1 , lightAmbient[0] ,lightAmbient[1] ,lightAmbient[2] );
        gl.uniform3f(lightDiffusedUniform , 1 , lightDiffused[0] , lightDiffused[1] , lightDiffused[2]);
        gl.uniform3f(lightSpecularUniform , 1 , lightSpecular[0] , lightSpecular[1] , lightSpecular[2]);
        gl.uniform4f(lightPositionUniform , 1 , lightPosition[0], lightPosition[1] ,lightPosition[2] ,lightPosition[3]);

        gl.uniform3f(materialAmbientUniform , 1 , materialAmbient[0] , materialAmbient[1] , materialAmbient[2]);
        gl.uniform3f(MaterialDiffuseUniform , 1 , materialDiffuse[0] , materialDiffuse[1] , materialDiffuse[2]);
        gl.uniform3f(materialSpecularUniform , 1 , materialSpecular[0] , materialSpecular[1] , materialSpecular[2]);
        gl.uniform1f(materialShininessUniform, materialShinniness);
    }

    else 
    {
        gl.uniform1i(keyPressUniform , 0);
    }

    gl.bindVertexArray(vao);

    gl.drawArrays(gl.TRIANGLE_FAN , 0 , 4);
    gl.drawArrays(gl.TRIANGLE_FAN , 4 , 4);
    gl.drawArrays(gl.TRIANGLE_FAN , 8 , 4);
    gl.drawArrays(gl.TRIANGLE_FAN , 12 , 4);
    gl.drawArrays(gl.TRIANGLE_FAN , 16 , 4);
    gl.drawArrays(gl.TRIANGLE_FAN , 20 , 4);

    gl.bindVertexArray(null);

    gl.useProgram(null);

    update();

    // Double buffering
    requestAnimationFrame(display , canvas);
}

function update()
{
    // code
    anglePyramid = anglePyramid + 0.2;

    if (anglePyramid >= 360.0)
    {
        anglePyramid = anglePyramid - 360.0;
    }
}

function uninitialize()
{
    // code
    if(shaderProgramObject > 0)
    {
        gl.useProgram(shaderProgramObject);

        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);
        if(shaderObjects && shaderObjects.length > 0)
        {
            for(let i = 0 ; i < shaderObjects.length ; i++)
            {
                gl.dettachShader(shaderProgramObject , shaderObjects[i]);
                gl.deleteShader(shaderObjects[i]);

                shaderObjects[i] = null;
            }
        }

        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject);

        shaderProgramObject = null;
    }

    if(vbo)
    {
        gl.deleteBuffer(vbo);
        vbo = null;
    }

    if (vao)
    {
        gl.deleteVertexArray(vao);
        vao = null;
    }
}

