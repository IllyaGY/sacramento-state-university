package Main;

import java.awt.Color;
import java.awt.Point;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;

import Data.Sprite;
import Data.Vector2D;
import Data.spriteInfo;
import FileIO.EZFileRead;
import Main.Collision;
import logic.Control;
import timer.stopWatchX;


public class Main {
	// Fields (Static) below...
	public static Color c = new Color(100, 0, 0);
	public static stopWatchX timer = new stopWatchX(70);
	public static spriteInfo sprite;
	public static ArrayList<spriteInfo> sprites = new ArrayList<>();
	public static int currentSpriteIndex = 0;
	public static Vector2D coords = new Vector2D();
	public static int countWalks = 0; 
	public static int prevAnim = 0; 
	public static spriteInfo currentSprite;
	public static String message = ""; 
	//public static HashMap<int[], String> objectCounter = new ArrayList<int[], String>();
	public static ArrayList<Collision> objects = new ArrayList<Collision>();
	// End Static fields...
	public static int x; 
	public static int y; 
	public static int ending = 0; 
	public static boolean activeObj = false; 
	public static String c1 = "False"; 

	public static void main(String[] args) {
		Control ctrl = new Control(); // Do NOT remove!
		ctrl.gameLoop(); // Do NOT remove!
	}


	/* This is your access to things BEFORE the game loop starts */
	public static void start() {
		// TODO: Code your starting conditions here...NOT DRAW CALLS HERE! (no addSprite or drawString) 
		coords.setCoords(1200,550);
		x = coords.getX(); 
		y = coords.getY(); 
		for(int i = 0; i < 16; i++)
			sprites.add(new spriteInfo(new Vector2D(coords.getX(),coords.getY()), "y"+i));
		loadObjCoords();
		sprites.add(new spriteInfo(new Vector2D(1200,50), "horse1"));


	}

	/* This is your access to the "game loop" (It is a "callback" method from the Control class (do NOT modify that class!))*/
	public static void update(Control ctrl) {
		// TODO: This is where you can code! (Starting code below is just to show you how it works)

		if(timer.isTimeUp()) {

			sprites.get(currentSpriteIndex).setCoords(coords);
			currentSprite = sprites.get(currentSpriteIndex);
			timer.resetWatch(); 
			if(prevAnim == currentSpriteIndex && countWalks > 1) currentSpriteIndex = standPos(currentSpriteIndex);
			else if(prevAnim == currentSpriteIndex && countWalks < 2) countWalks++;
			else countWalks = 0;
			prevAnim = currentSpriteIndex;
			if(ending == 1) {
				if(objects.size() >= 5)
					objects.remove(4) ;
			}
			for (Collision obj : objects) {

				if(obj.radiusCheck(currentSprite.getCoords().getX(), currentSprite.getCoords().getY())) {
					if(obj.getUse()) {
						activeObj = true; 
						message = obj.getMessage();					
					}
					if (obj.collisionCheck(currentSprite.getCoords().getX(), currentSprite.getCoords().getY())) {
						currentSprite.setCoords(x,y);
						c1 = "True"; 
						break;

					}

					
				}
				
				else {
					activeObj = false; 
					message = ""; 
				}

			}
			x=coords.getX();
			y=coords.getY();

		}
		ctrl.addSpriteToFrontBuffer(0, 0, "bg");

		ctrl.drawString(700, 700, message , new Color(255,0,0));

		ctrl.drawString(20, 20, currentSprite.getCoords().getX() + " "  + currentSprite.getCoords().getY() , new Color(255,0,0));


		for(Collision obj : objects) ctrl.addSpriteToFrontBuffer(obj.getLocation_TL().x, obj.getLocation_TL().y, obj.getTag());

		if(ending == 0) ctrl.addSpriteToFrontBuffer(currentSprite.getCoords().getX(), currentSprite.getCoords().getY(), currentSprite.getTag());
		else ctrl.addSpriteToFrontBuffer(sprites.get(sprites.size()-1).getCoords().getX(), sprites.get(sprites.size()-1).getCoords().getY(), sprites.get(sprites.size()-1).getTag());
	}
	public static int standPos(int currentSpriteIndex) {
		if(currentSpriteIndex < 4) return 0;
		else if(currentSpriteIndex < 8) return 4; 
		else if(currentSpriteIndex < 12) return 8; 
		else return 12;	
	}
	public static void loadObjCoords() {
		EZFileRead ezr = new EZFileRead("Objects.txt");
		for(int i = 0; i < ezr.getNumLines(); i++){
			int radius = 0;
			String message = ""; 
			String raw = ezr.getLine(i);
			StringTokenizer st = new StringTokenizer(raw, "*");
			String tag = st.nextToken();
			int coordX = Integer.parseInt(st.nextToken()); 
			int coordY = Integer.parseInt(st.nextToken()); 
			int dimX = Integer.parseInt(st.nextToken()); 
			int dimY = Integer.parseInt(st.nextToken());
			boolean usable = (Integer.parseInt(st.nextToken())==1?true : false); 
			if(usable) { 
				radius = Integer.parseInt(st.nextToken()); 
				message = st.nextToken();  
			}
			objects.add(new Collision(tag, new Point(coordX,coordY), dimX,dimY,usable, radius,message));
		}
	}
}
