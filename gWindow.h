//Created by Duncan Klug on 4/13/17

typedef void (*onResizeWindow)();

class gWindow : public RenderWindow
{
  protected:
    // Function pointer that is called on window resize
    onResizeWindow orw;

  public:
    void onResize()
    {
      //TODO: implement
      orw();
    }

    void setResizeFunction (onResizeWindow orw)
    {
      this->orw = orw;
    }

};
