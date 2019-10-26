// halcyon_application.h

#ifndef HALCYON_APPLICATION_H_INCLUDED
#define HALCYON_APPLICATION_H_INCLUDED

struct mouse_device;
struct keyboard_device;

struct application
{
   application() = default;
   virtual ~application() = default;

   virtual bool init() = 0;
   virtual bool tick(double app_time, 
                     double frame_time, 
                     const keyboard_device *keyboard, 
                     const mouse_device *mouse) = 0;
   virtual void render() = 0;
   virtual void exit() = 0;
};

application *create_application(int argc, char **argv);

#endif // HALCYON_APPLICATION_H_INCLUDED
