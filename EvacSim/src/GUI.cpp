#include "GUI.h"

GUI::GUI()
{
    myRenderer = &CEGUI::OpenGLRenderer::create();
    CEGUI::System::create( *myRenderer );
    CEGUI::GUIContext(*myRenderer);

    // initialise the required dirs for the DefaultResourceProvider
    rp = static_cast<CEGUI::DefaultResourceProvider*>
         (CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "data/gui/schemes/");
    rp->setResourceGroupDirectory("imagesets", "data/gui/imagesets/");
    rp->setResourceGroupDirectory("fonts", "data/gui/fonts/");
    rp->setResourceGroupDirectory("layouts", "data/gui/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "data/gui/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "data/gui/lua_scripts/");
    // This is only really needed if you are using Xerces and need to
    // specify the schemas location
    rp->setResourceGroupDirectory("schemas", "data/gui/xml_schemas/");

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");

    CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
    CEGUI::System::getSingleton().getDefaultGUIContext().
    setDefaultTooltipType( "TaharezLook/Tooltip" );


}

GUI::~GUI()
{}

int GUI::init()
{

}

int GUI::createTest()
{
    using namespace CEGUI;
    WindowManager& wmgr = WindowManager::getSingleton();
    Window* myRoot = wmgr.createWindow( "DefaultWindow", "root" );



    //Window* myRoot = wmgr.loadLayoutFromFile("Console.layout", "layouts");
    System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
    fWnd = static_cast<FrameWindow*>(
               wmgr.createWindow( "TaharezLook/FrameWindow", "testWindow" ));

    myRoot->addChild( fWnd );

    // position a quarter of the way in from the top-left of parent.
    fWnd->setPosition( UVector2( UDim( 0.2f, 0.0f ), UDim( 0.6f, 0.0f ) ) );
    // set size to be half the size of the parent
    fWnd->setSize( USize( UDim( 0.6f, 0.0f ), UDim( 0.2f, 0.0f ) ) );
    fWnd->setText( "Panel Zajebistoœci " );
    fWnd->setTooltipText( "This is an example tooltip" ) ;

    Window & firstLabel = * wmgr.createWindow( "TaharezLook/StaticText",
                          "MyFirstLabel" ) ;

    fWnd->addChild( &firstLabel ) ;

    firstLabel.setProperty( "FrameEnabled", "true" ) ;
    firstLabel.setProperty( "BackgroundEnabled", "true" ) ;

    firstLabel.setPosition( UVector2( cegui_reldim(0.02f),
                                      cegui_reldim(0.2f) ) ) ;
    USize a;
//firstLabel.set
    firstLabel.setSize( a ) ;
    firstLabel.setText( "A (static) label" ) ;


}

void GUI::renderTest()
{
    std::stringstream a;
    a.str();
    fWnd->setText( "DEBUG WINDOW" );

    fWnd->setAlpha(0.6);
}
//dtor

