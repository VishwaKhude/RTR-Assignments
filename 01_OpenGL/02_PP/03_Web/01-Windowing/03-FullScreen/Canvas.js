// Our Main Function

var canvas = null;

var context = null;

function main() {
    // Get Canvas
     canvas = document.getElementById("VMK");
    if (canvas == null)
        console.log("Getting of Canvas Failed\n");

    else
        console.log("Getting Canvas Succedded\n");

    // Get context from above canvas 
     context = canvas.getContext("2d");
    if (context == null)
        console.log("Getting 2D Context failed\n");

    else
        console.log("Getting 2D Context Succedded\n");

    // Get
    context.fillStyle = "0x000000";

    // Paint The Canvas Rectangle With Above Color
    context.fillRect(0, 0, canvas.width, canvas.height);

    // Draw Text
    drawText("Hello World!!!");

    // Register for keyboard events
    window.addEventListener("keydown", keyDown, false);

    // Register for mouse events
    window.addEventListener("click", mouseDown, false);

}

function keyDown(event)
{
    // Code
    switch (event.keyCode)
    {
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

function drawText(text)
{
    //Code
    // Centering the text horizontally
    context.textAlign = "center";

    // Centering the window vertically
    context.textBaseline = "middle";

    // Set Font
    context.font = "48px sans-serif";

    // Set The Text Color to White
    context.fillStyle = "lime";

    // Paint the text with above color
    context.fillText(text, canvas.width / 2, canvas.height / 2);
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
    }
}
