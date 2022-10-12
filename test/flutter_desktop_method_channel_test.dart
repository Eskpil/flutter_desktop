import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_desktop/flutter_desktop_method_channel.dart';

void main() {
  MethodChannelFlutterDesktop platform = MethodChannelFlutterDesktop();
  const MethodChannel channel = MethodChannel('flutter_desktop');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
