package edittool;

import java.util.ArrayList;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement( name = "MODULES" )
@XmlAccessorType(XmlAccessType.FIELD)
public class Modules {
	@XmlElement( name = "MODULE" )
	public ArrayList<Module> modules;

	public Modules(){
		this.modules = new ArrayList<Module>();
	}
	
	
	public ArrayList<Module> getModules() {
		return this.modules;
	}
	
	
	public void setModules(ArrayList<Module> modules) {
		this.modules = modules;
	}
	
	public void add(Module module){
			modules.add(module);

	}
	
	public boolean remove(Module module){
		//if(modules.size()>1){
			modules.remove(module);
			return true;
		//}
		/*else if(modules.size()==1)
		{
			modules.clear();
			modules = null;
			return true;
		} */
		//else
			//return false;
	}
	
}
