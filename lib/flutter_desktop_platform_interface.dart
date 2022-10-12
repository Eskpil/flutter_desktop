import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_desktop_method_channel.dart';

abstract class FlutterDesktopPlatform extends PlatformInterface {
  /// Constructs a FlutterDesktopPlatform.
  FlutterDesktopPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterDesktopPlatform _instance = MethodChannelFlutterDesktop();

  /// The default instance of [FlutterDesktopPlatform] to use.
  ///
  /// Defaults to [MethodChannelFlutterDesktop].
  static FlutterDesktopPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [FlutterDesktopPlatform] when
  /// they register themselves.
  static set instance(FlutterDesktopPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<String?> getWindowHandle() {
    throw UnimplementedError('windowHandle() has not been implemented.');
  }
}
