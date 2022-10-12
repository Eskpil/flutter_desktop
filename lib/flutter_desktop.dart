import 'flutter_desktop_platform_interface.dart';

class FlutterDesktop {
  Future<String?> getPlatformVersion() {
    return FlutterDesktopPlatform.instance.getPlatformVersion();
  }

  Future<String?> getWindowHandle() {
    return FlutterDesktopPlatform.instance.getWindowHandle();
  }
}
