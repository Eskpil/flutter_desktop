#include "include/flutter_desktop/flutter_desktop_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkwayland.h>
#include <sys/utsname.h>

#include <cstring>

#define FLUTTER_DESKTOP_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), flutter_desktop_plugin_get_type(), \
                              FlutterDesktopPlugin))

struct _FlutterDesktopPlugin {
  GObject parent_instance;

  FlPluginRegistrar* registrar;
  gchar *handle;
};

G_DEFINE_TYPE(FlutterDesktopPlugin, flutter_desktop_plugin, g_object_get_type())

#ifdef GDK_WINDOWING_WAYLAND
static void
on_wayland_exported (GdkWindow  *window,
                     const char *handle,
                     gpointer    user_data)
{
  FlutterDesktopPlugin *self = (FlutterDesktopPlugin*)user_data;
  self->handle = g_strdup_printf("%s", handle);
}
#endif


// Called when a method call is received from Flutter.
static void flutter_desktop_plugin_handle_method_call(
    FlutterDesktopPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else if (strcmp(method, "getWindowHandle") == 0) {
    FlView* view = fl_plugin_registrar_get_view(self->registrar);
    if (!view) {
      response = FL_METHOD_RESPONSE(fl_method_error_response_new("200", "View is null", nullptr));
      return;
    }
    GtkWindow* window = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(view)));
    GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window));

#ifdef GDK_WINDOWING_X11
    if (display && GDK_IS_X11_DISPLAY (display)) {
      gchar *caller_id = g_strdup_printf ("%lu", (gulong)GDK_WINDOW_XID (gtk_widget_get_window(GTK_WIDGET(window))));
      self->handle = caller_id;
    }
#endif
#ifdef GDK_WINDOWING_WAYLAND
	  if (display && GDK_IS_WAYLAND_DISPLAY (display)) {
		  gdk_wayland_window_export_handle (gtk_widget_get_window (GTK_WIDGET(window)), on_wayland_exported, self, NULL);
		  gdk_display_sync (display);
	  }
#endif

    g_autoptr(FlValue) result = fl_value_new_string(self->handle);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  } 

  fl_method_call_respond(method_call, response, nullptr);
}

static void flutter_desktop_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(flutter_desktop_plugin_parent_class)->dispose(object);
}

static void flutter_desktop_plugin_class_init(FlutterDesktopPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = flutter_desktop_plugin_dispose;
}

static void flutter_desktop_plugin_init(FlutterDesktopPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  FlutterDesktopPlugin* plugin = FLUTTER_DESKTOP_PLUGIN(user_data);
  flutter_desktop_plugin_handle_method_call(plugin, method_call);
}

void flutter_desktop_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  FlutterDesktopPlugin* plugin = FLUTTER_DESKTOP_PLUGIN(
      g_object_new(flutter_desktop_plugin_get_type(), nullptr));

  plugin->registrar = FL_PLUGIN_REGISTRAR(g_object_ref(registrar));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "flutter_desktop",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
