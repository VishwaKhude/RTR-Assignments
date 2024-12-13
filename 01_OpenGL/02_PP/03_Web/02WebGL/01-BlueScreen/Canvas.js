// Our Main Function

// Global Variables
var canvas = null;

var gl = null;
var bfullscreen = false;
var canvas_original_width;
var canvas_original_height;

var requestAnimationFrame =
    window.requestAnimationFrame ||         // For Chrome
    window.webkitRequestAnimationFrame ||   // For Safari(MAC)
    window.mozRequestAnimationFrame ||      // For Mozilla Firefox
    window.oRequestAnimationFrame ||        // For Opera
    window.msRequestAnimationFrame;         // For Microsoft Edge

function main() {
    // Get Canvas
     canvas = document.getElementById("VMK");
    if (canvas == null)
        console.log("Getting of Canvas Failed\n");

    else
        console.log("Getting Canvas Succedded\n");

    

    // set canvas width & height for future use
    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;
  
    // Register for keyboard events
    window.addEventListener("keydown", keyDown, false);

    // Register for mouse events
    window.addEventListener("click", mouseDown, false);

    window.addEventListener("resize", resize, false);

    initialize();

    resize();

    display();
}

function keyDown(event)
{
    // Code
    switch (event.keyCode)
    {
        case 81:
        case 113:
            uninitialize();
            window.close();

        case 70:
        case 102:
            toggleFullscreen();
            break;

        default:
            break;
    }
  
}

function mouseDown()
{
   
}
 


function toggleFullscreen()
{
    var fullscreen_element =
        document.fullscreenElement ||
        document.webkitFullscreenElement ||
        document.mozFullScreenElement ||
        document.msFullscreenElement ||
        null;

    // If not FullScreen
    if (fullscreen_element == null) {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();

        else if (canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();

        else if (canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();

        else if (canvas.msRequestFullscreen)
            canvas.msRequestFullscreen();

        bfullscreen = true;
    }

    else
    {
        if (document.ExitFullscreen)
            document.ExitFullscreen();

        else if (document.webkitExitFullscreen)
            document.webkitExitFullscreen();

        else if (document.mozCancelFullScreen)
            document.mozCancelFullScreen();

        else if (document.msExitFullscreen)
            document.msExitFullscreen();

        bfullscreen = false;
    }
}

function initialize()
{
    //code
    // Get context from above canvas 
    gl = canvas.getContext("webgl2");
    if (gl == null)
        console.log("Getting WebGL2 Context Failed\n");

    else
        console.log("Getting WebGL2 Context Succeded\n");

    // Set WebGL2 context's view width and view height properties
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    // Set ClearColor
    gl.clearColor(0.0, 0.0, 1.0, 1.0);
}

function resize()
{
    //code
    if (bfullscreen == true) 
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else
    {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    // Set viewport
    gl.viewport(0, 0, canvas.width, canvas.height);
}

function display()
{
    //code
    gl.clear(gl.COLOR_BUFFER_BIT);

    // Do Double buffering
    requestAnimationFrame(display, canvas);
}

function update()
{
    //code

}

function uninitialize()

{
    //code

}

