#include "Image.h"

static const
GLenum cube_map_parts[6] = {    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                           };



Image::~Image()
{
}

Image::Image()
{
    imageID = -1; // wiadomo
    width = 0;
    height = 0;
}

Image::Image(const char *filename)
{
    imageID = imgToTexture(filename);
}

Image::Image(      const std::string& posXFilename,
                   const std::string& negXFilename,
                   const std::string& posYFilename,
                   const std::string& negYFilename,
                   const std::string& posZFilename,
                   const std::string& negZFilename)
{
    std::string* files = new std::string[6];
    files[0] = posXFilename;
    files[1] = negXFilename;
    files[2] = posYFilename;
    files[3] = negYFilename;
    files[4] = posZFilename;
    files[5] = negZFilename;

    glGenTextures(1, &imageID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, imageID);

    for (unsigned int i = 0 ; i < 6 ; i++)
    {

        GLenum format;
        SDL_Surface* surface = &imgToSurface(files[i], format);

        if (!surface || (format == -1))
        {
            std::cerr << "error: blad przy wczytywaniu textury kubicznej" << std::endl;
            imageID = -1;
            return;
        }

        glTexImage2D(cube_map_parts[i], 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

unsigned int Image::imgToTexture(const char *filename)
{

    SDL_Surface *surface = IMG_Load(filename); // Wczytanie obrazka z pliku, funkcje dostarcza SDL2_Image
    // Surface to format przechowywania tekstury w SDL
    if(!surface)
    {
        fprintf(stderr, "error: bynajmniej nie jest mo¿liwe otworzenie grifiki: \"%s\"\n", filename);
        return -1;
    }

    // sprawdzanie formatu pixeli, dozwolone tylko pliki 24 i 32 bitowe, do reszty trzeba napisaæ converter, co go usun¹³em bo niezmienionej formie nie dzia³a³ w SDL2...

    GLenum format;
    switch (surface->format->BytesPerPixel)  	// wybranie dormatu, palety kolorów
    {
    case 4:
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;		// jeœli kolor czerwony na najm³odszych bitach, to RGBA, inaczej BGRA...
        break;
    case 3:
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
        break;
    default:
        fprintf(stderr, "error: format pliku nie jest poprawny dla tak prostego loadera tekstur! bit/px = \"%i\"\n", surface->format->BytesPerPixel);
        return -1;
    }



    // Create texture.
    unsigned int texture_id = ~0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h,
                 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    //glGenerateMipmap(GL_TEXTURE_2D);

    //glGenSamplers(1, &sampler);

    printf("info: imgToTexture loaded texture \"%s\" as %u\n", filename, texture_id);

    width = surface->w;
    height = surface->h;

    // Done
    SDL_FreeSurface(surface);		// czyszczenie pamiêci ze zbêdnej grafiki
    return texture_id; 				// zwrócenie indeksu taktury za³adowanej do karty graficznej
}



SDL_Surface& Image::imgToSurface(const std::string& filename, GLenum& format)
{

    SDL_Surface *surface = IMG_Load(filename.c_str()); // Wczytanie obrazka z pliku, funkcje dostarcza SDL2_Image
    // Surface to format przechowywania tekstury w SDL
    if(!surface)
    {
        fprintf(stderr, "error: bynajmniej nie jest mo¿liwe otworzenie grifiki: \"%s\"\n", filename.c_str());
    }

    switch (surface->format->BytesPerPixel)  	// wybranie dormatu, palety kolorów
    {
    case 4:
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;		// jeœli kolor czerwony na najm³odszych bitach, to RGBA, inaczej BGRA...
        break;
    case 3:
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
        break;
    default:
        fprintf(stderr, "error: format pliku nie jest poprawny dla tak prostego loadera tekstur! bit/px = \"%i\"\n", surface->format->BytesPerPixel);
    }

    printf("info: loaded surface \"%s\"\n", filename.c_str());

    return *surface; 				// zwrócenie indeksu taktury za³adowanej do karty graficznej
}






