import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'flutter_desktop_platform_interface.dart';

/// An implementation of [FlutterDesktopPlatform] that uses method channels.
class MethodChannelFlutterDesktop extends FlutterDesktopPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_desktop');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String?> getWindowHandle() async {
    final handle = await methodChannel.invokeMethod<String>("getWindowHandle");
    return handle;
  }
}
