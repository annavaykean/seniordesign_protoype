import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/analysis_page.dart';
import 'code/settings_page.dart';
import 'code/bluetooth_test_screen.dart';
import 'code/send_bluetooth_data.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'dart:async';

void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  static BluetoothDevice device = null;
  static int testing = 33;
  static StreamSubscription deviceConnection = null;
  static List<BluetoothService> services = new List();

  @override
  Widget build(BuildContext context){
    return MaterialApp(
      title: 'Testing Sign In with Firebase',
      home: new WelcomeScreen(),
      routes: <String, WidgetBuilder> {
        '/WelcomeScreen': (BuildContext context) => new WelcomeScreen(),
        '/DashboardScreen': (BuildContext context) => new DashboardScreen(),
        '/SettingsScreen' : (BuildContext context) => new SettingsScreen(),
        '/AnalysisScreen' : (BuildContext context) => new AnalysisScreen(),
        '/BluetoothTestScreen' : (BuildContext context) => new BluetoothTestScreen(),
        '/SendBluetoothData' : (BuildContext context) => new SendBluetoothData(),
      }
      );
  }
}

class WelcomeScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new Scaffold(
        appBar: AppBar(
          title: const Text('Welcome To My App!'),
        ),
        body: Center(
            child: RaisedButton(
                child: const Text('Enter the App!'),
                onPressed: () => Navigator.of(context).pushNamed('/DashboardScreen')
            )
        )
    );
  }
}



