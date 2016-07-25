package edittool;

import java.awt.geom.Point2D;
import java.util.ArrayList;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import javax.xml.bind.annotation.XmlList;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.adapters.XmlJavaTypeAdapter;

@XmlRootElement
@XmlAccessorType(XmlAccessType.FIELD)
public class Module {
	@XmlElement(name = "Density")
	private double density;
	@XmlElement(name = "Friction")
	private double friction;
	@XmlElement(name = "Restitution")
	private double restitution;
	@XmlElement(name = "Fixture_type")
	private double fixture_type;
	@XmlAttribute(name="id")
	private int id = 0;
	
	
	@XmlElementWrapper(name="points")
	@XmlElement(name="point")
	@XmlJavaTypeAdapter(MyPointAdapter.class)
	private ArrayList<Point2D> points = new ArrayList<Point2D>();
	
	@XmlElementWrapper(name="texture_coordinates")
	@XmlElement(name="texture-coordinate")
	@XmlJavaTypeAdapter(MyPointAdapter.class)
    private ArrayList<Point2D> texture_coordinates = new ArrayList<Point2D>();
	
	public Module(){
	}
	
	@Override
	public String toString(){
		return "Modul nr"+String.valueOf(id);
		
	}
	
	
	public double getDensity() {
		return density;
	}
	
	
	public void setDensity(double density) {
		this.density = density;
	}
	
	
	public double getFriction() {
		return friction;
	}
	
	
	public void setFriction(double friction) {
		this.friction = friction;
	}
	
	
	public double getRestitution() {
		return restitution;
	}
	
	
	public void setRestitution(double restitution) {
		this.restitution = restitution;
	}
	
	public double getFixture_type() {
		return fixture_type;
	}

	
	public void setFixture_type(double fixture_type) {
		this.fixture_type = fixture_type;
	}
	
	public ArrayList<Point2D> getPoint2Ds() {
		return points;
	}

	
	public void setPoint2Ds(ArrayList<Point2D> points) {
		this.points = points;
	}
	
	public ArrayList<Point2D> getTexture_coordinates() {
		return texture_coordinates;
	}

	
	public void setTexture_coordinates(ArrayList<Point2D> texture_coordinates) {
		this.texture_coordinates = texture_coordinates;
	}
	
	public int getId() {
		return id;
	}

	
	public void setId(int id) {
		this.id = id;
	}
}
