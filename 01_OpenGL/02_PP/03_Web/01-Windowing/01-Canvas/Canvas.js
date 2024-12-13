// Our Main Function
function main() {
    // Get Canvas
    var canvas = document.getElementById("VMK");
    if (canvas == null)
        console.log("Getting of Canvas Failed\n");

    else
        console.log("Getting Canvas Succedded\n");

    // Get context from above canvas 
    var context = canvas.getContext("2d");
    if (context == null)
        console.log("Getting 2D Context failed\n");

    else
        console.log("Getting 2D Context Succedded\n");

    // Get
    context.fillStyle = "0x000000";

    // Paint The Canvas Rectangle With Above Color
    context.fillRect(0, 0, canvas.width, canvas.height);

    //Declare the text
    var text = "Hello World!!!";

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
