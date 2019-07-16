#include <queue>

class CScrollRoller
  {
  public:
    CScrollRoller ();
    ~CScrollRoller ();
    void Run ();
  private:
    bool BuildGeometry ();
    bool LoadTextures  ();
  private:
    std::queue <int> m_queueList; 
  };



/*

6 элементов
if (n == 1)
   */