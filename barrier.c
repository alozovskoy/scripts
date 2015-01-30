#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XInput2.h>

int dpy_w;
int dpy_h;

int barrier_x;
int barrier_y;
Display *dpy;
PointerBarrier barrier;
Window window;

static void
create_barrier (void)
{

//barrier = XFixesCreatePointerBarrier (dpy, window, 0, barrier_y, dpy_w, barrier_y, 0, 0, NULL); - for horizontal barrier
barrier = XFixesCreatePointerBarrier (dpy, window, barrier_x, 0, barrier_x, dpy_h, 0, 0, NULL); // for vertical barrier
}

int
main (int argc, char **argv)
{
    XEvent xev;
    XIEventMask mask;
    unsigned char mask_bits[XIMaskLen (XI_LASTEVENT)] = { 0 };

    dpy = XOpenDisplay(NULL);

    dpy_w = DisplayWidth (dpy, DefaultScreen (dpy));
    dpy_h = DisplayHeight (dpy, DefaultScreen (dpy));

    window = XCreateSimpleWindow (dpy, DefaultRootWindow(dpy),
                                  0, 0, dpy_w, dpy_h,
                                  0, BlackPixel(dpy, 0),
                                  WhitePixel(dpy, 0));

    barrier_y = 100;
    barrier_x = 100;

    create_barrier ();

    XISetMask (mask_bits, XI_BarrierHit);
    XISetMask (mask_bits, XI_BarrierLeave);
    mask.deviceid = XIAllMasterDevices;
    mask.mask = mask_bits;
    mask.mask_len = sizeof (mask_bits);
    XISelectEvents (dpy, window, &mask, 1);
    XSync(dpy, False);
 
    while (1) {}

}
