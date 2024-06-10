/* This will handle the "Hot Key" system. */

package Main;

import logic.Control;
import timer.stopWatchX;

public class KeyProcessor{
	// Static Fields
	private static char last = ' ';			// For debouncing purposes
	private static stopWatchX sw = new stopWatchX(250);

	// Static Method(s)
	public static void processKey(char key){
		if(key == ' ')				return;
		// Debounce routine below...
		if(key == last)
			if(sw.isTimeUp() == false)			return;
		last = key;
		sw.resetWatch();

		/* TODO: You can modify values below here! */

		switch(key){
		case '%':								// ESC key
			System.exit(0);
			break;

		case 'm':
			// For mouse coordinates
			Control.isMouseCoordsDisplayed = !Control.isMouseCoordsDisplayed;
			break;
		case 'w':
			change(15, 12, 12);
			Main.coords.adjustY(-20);
			break; 

		case 'a':
			change(7,4,4);
			Main.coords.adjustX(-20);
			break; 

		case 's':
			change(3,0, 0);
			Main.coords.adjustY(20);
			break; 

		case 'd':
			change(11,8,8);
			Main.coords.adjustX(20);
			break; 
		case '$': 
			if (Main.activeObj) Main.ending = 1; 
		}
		
			

	}
	public static void change(int upperLim, int lowerLim, int standPos) {
		Main.currentSpriteIndex++;
		Main.currentSpriteIndex = ((Main.currentSpriteIndex > upperLim) ? lowerLim : Main.currentSpriteIndex);
		Main.currentSpriteIndex = ((Main.currentSpriteIndex < lowerLim) ? upperLim : Main.currentSpriteIndex);
	}
}