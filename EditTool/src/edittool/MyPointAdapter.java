package edittool;

import java.awt.geom.Point2D;
import javax.xml.bind.annotation.adapters.XmlAdapter; 

public class MyPointAdapter extends XmlAdapter<String, Point2D> 
{ 
	@Override
    public String marshal(Point2D v) throws Exception {
        return String.format("%f;%f", v.getX(), v.getY());
    }

    @Override
    public Point2D unmarshal(String v) throws Exception {
        String[] coords = v.split(";");
        return new Point2D.Double(Double.parseDouble(coords[0]), Double.parseDouble(coords[1]));
    }
}