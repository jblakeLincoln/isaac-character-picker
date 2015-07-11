var NUM_CHARACTERS = 11;
var currentIndex=0; // Which character we're currently dealing with, wherever
thetaDeg = 0; // Circle rotation in degrees - we use rads inside the update loop
targetTheta = 180;

isSpinning = false;
spinSpeed = 0; // Speed at which theta is incremented
spinCounter = 0; // Timing the spins, counting frame by frame
rollResult = 0; // Which character has been rolled

var spinInterval; // Reference to our update loop
var isSelectable = new Array(NUM_CHARACTERS);

// Scale things relative to window size
var windowWidth = 1900;
var windowHeight=1000;
var baseFontSize = 200;
var currentFontSize = 200;

var characters = ["Isaac", 
					"Maggy", 
					"Cain", 
					"Judas", 
					"???", 
					"Eve", 
					"Samson", 
					"Azazel", 
					"Lazarus", 
					"Eden", 
					"The Lost"];


function mainFunc() {
	document.getElementById('btnSpin').value = "Roll";
	getWidthOnScale();
	document.getElementById('isJsEnabled').innerHTML = "";
	
	for (var i = 0; i < NUM_CHARACTERS; ++i) {
		isSelectable.push(true);
		isSelectable[i]=true;
		
		if (characters[i] == "The Lost")
		{
			isSelectable[i] = false;
			document.getElementById(characters[i]+"Thumb").style.opacity = 0.3;
		}	
	}
	
	// Get a random Eden image
	document.getElementById('Eden').style.opacity = 0.0;
	for (var i = 0; i < 10; ++i) {
		
		document.getElementById('Eden').src = "images/img_Eden" + i + ".png";
	}

	var eden = Math.floor(Math.random()*10)+1;
	document.getElementById('Eden').src = "images/img_Eden" + eden + ".png";
	document.getElementById('Eden').style.opacity = 1.0;
	document.getElementById('EdenThumb').src = "images/img_Eden" + eden + ".png";
	document.getElementById('Isaac').src = "images/img_Isaac.png";
	document.getElementById('Maggy').src = "images/img_Maggy.png";
	document.getElementById('Cain').src = "images/img_Cain.png";
	document.getElementById('Judas').src = "images/img_Judas.png";
	document.getElementById('???').src = "images/img_BlueBaby.png";
	document.getElementById('Eve').src = "images/img_Eve.png";
	document.getElementById('Samson').src = "images/img_Samson.png";
	document.getElementById('Azazel').src = "images/img_Azazel.png";
	document.getElementById('Lazarus').src = "images/img_Lazarus.png";
	document.getElementById('The Lost').src = "images/img_Lost.png";
	
	update();
}

function positionImages(img) {

	// Evenly space around the circle
	y = Math.cos(currentIndex*(Math.PI*2)/NUM_CHARACTERS);
    x = -Math.sin(currentIndex*(Math.PI*2)/NUM_CHARACTERS);
	
	px = x;
	py = y;
	
	// Rotate the circle by however many degrees necessary
	theta = thetaDeg*(Math.PI/180);
	cs = Math.cos(theta);
    sn = Math.sin(theta);
    px = x * cs + y * sn;
    py = x * sn - y * cs;
	
	// Turn the circle into an oval
	px*=1.5 *(windowWidth*0.0005);
	py*=0.3;
	
	// Scale the circle up for display
	px *= 200;
	py *= -200;
	

	img.style.opacity = (100+py)/100;
	
	// Set zindex so everything overlaps properly, then position them
	img.style.zIndex = parseInt(py);
	img.style.left = px + "px";
	img.style.top = py + "px";
	
	// Scale spirte based on depth
	final = (parseInt(img.style.top) *0.4);
	img.style.width = 196 + final + "px";//final+"px";
	
	var news = windowWidth*0.1;
	//newSize *= img.style.width;
	img.style.width = news + "px";
	//img.style.height = final+"px";
	img.style.left = -parseFloat(img.style.left) - parseFloat(img.style.width)/2+"px";//-(parseInt(img.style.left) - final) + "px";
	currentIndex++;	
}

