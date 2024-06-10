package Main;

import java.awt.Point;

public class Collision {
	boolean usable;
	int dimX,dimY;
	Point coords;
	String tag;
	int radius;
	String message; 
	public Collision(String tag, Point coords, int dimX, int dimY, boolean usable, int radius, String message) {
		this.usable = usable; 
		this.dimX = dimX;
		this.dimY = dimY;
		this.tag = tag; 
		this.coords = coords;
		this.radius = radius; 
		this.message = message; 
	}
	public void setLocX(int locX) {
		coords.x = locX; 
	}
	public void setLocY(int locY) {
		coords.y = locY; 
	}
	public boolean getUse() {
		return usable; 
	}
	public Point getLocation_TL() {
		return coords; 
	}
	public Point getLocation_RB() {
		return new Point(coords.x+dimX, coords.y+dimY);
	}
	public String getTag() {
		return tag; 
	}
	public String getMessage() {
		return message; 
	}
	public boolean collisionCheck(int _x, int _y) {  //Character's rectangle of 80x120 is compared to an object's rectangle
		if((_x > coords.x+dimX || _x+80 < coords.x) || (_y > coords.y+dimY || _y+120 < coords.y)) 			
			return false; 
		return true; 
	}
	public boolean radiusCheck(int _x, int _y) {    //Character's rectangle of 80x120 is compared to an object's usage rectangle
		if((_x > coords.x+dimX+radius || _x+80 < coords.x-radius) || (_y > coords.y+dimY+radius || _y+120 < coords.y-radius))
			return false;  
		return true;
	}

}	
