package edittool;


import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Point2D;
import java.io.File;
import java.util.ArrayList;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;

import javax.swing.JComponent;
//import javax.swing.JPanel;

public class ModulePaintPanel extends JComponent implements MouseListener{

	private int x, y;
	private boolean block;
	private int imageWidth;
	private int imageHeight;
	 
    private ArrayList<Point2D> points;
    private ArrayList<Point2D> texture_coordinates;
    private Modules modules = new Modules();
	/**
	 * Create the panel.
	 */
	public ModulePaintPanel() {
		addMouseListener(this);
		block = false;
	}
	
	public void initializePoint2Ds(){
		points = new ArrayList<Point2D>();
		texture_coordinates = new ArrayList<Point2D>();
	}
	@Override
	public void mouseClicked(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mousePressed(MouseEvent e) {
		if(block==false){
			 x = e.getX();
		     y = e.getY();
		     points.add(new Point2D.Double(x, y));
		     double txtcoordX = (double)x/(double)imageWidth;
		     double txtcoordY = (double)y/(double)imageHeight;
		     texture_coordinates.add(new Point2D.Double(txtcoordX,txtcoordY));
		     
		     repaint();
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
    public void paintComponent(Graphics g){
        Graphics2D g2d = (Graphics2D) g;
        g2d.setColor(new Color(255,0,0,200));
        if(points!=null){
        	drawPoint2Ds(g2d,points);
        	drawLines(g2d);
        }
        g2d.setColor(Color.RED);
        for(Module module : modules.getModules()){
        	drawPoint2Ds(g2d,module.getPoint2Ds());
        	drawModulesLines(g2d,module.getPoint2Ds());
        }
    }
	
	public void drawPoint2Ds(Graphics g,ArrayList<Point2D> fpoints){
		Graphics2D g2d = (Graphics2D) g;
		for(Point2D p : fpoints){
			int x = (int) p.getX();
			int y = (int) p.getY();
			g2d.fillOval(x-2, y-2, 4, 4);
		}
	}
	
	public void drawLines(Graphics g){ 
		Graphics2D g2d = (Graphics2D) g;
		for(int i=0;i<points.size()-1;i++){
			int x1 = (int) points.get(i).getX();
			int y1 = (int) points.get(i).getY();
			int x2 = (int) points.get(i+1).getX();
			int y2 = (int) points.get(i+1).getY(); 
			g2d.drawLine(x1, y1, x2, y2);
		}
		if(points.size()>2){
			if(block==false)
				g2d.setColor(Color.GREEN);
			else
				g2d.setColor(Color.RED);
			int last = points.size()-1;
			int x1 = (int) points.get(last).getX();
			int y1 = (int) points.get(last).getY();
			int x2 = (int) points.get(0).getX();
			int y2 = (int) points.get(0).getY(); 
			g2d.drawLine(x1, y1, x2, y2);
		}
	}
	
	public void drawModulesLines(Graphics g,ArrayList<Point2D> fpoints){  
		Graphics2D g2d = (Graphics2D) g;
		for(int i=0;i<fpoints.size()-1;i++){
			int x1 = (int) fpoints.get(i).getX();
			int y1 = (int) fpoints.get(i).getY();
			int x2 = (int) fpoints.get(i+1).getX();
			int y2 = (int) fpoints.get(i+1).getY(); 
			g2d.drawLine(x1, y1, x2, y2);
		}
		if(fpoints.size()>2){
			int last = fpoints.size()-1;
			int x1 = (int) fpoints.get(last).getX();
			int y1 = (int) fpoints.get(last).getY();
			int x2 = (int) fpoints.get(0).getX();
			int y2 = (int) fpoints.get(0).getY(); 
			g2d.drawLine(x1, y1, x2, y2);
		}
	}
	
	public void correctPoint2Ds(){
		for(Module module : modules.getModules()){
			ArrayList<Point2D> points2 = module.getPoint2Ds();
			for(Point2D point2 : points2){
				for(int i=0;i<points.size();i++){
					if(distance(points.get(i).getX(),points.get(i).getY(),point2.getX(),point2.getY())<10){
						points.set(i, point2);
						System.out.println("zmiana");
					}
				}
			}
		}
	}
	
	private double distance(double x1,double y1,double x2, double y2){
		return Math.sqrt((Math.pow((x2-x1), 2))+(Math.pow((y2-y1), 2)));
	}
	
	public void addModuleToContainer(int id,String density,String friction,String restitution,String fixture) {
		Module module = new Module();
		if(density.equals(""))
			module.setDensity(0);
		else
			module.setDensity(Integer.parseInt(density));
		if(friction.equals(""))
			module.setFriction(0);
		else
			module.setFriction(Integer.parseInt(friction));
		if(restitution.equals(""))
			module.setRestitution(0);
		else
			module.setRestitution(Integer.parseInt(restitution));
		if(fixture.equals(""))
			module.setFixture_type(0);
		else
			module.setFixture_type(Integer.parseInt(fixture));
		module.setId(id);
		module.setPoint2Ds(points);
		module.setTexture_coordinates(texture_coordinates);
		modules.add(module);
	}
	
	public void saveToXML(){
		try {
			File file = new File("C:\\x\\file.xml");
			
				JAXBContext jaxbContext;
				jaxbContext = JAXBContext.newInstance(Modules.class);
				Marshaller jaxbMarshaller = jaxbContext.createMarshaller();
				// output pretty printed
				jaxbMarshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
				jaxbMarshaller.marshal(modules, file);
				jaxbMarshaller.marshal(modules, System.out);
			
		} catch (JAXBException e1) {
				// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		for(Module module : modules.getModules()){
		for(Point2D point: module.getPoint2Ds()){
			System.out.println(point.getX()+" "+point.getY());
		}
		for(Point2D tex: module.getTexture_coordinates()){
			System.out.println(tex.getX()+" "+tex.getY());
		}
		}
	}

	public boolean removeModel(int id){
		for(Module mod : modules.getModules()){
			if(mod.getId()==id){
				modules.remove(mod);
				mod.setPoint2Ds(null);
				mod=null;
				return true;
			}
		}
		return false;
	}
	
	public void setBlock(boolean block){
		this.block=block;
	}
	
	public boolean getBlock(){
		return block;
	}
	
	public ArrayList<Point2D> getPoint2Ds(){
		return points;
	}
	
	public void setPoint2Ds(ArrayList<Point2D> points){
		this.points = points;
	}
	
	public void delete(){
		this.points = null;
		this.texture_coordinates = null;
	}
	
	public int getImageWidth() {
		return imageWidth;
	}
	
	public void setImageWidth(int imageWidth) {
		this.imageWidth = imageWidth;
	}
	
	public int getImageHeight() {
		return imageHeight;
	}
	
	public void setImageHeight(int imageHeight) {
		this.imageHeight = imageHeight;
	}
	
	public ArrayList<Point2D> getTexture_coordinates() {
		return texture_coordinates;
	}

}
