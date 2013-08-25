package initialTesting;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class HIDManager implements KeyListener {

	/**
	 * Lists the four arrow keys in order of: top, left, bottom, right
	 */
	boolean[] arrowsPressed = new boolean[4];
	
	public HIDManager(){
		for(int i=0; i<arrowsPressed.length; i++)
			arrowsPressed[i]=false;
	}
	
	@Override
	public void keyPressed(KeyEvent e) {
		keyChanged(e, true);
	}

	@Override
	public void keyReleased(KeyEvent e) {
		keyChanged(e, false);
	}

	private void keyChanged(KeyEvent e, boolean isDown) {
		int c = e.getKeyCode();
		if(c == KeyEvent.VK_UP)
			arrowsPressed[0] = isDown;
		if(c == KeyEvent.VK_LEFT)
			arrowsPressed[1] = isDown;
		if(c == KeyEvent.VK_DOWN)
			arrowsPressed[2] = isDown;
		if(c == KeyEvent.VK_RIGHT)
			arrowsPressed[3] = isDown;
	}

	@Override
	public void keyTyped(KeyEvent arg0) {
		// TODO Auto-generated method stub
	}

}
