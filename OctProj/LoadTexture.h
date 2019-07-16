#include <atlstr.h>
#include <vector>

#include "glaux.h"

class CLoadTexture
  {
  public:
    CLoadTexture (int nTotalTextures);
    ~CLoadTexture ();

    bool LoadTexture (CString sTextureName, GLuint &gluTex);
    bool LoadTexture (CString sTextureName);
    //void AddTexture (GLuint *gluTex);
    //bool AddLoadedTexture (CString sTextureName);
    const std::vector <GLuint>& GetTextures ();
  private:
    std::vector <GLuint> m_vgluTexture;
    int m_nNextTexture;
  };