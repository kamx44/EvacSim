package edittool;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class PaintPanel extends JPanel {

	/**
	 * Create the panel.
	 */
	private BufferedImage image;
	
	
	public PaintPanel() {
		
	}
	
	public void setImage(BufferedImage image){
		this.image = image;
		
	}
	
	 @Override
	 protected void paintComponent(Graphics g) {
	     super.paintComponent(g);
	     g.drawImage(image, 0, 0, null); // see javadoc for more info on the parameters            
	 }

}