function update(){

	// Change spin speed based on elapsed frames
	spinCounter++;
	if (spinCounter < 100) {
		if (spinSpeed < 10)
		spinSpeed += 0.1;
	}
	else { // If we're gone on for so long, start slowing
		if (spinSpeed > 2.5)
			spinSpeed -= 0.1;
		
		// If we hit the character we want to select
		if (spinSpeed < 3 && Math.abs(thetaDeg-targetTheta) < 3)
		{
			isSpinning = false;
			spinSpeed = 0;
			thetaDeg = targetTheta;
			
			document.getElementById('btnSpin').value = characters[rollResult] + "\nRoll again?";
			window.clearInterval(spinInterval); // remove update calls
		}
	}

	// Spin our circle and correct it when necessary
	thetaDeg += spinSpeed;
	if (thetaDeg > 360)
		thetaDeg -= 360;
	
	// Now that we've changed theta, reposition everything
	positionAll();
}

function positionAll() {
	currentIndex = 0;
	positionImages(document.getElementById('Isaac'));
	positionImages(document.getElementById('Maggy'));
	positionImages(document.getElementById('Cain'));
	positionImages(document.getElementById('Judas'));
	positionImages(document.getElementById('???'));
	positionImages(document.getElementById('Eve'));
	positionImages(document.getElementById('Samson'));
	positionImages(document.getElementById('Azazel'));
	positionImages(document.getElementById('Lazarus'));
	positionImages(document.getElementById('Eden'));
	positionImages(document.getElementById('The Lost'));
}

// Roll for a character
function onSpinClick() {
	if (isSpinning)
		return;
	
	currentFontSize=100;
	var fontz = baseFontSize*currentFontSize;
	document.getElementById('btnSpin').style.fontSize = fontz + "px";
	
	// Count up how many choices we have
	var possibleCount = 0;
	
	for (var i = 0; i < NUM_CHARACTERS; ++i) {
		if (isSelectable[i])
			possibleCount++;
	}
	
	// If no characters are left as options, we can't do anything
	if (possibleCount === 0) {
		document.getElementById('btnSpin').value = "Select\nCharacters";
		return;
	}
	
	isSpinning = true;
	spinCounter = 0;
	spinSpeed = 0;
	
	// Roll based on how many choices we have
	// Then find the character's place in degrees at the edge of the circle
	rollResult = Math.floor(Math.random()*possibleCount);
	
	var tempCount = 0;
	for (var i = 0; i < NUM_CHARACTERS; ++i) {
		if (rollResult === tempCount && isSelectable[i])
			rollResult = i;
		else if (isSelectable[i])
			tempCount++;
	}
	
	targetTheta = (rollResult/NUM_CHARACTERS)*360;
	spinInterval = setInterval(update, 30);
	

	document.getElementById('btnSpin').value = " ";
}

// Character (de)selection
function onIconClick(e) {
	if (isSpinning)
		return;
	
	
	// Get the triggered element
	if (!e)
        e = window.event;
    var sender = e.srcElement || e.target;
	// Find the triggered element and change its opacity
    for (var i = 0; i < NUM_CHARACTERS; ++i) {
		if (sender.id == characters[i]+"Thumb")
		{
			isSelectable[i] = !isSelectable[i];

			if (isSelectable[i])
				document.getElementById(sender.id).style.opacity = 1.0;
			else
				document.getElementById(sender.id).style.opacity = 0.3;
			break;
		}
	}	
}

// Change things based on window size
function getWidthOnScale() {
	// Store the current w/h values
	windowWidth = document.body.clientWidth;
	windowHeight = document.body.clientHeight;
	
	// Scale the font
	baseFontSize = windowWidth*0.0004;
	document.getElementById('btnSpin').style.fontSize = baseFontSize*currentFontSize+"px";
	
	// Scale the character spinner
	var newCharHeight = windowWidth/7;
	if (newCharHeight < 140)
		newCharHeight = 140;
	
	document.getElementById('btnHolder').style.height = newCharHeight*0.65+"px";
	document.getElementById('characters').style.height = newCharHeight+"px";

	// Change the button size based on text contents
	if (document.getElementById('btnSpin').value === "Roll")
		currentFontSize = 200;
	else
		currentFontSize=100;
	
	if (windowWidth < 1000)
		document.getElementById('selectionRow').style.width = 320+"px";
	else
		document.getElementById('selectionRow').style.width = 1000+"px";
	
	var fontz = baseFontSize*currentFontSize;
	document.getElementById('btnSpin').style.fontSize = fontz + "px";
	
	positionAll();
}

window.onload = mainFunc;

window.onresize = getWidthOnScale;
