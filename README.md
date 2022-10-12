# flutter_desktop

Flutter plugin to better interact with the wayland and x11 display server stacks.
Currently it can fetch windowHandles (XID) on x11 windows. This is neat when using
dbus + xdg-desktop-portals. There has been an attempt at doing this with wayland 
using xdg_foreign but not tested.