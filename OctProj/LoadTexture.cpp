#pragma once
#include "stdafx.h"

#include "LoadTexture.h" 

///////////////////////////////////////////////////
CLoadTexture::CLoadTexture (int nTotalTextures)
  {
  m_vgluTexture.assign (nTotalTextures, 0);
  }
///////////////////////////////////////////////////
CLoadTexture::~CLoadTexture ()
  {
  }
///////////////////////////////////////////////////
bool CLoadTexture::LoadTexture (CString sTextureName, GLuint &gluTex)
  {
  AUX_RGBImageRec *pTexture;
  pTexture = auxDIBImageLoad (sTextureName);
  if (pTexture)
    {
    glBindTexture (GL_TEXTURE_2D, gluTex);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D, 0, 4, pTexture->sizeX, pTexture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture->data);

    return true;
    }
  return false; 
  }
///////////////////////////////////////////////////
bool CLoadTexture::LoadTexture (CString sTextureName)
  {
  AUX_RGBImageRec *pTexture;
  pTexture = auxDIBImageLoad (sTextureName);
  if (pTexture)
    {
    glBindTexture (GL_TEXTURE_2D, m_vgluTexture[m_nNextTexture++]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D, 0, 4, pTexture->sizeX, pTexture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pTexture->data);

    return true;
    }
  return false; 
  }
///////////////////////////////////////////////////
//void CLoadTexture::AddTexture (GLuint *gluTex)
//  {
//  bool bSucceed (false);
//  int n(0);
//  while (m_vgluTexture[n++] != 0);
//    {
//    if (m_vgluTexture.size () == n)
//      return (bSucceed);
//    }
//  m_vgluTexture.push_back (gluTex);
//  bSucceed = true;
//  return bSucceed;
//  }
///////////////////////////////////////////////////
//bool CLoadTexture::AddLoadedTexture (CString sTextureName)
//  {
//  GLuint gluTex;
//  if (LoadTexture (sTextureName, gluTex))
//    {
//    m_vgluTexture.push_back (&gluTex);
//    return true;
//    }
//  else
//    return false;
//  }
/////////////////////////////////////////////////////
const std::vector <GLuint>& CLoadTexture::GetTextures ()
  {
  return m_vgluTexture;
  }
///////////////////////////////////////////////////