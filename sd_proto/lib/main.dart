import 'package:flutter/material.dart';
import 'code/dashboard_page.dart';
import 'code/analysis_page.dart';
import 'code/settings_page.dart';
import 'code/bluetooth_test_screen.dart';

void main(){
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
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



