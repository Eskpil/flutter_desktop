import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_desktop/flutter_desktop.dart';
import 'package:flutter_desktop/flutter_desktop_platform_interface.dart';
import 'package:flutter_desktop/flutter_desktop_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockFlutterDesktopPlatform
    with MockPlatformInterfaceMixin
    implements FlutterDesktopPlatform {
  @override
  Future<String?> getPlatformVersion() => Future.value('42');
  Future<String?> getWindowHandle() => Future.value('123');
}

void main() {
  final FlutterDesktopPlatform initialPlatform =
      FlutterDesktopPlatform.instance;

  test('$MethodChannelFlutterDesktop is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelFlutterDesktop>());
  });

  test('getPlatformVersion', () async {
    FlutterDesktop flutterDesktopPlugin = FlutterDesktop();
    MockFlutterDesktopPlatform fakePlatform = MockFlutterDesktopPlatform();
    FlutterDesktopPlatform.instance = fakePlatform;

    expect(await flutterDesktopPlugin.getPlatformVersion(), '42');
  });

  test('getWindowHandle', () async {
    FlutterDesktop flutterDesktopPlugin = FlutterDesktop();
    MockFlutterDesktopPlatform fakePlatform = MockFlutterDesktopPlatform();
    FlutterDesktopPlatform.instance = fakePlatform;

    expect(await flutterDesktopPlugin.getWindowHandle(), '123');
  });
}
