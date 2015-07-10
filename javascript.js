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
	document.getElementById('isJsEnabled').innerHTML = "";
	document.getElementById('characters').style.height = 300+"px";
	
	for (var i = 0; i < NUM_CHARACTERS; ++i) {
		isSelectable.push(true);
		isSelectable[i]=true;
	}
	
	var eden = Math.floor(Math.random()*10)+1;
	document.getElementById('Eden').src = "images/img_Eden" + eden + ".png";
	
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
	px*=1.3;
	py*=0.25;
	
	// Scale the circle up for display
	px *= -300;
	py *= -300;
	
	// Set zindex so everything overlaps properly, then position them
	img.style.zIndex = parseInt(py);
	img.style.left = -80+px + "px";
	img.style.top = py + "px";
	
	// Scale spirte based on depth
	final = 120+(parseInt(img.style.top) *0.5);
	img.style.width = final+"px";
	img.style.height = final+"px";
	
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
	
	document.getElementById('btnSpin').style.fontSize = 96 + "px";
	document.getElementById('btnSpin').value = "";
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
		if (sender.id == characters[i])
		{
			isSelectable[i] = !isSelectable[i];

			if (isSelectable[i])
				document.getElementById(sender.id).style.opacity = 1.0;
			else
				document.getElementById(sender.id).style.opacity = 0.6;
			break;
		}
	}	
}

window.onload = mainFunc;
