package edittool;

import java.awt.BorderLayout;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import java.awt.GridBagLayout;
import java.awt.geom.Point2D;

import javax.imageio.ImageIO;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JTextField;
import javax.swing.JComboBox;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.TreeSet;
import java.awt.event.ActionEvent;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;

public class Window extends JFrame {
	
	private JTextField txtDensity;
	private JTextField txtFriction;
	private JTextField txtRestitution;
	private JTextField txtFixtureType;
	private BufferedImage image;
	private PaintPanel paintPanel;
	private JTextField txtTextureName;
	private ArrayList<Module> modules;
	private ModulePaintPanel modulePaintPanel;
	private TreeSet<Integer> idList = new TreeSet<Integer>();
	private JComboBox<Integer> comboBoxModules;
	private int imageWidth;
	private int imageHeight;
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Window frame = new Window();
					//frame.setGlassPane(modulePaintPanel);
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public Window() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 495, 419);
		getContentPane().setLayout(null);
		
		addLabels();
		addTextFields();
		
		
		comboBoxModules = new JComboBox();
		comboBoxModules.setBounds(345, 87, 108, 23);
		getContentPane().add(comboBoxModules);
		
		for(int i=1;i<100;i++){
			idList.add(i);
			//idList.pollFirst();
		}
		
		
		JButton btnAddTexture = new JButton("Add Texture");
		btnAddTexture.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				paintPanel = new PaintPanel();
				paintPanel.setBackground(Color.WHITE);
				paintPanel.setBounds(10, 11, 300, 300);
				getContentPane().add(paintPanel);
				try {
					String imagePath = new String();
					txtTextureName.setText("statek.png");  //narazie
					imagePath = txtTextureName.getText();
					if(imagePath.equals("")||imagePath.equals("PODAJ NAZWE!!")){
						txtTextureName.setText("PODAJ NAZWE!!");
					}
					else{
						image = ImageIO.read(new File("textures/"+imagePath));
						imageWidth = image.getWidth();
						imageHeight = image.getHeight();
						paintPanel.setImage(image);
						paintPanel.repaint();
						modulePaintPanel = new ModulePaintPanel(); 
						modulePaintPanel.setImageHeight(imageHeight);
						modulePaintPanel.setImageWidth(imageWidth);
						modulePaintPanel.setBounds(10, 11, 300, 300);
						getContentPane().add(modulePaintPanel);
						getContentPane().setComponentZOrder(modulePaintPanel, 0);
						modulePaintPanel.setBlock(true);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}  
			}
		});
		btnAddTexture.setBounds(10, 322, 114, 23);
		getContentPane().add(btnAddTexture);
		
		JButton btnStartPaint = new JButton("Start");
		btnStartPaint.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if(image!=null&&modulePaintPanel.getBlock()==true){
					modulePaintPanel.initializePoint2Ds();
					modulePaintPanel.setBlock(false);
					modulePaintPanel.repaint();
				}
				else
				{
					modulePaintPanel.setBlock(true);
					modulePaintPanel.delete();
					modulePaintPanel.repaint();
				}
			}
		});
		btnStartPaint.setBounds(314, 25, 76, 23);
		getContentPane().add(btnStartPaint);
		
		JButton btnEndPaint = new JButton("End");
		btnEndPaint.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(modulePaintPanel.getPoint2Ds()!=null){
					modulePaintPanel.correctPoint2Ds();
					modulePaintPanel.repaint();
					modulePaintPanel.setBlock(true);
				}
			}			
		});
		btnEndPaint.setBounds(396, 25, 83, 23);
		getContentPane().add(btnEndPaint);
		
		JButton btnAddModule = new JButton("Add");
		btnAddModule.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(modulePaintPanel.getBlock()==true&&modulePaintPanel.getPoint2Ds()!=null){
					int id = idList.pollFirst();
					modulePaintPanel.addModuleToContainer(id,txtDensity.getText(),txtFriction.getText(),txtRestitution.getText(),txtFixtureType.getText());
					comboBoxModules.addItem(id);
					System.out.println("dodano modul"+id);
					modulePaintPanel.delete();
				}
			}
		});
		btnAddModule.setBounds(314, 53, 76, 23);
		getContentPane().add(btnAddModule);
		
		JButton btnRemoveModule = new JButton("Remove");
		btnRemoveModule.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(comboBoxModules.getSelectedIndex()!=-1){
					int id = ((int)comboBoxModules.getSelectedItem());
					if(modulePaintPanel.removeModel(id)==true){
						comboBoxModules.removeItemAt(comboBoxModules.getSelectedIndex());
						comboBoxModules.repaint();
						System.out.println("usunieto obiekt"+id);
						idList.add(id);
						modulePaintPanel.repaint();
					}
					else
						System.out.println("nie usunieto obiektu"+id);
				}
			}
		});
		btnRemoveModule.setBounds(396, 53, 83, 23);
		getContentPane().add(btnRemoveModule);
		
		JButton btnSaveToXml = new JButton("Save to XML");
		btnSaveToXml.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				modulePaintPanel.saveToXML();
			}
		});
		btnSaveToXml.setBounds(355, 347, 114, 23);
		getContentPane().add(btnSaveToXml);
	}
	
	private void addTextFields() {
		txtTextureName = new JTextField();
		txtTextureName.setBounds(10, 361, 114, 20);
		getContentPane().add(txtTextureName);
		txtTextureName.setColumns(10);
		
		txtDensity = new JTextField();
		txtDensity.setBounds(355, 138, 86, 20);
		getContentPane().add(txtDensity);
		txtDensity.setColumns(10);
		
		txtFriction = new JTextField();
		txtFriction.setBounds(355, 188, 86, 20);
		getContentPane().add(txtFriction);
		txtFriction.setColumns(10);
		
		txtRestitution = new JTextField();
		txtRestitution.setBounds(355, 238, 86, 20);
		getContentPane().add(txtRestitution);
		txtRestitution.setColumns(10);
		
		txtFixtureType = new JTextField();
		txtFixtureType.setBounds(355, 291, 86, 20);
		getContentPane().add(txtFixtureType);
		txtFixtureType.setColumns(10);
	}

	private void addLabels(){
		JLabel lblModule = new JLabel("MODULE");
		lblModule.setFont(new Font("Tahoma", Font.BOLD, 11));
		lblModule.setForeground(Color.RED);
		lblModule.setBounds(367, 11, 48, 14);
		getContentPane().add(lblModule);
		
		JLabel lblTextureName = new JLabel("Texture Name");
		lblTextureName.setBounds(10, 347, 100, 14);
		getContentPane().add(lblTextureName);
		
		JLabel lblDensity = new JLabel("density");
		lblDensity.setBounds(355, 121, 46, 14);
		getContentPane().add(lblDensity);
		
		JLabel lblFriction = new JLabel("friction");
		lblFriction.setBounds(355, 169, 46, 14);
		getContentPane().add(lblFriction);
		
		JLabel lblRestitution = new JLabel("restitution");
		lblRestitution.setBounds(355, 225, 63, 14);
		getContentPane().add(lblRestitution);
		
		JLabel lblFixturetype = new JLabel("fixture_type");
		lblFixturetype.setBounds(355, 277, 86, 14);
		getContentPane().add(lblFixturetype);
	}
}
